
#include "SSAnimationController.hpp"
#include "SSAnimeSettings.hpp"
#include "SSProject.hpp"
#include "SSDrawPart.hpp"
#include "SSDrawCellPart.hpp"

namespace sssdk
{
	SSAnimationController::SSAnimationController()
		: m_tick{ 0.0 }
		, m_isLoop{ false }
		, m_pAnimeSettings{ nullptr }
		, m_drawParts{}
		, m_isAnimationEnd{ false }
	{
	}

	SSAnimationController::~SSAnimationController()
	{
	}

	bool SSAnimationController::build(SSProject* proj, StringView pack, StringView anim)
	{
		auto animPack = proj->getAnimPack(pack);
		if (not animPack)
		{
			return false;
		}
		auto animation = animPack->getAnimation(anim);
		if (not animation)
		{
			return false;
		}
		auto setupAnim = animPack->getSetupAnimation();
		if (not setupAnim)
		{
			return false;
		}
		if (animation->isOverrideSettings())
		{
			m_pAnimeSettings = &(animation->getAnimeSettings());
		}
		else
		{
			m_pAnimeSettings = &(proj->getAnimeSettings());
		}
		const auto& model = animPack->getModel();
		const auto& animParts = animation->getAnimParts();
		for (const auto& part : animParts)
		{
			const auto* modelPart = model.getPart(part.getName());
			if (not modelPart)
			{
				continue;
			}
			std::unique_ptr<SSDrawPart> drawPart;
			switch (modelPart->getType())
			{
			case ModelPartType::normal:
				drawPart.reset(new SSDrawCellPart(&part, proj));
				break;
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
			default:
				drawPart.reset(new SSDrawPart(&part));
				break;
			}
			if (drawPart)
			{
				if (const auto* setupPart = setupAnim->getAnimPart(part.getName()))
				{
					drawPart->setSetupPart(setupPart);
				}
				drawPart->setModelPart(modelPart);
			}
			m_drawParts.emplace_back(std::move(drawPart));
		}
		for (const auto& part : animParts)
		{
			const auto* modelPart = model.getPart(part.getName());
			if (not modelPart)
			{
				continue;
			}
			const int32 parentIndex = modelPart->getParentIndex();
			if (parentIndex < 0)
			{
				continue;
			}
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
				(*findPart)->setParent(findParentPart->get());
			}
		}
		return true;
	}

	void SSAnimationController::update(double deltaTime)
	{
		setTime(m_tick + deltaTime);
	}

	void SSAnimationController::draw(const Vec2& pos) const
	{
		Vec2 pivot = m_pAnimeSettings->getPivot();
		pivot.y *= -1;
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

