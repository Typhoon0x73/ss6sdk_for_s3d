
#include "SSProject.hpp"
#include "SSDrawCellPart.hpp"

namespace sssdk
{
	SSDrawCellPart::SSDrawCellPart(StringView ssae, const SSAnimationPart* anim, ISSCellmaps* cellmaps)
		: SSDrawPart{ anim }
		, m_packName{ ssae }
		, m_pCellmap{ nullptr }
		, m_pCellmaps{ nullptr }
		, m_pCell{ nullptr }
		, m_texture{}
	{
		setCellmaps(cellmaps);
		if (not m_pCellmaps)
		{
			return;
		}
		const auto& attributes = anim->getAttributes();
		for (const auto& attribute : attributes)
		{
			if (attribute.getAttributeKind() == ATTRIBUTE_KIND_CELL)
			{
				const auto& keyValues = attribute.getKeyValues();
				if (keyValues.size() > 0 and keyValues[0].getValue().getType() == SSValue::TYPE_CELL)
				{
					if (const auto* first = keyValues[0].getValue().getValue<SSCellValue>())
					{
						m_cell.mapId = first->mapId;
						m_cell.name = first->name;
						break;
					}
				}
			}
		}
		m_pCellmap = m_pCellmaps->getCellmap(ssae, m_cell.mapId);
		if (not m_pCellmap)
		{
			return;
		}
		m_pCell = m_pCellmap->getCell(m_cell.name);
	}

	SSDrawCellPart::~SSDrawCellPart()
	{
	}

	void SSDrawCellPart::update(int32 frame)
	{
		SSDrawPart::update(frame);

		m_pCellmap = m_pCellmaps->getCellmap(m_packName, m_cell.mapId);
		if (not m_pCellmap)
		{
			return;
		}
		m_pCell = m_pCellmap->getCell(m_cell.name);
	}

	void SSDrawCellPart::draw(const Vec2& canvasOffset) const
	{
		if (m_isHide)
		{
			return;
		}
		if (not m_pCellmaps or not m_pCell)
		{
			return;
		}
		if (not m_pCellmap)
		{
			return;
		}
		if (m_uvScale.x == 0.0f or m_uvScale.y == 0.0f)
		{
			return;
		}

		RectF srcRect{ RectF::Empty() };
		srcRect.setPos(m_pCell->getPos());
		srcRect.setSize(m_pCell->getSize());
		srcRect = srcRect.scaled(m_uvScale.x, m_uvScale.y);

		Vec2 cellPivot = m_pCell->getPivot(); // セルの原点
		cellPivot.y *= -1;
		const auto& cellOffset = srcRect.size * cellPivot;
		Float3 trans, scale;
		Quaternion rotate;
		m_worldMatrix.decompose(scale, rotate, trans);
		const auto& drawAtPos = canvasOffset + cellOffset + Vec2{ trans.x, trans.y };
		float alpha = getAlpha() * m_localAlpha;
		{
			const BlendState blend = GetBlendState(m_pModelPart->getAlphaBlendType());
			const SamplerState sampler{ m_pCellmap->getWrapMode(), m_pCellmap->getFilterMode() };
			const ScopedRenderStates2D renderState{ blend, sampler };
			const Texture drawTexture = TextureAsset(m_pCellmap->getTextureKey());
			srcRect.moveBy(m_uvTranslate * drawTexture.size());
			drawTexture(srcRect)
				.scaled(1.0 / m_uvScale.x, 1.0 / m_uvScale.y)
				.flipped(m_isImageFlipV)
				.mirrored(m_isImageFlipH)
				.scaled(scale.x * m_localScale.x, scale.y * m_localScale.y)
				.rotated(Math::ToRadians(-getRotation().z))
				.drawAt(drawAtPos, ColorF{ 1.0, 1.0, 1.0, alpha });
		}
	}

	void SSDrawCellPart::setCellmaps(ISSCellmaps* cellmaps)
	{
		m_pCellmaps = cellmaps;
	}

	BlendState SSDrawCellPart::GetBlendState(DRAW_MODE mode)
	{
		switch (mode)
		{
		case DRAW_MODE_MIX:       return BlendState{ true, Blend::SrcAlpha    , Blend::InvSrcAlpha , BlendOp::Add };
		case DRAW_MODE_MUL:       return BlendState{ true, Blend::Zero        , Blend::SrcColor    , BlendOp::Add };
		case DRAW_MODE_ADD:       return BlendState{ true, Blend::SrcAlpha    , Blend::One         , BlendOp::Add };
		case DRAW_MODE_SUB:       return BlendState{ true, Blend::SrcAlpha    , Blend::One         , BlendOp::RevSubtract, Blend::Zero, Blend::DestAlpha };
		case DRAW_MODE_MUL_ALPHA: return BlendState{ true, Blend::DestColor   , Blend::InvSrcAlpha , BlendOp::Add };
		case DRAW_MODE_SCREEN:    return BlendState{ true, Blend::InvDestColor, Blend::One         , BlendOp::Add };
		case DRAW_MODE_EXCLUSION: return BlendState{ true, Blend::InvDestColor, Blend::InvDestColor, BlendOp::Add };
		case DRAW_MODE_INVERT:    return BlendState{ true, Blend::InvDestColor, Blend::Zero        , BlendOp::Add };

		default: break;
		}
		return BlendState{ BlendState::Default2D };
	}
}
