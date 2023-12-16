
#include "SSAnimationController.hpp"
#include "SSAnimeSettings.hpp"
#include "SSProject.hpp"
#include "SSDrawPart.hpp"
#include "SSDrawCellPart.hpp"

namespace sssdk
{
	namespace
	{
		SSDrawPart* createDrawPart(const SSProject* proj, StringView pack, const SSAnimationPart* part, const ModelPartType& type)
		{
			switch (type)
			{
			case ModelPartType::normal:
				return new SSDrawCellPart(pack, part, proj);
			case ModelPartType::mesh:
				//break;
			case ModelPartType::null:
			case ModelPartType::shape:
			case ModelPartType::text:
			case ModelPartType::nines:
			case ModelPartType::instance:
			case ModelPartType::armature:
			case ModelPartType::effect:
			case ModelPartType::movenode:
			case ModelPartType::constraint:
			case ModelPartType::mask:
			case ModelPartType::joint:
			case ModelPartType::bonepoint:
			case ModelPartType::transform_constraint:
			case ModelPartType::camera:
			case ModelPartType::audio:
				return new SSDrawPart(part);
			default:
				break;
			}
			return nullptr;
		}
	}

	SSAnimationController::SSAnimationController()
		: m_pProject{ nullptr }
		, m_pAnimationPack{ nullptr }
		, m_pAnimation{ nullptr }
		, m_tick{ 0.0 }
		, m_isLoop{ false }
		, m_pAnimeSettings{ nullptr }
		, m_drawParts{}
		, m_isAnimationEnd{ false }
	{
	}

	SSAnimationController::~SSAnimationController()
	{
	}

	bool SSAnimationController::build(const SSProject* proj, StringView pack, StringView anim)
	{
		m_pProject = proj;
		if (m_pProject == nullptr)
		{
			Logger << U"アニメーションのビルドに失敗。引数[proj]がnullです。";
			return false;
		}

		m_pAnimationPack = proj->getAnimPack(pack);
		if (not m_pAnimationPack)
		{
			Logger << U"パック名:{} がプロジェクト内に見つかりませんでした。"_fmt(pack);
			return false;
		}

		m_pAnimation = m_pAnimationPack->getAnimation(anim);
		if (not m_pAnimation)
		{
			Logger << U"パック名:{} にアニメーション名:{} が見つかりませんでした。"_fmt(pack, anim);
			return false;
		}

		const auto& setupAnim = m_pAnimationPack->getSetupAnimation();
		if (not setupAnim)
		{
			Logger << U"アニメーションのビルドに失敗。セットアップ用のアニメーションがありません。";
			return false;
		}

		// アニメーション設定を参照しておく
		{
			if (m_pAnimation->isOverrideSettings())
			{
				m_pAnimeSettings = &(m_pAnimation->getAnimeSettings());
			}
			else
			{
				m_pAnimeSettings = &(m_pProject->getAnimeSettings());
			}
		}

		// 描画パーツの生成
		createDrawParts();

		// アニメーションパーツの親子付け
		linkPart();

		// 描画パーツをソート
		sortDrawParts();

		return true;
	}

	void SSAnimationController::update(double deltaTime)
	{
		setTime(m_tick + deltaTime);
	}

	void SSAnimationController::draw(const Vec2& pos) const
	{
		Vec2 pivot = m_pAnimeSettings->getPivot();
		pivot.y *= -1; // スプライトスタジオでは上が＋なので反転しておく
		const auto& canvasOffset = m_pAnimeSettings->getCanvasSize() * (pivot + Vec2{ 0.5, 0.5 }) + pos;
		for (const auto& it : m_drawParts)
		{
			if (not it)
			{
				continue;
			}
			it->draw(canvasOffset);
		}
	}

	void SSAnimationController::drawAt(const Vec2& pos) const
	{
	}

	void SSAnimationController::setTime(double t)
	{
		m_tick = t;
		updatePart(calcFrame());
	}

	void SSAnimationController::setLoop(bool isLoop)
	{
		m_isLoop = isLoop;
	}

	double SSAnimationController::getTime() const
	{
		return m_tick;
	}

	bool SSAnimationController::isLoop() const
	{
		return m_isLoop;
	}

	bool SSAnimationController::isAnimationEnd() const
	{
		return m_isAnimationEnd;
	}

	void SSAnimationController::createDrawParts()
	{
		const auto& model = m_pAnimationPack->getModel();
		const auto& animParts = m_pAnimation->getAnimParts();
		const auto& setupAnim = m_pAnimationPack->getSetupAnimation();
		for (const auto& part : animParts)
		{
			const auto* modelPart = model.getPart(part.getName());
			if (not modelPart)
			{
				continue;
			}
			SSDrawPart* drawPart = createDrawPart(m_pProject, m_pAnimationPack->getName(), &part, modelPart->getType());
			if (drawPart)
			{
				if (const auto* setupPart = setupAnim->getAnimPart(part.getName()))
				{
					drawPart->setSetupPart(setupPart);
				}
				drawPart->setModelPart(modelPart);
			}
			m_drawParts.emplace_back(drawPart);
		}
	}

	void SSAnimationController::linkPart()
	{
		const auto& model = m_pAnimationPack->getModel();
		const auto& animParts = m_pAnimation->getAnimParts();
		for (const auto& part : animParts)
		{
			// モデルパーツがあるか確認
			const auto* modelPart = model.getPart(part.getName());
			if (not modelPart)
			{
				continue;
			}

			// 親パーツがあるか確認
			const int32 parentIndex = modelPart->getParentIndex();
			if (parentIndex < 0)
			{
				continue;
			}

			// 描画パーツからモデルパーツと一致するものを探す
			const int32 index = modelPart->getArrayIndex();
			const auto& partName = model.getPart(index)->getName();
			const auto findPart = std::find_if(
				m_drawParts.begin(),
				m_drawParts.end(),
				[&](const std::unique_ptr<SSDrawPart>& p) { return p->getAnimPart()->getName() == partName; }
			);
			if (findPart == m_drawParts.end())
			{
				continue;
			}
			if (findPart->get())
			{
				continue;
			}
			// 親パーツも探す
			const auto& parentPartName = model.getPart(parentIndex)->getName();
			const auto findParentPart = std::find_if(
				m_drawParts.begin(),
				m_drawParts.end(),
				[&](const std::unique_ptr<SSDrawPart>& p) { return p->getAnimPart()->getName() == parentPartName; }
			);
			if (findParentPart == m_drawParts.end())
			{
				continue;
			}
			// 親を設定
			(*findPart)->setParent(findParentPart->get());
		}
	}

	void SSAnimationController::sortDrawParts()
	{
		if (not m_pAnimeSettings)
		{
			return;
		}
		switch (m_pAnimeSettings->getSortMode())
		{
		case SortMode::prio:
			m_drawParts.sort_by(SSDrawPart::CompareSortFuncByPrio);
			break;
		case SortMode::z:
			m_drawParts.sort_by(SSDrawPart::CompareSortFuncByZ);
			break;
		default:
			break;
		}
	}

	int32 SSAnimationController::calcFrame()
	{
		if (not m_pAnimeSettings)
		{
			return 0;
		}
		const int32 startFrame = m_pAnimeSettings->getStartFrame();
		const int32 endFrame = m_pAnimeSettings->getEndFrame();
		const int32 fps = m_pAnimeSettings->getFps();
		int32 frame = startFrame + static_cast<int32>(fps * m_tick);
		if (frame > endFrame)
		{
			if (m_isLoop)
			{
				m_tick = 0.0;
				frame = startFrame;
			}
			else
			{
				frame = endFrame;
				m_isAnimationEnd = true;
			}
		}
		return frame;
	}

	void SSAnimationController::updatePart(int32 frame)
	{
		for (const auto& it : m_drawParts)
		{
			if (not it)
			{
				continue;
			}
			it->update(frame);
		}
	}
}

