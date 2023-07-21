
#include "SSProject.hpp"
#include "SSDrawCellPart.hpp"

namespace sssdk
{
	SSDrawCellPart::SSDrawCellPart(StringView ssae, const SSAnimationPart* anim, ISSCellmaps* cellmaps)
		: SSDrawPart{ anim }
		, m_packName{ ssae }
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
		if (auto cellmap = m_pCellmaps->getCellmap(ssae, m_cell.mapId))
		{
			m_pCell = cellmap->getCell(m_cell.name);
		}
	}

	SSDrawCellPart::~SSDrawCellPart()
	{
	}

	void SSDrawCellPart::draw(const Vec2& canvasOffset) const
	{
		if (not m_pCellmaps or not m_pCell)
		{
			return;
		}
		const auto* cellmap = m_pCellmaps->getCellmap(m_packName, m_cell.mapId);
		if (not cellmap)
		{
			return;
		}

		BlendState blend = BlendState::Default2D;
		switch (m_pModelPart->getAlphaBlendType())
		{
		case DRAW_MODE_MUL: blend = BlendState::Multiplicative; break;
		case DRAW_MODE_ADD: blend = BlendState::Additive; break;
		case DRAW_MODE_SUB: blend = BlendState::Subtractive; break;
		case DRAW_MODE_MUL_ALPHA: blend = BlendState::Premultiplied; break;
		case DRAW_MODE_MIX:
		case DRAW_MODE_SCREEN:
		case DRAW_MODE_EXCLUSION:
		case DRAW_MODE_INVERT:
		default:
			break;
		}

		RectF srcRect{ RectF::Empty() };
		srcRect.setPos(m_pCell->getPos());
		srcRect.setSize(m_pCell->getSize());

		Vec2 cellPivot = m_pCell->getPivot(); // セルの原点
		cellPivot.y *= -1;
		const auto& cellOffset = srcRect.size * cellPivot;
		Float3 trans, scale;
		Quaternion rotate;
		m_worldMatrix.decompose(scale, rotate, trans);
		const auto& drawAtPos = canvasOffset + cellOffset + Vec2{ trans.x, -trans.y };
		{
			const ScopedRenderStates2D renderState{ blend, SamplerState(cellmap->getWrapMode(), cellmap->getFilterMode()) };
			const Texture drawTexture = TextureAsset(cellmap->getTextureKey());
			drawTexture(srcRect)
				.flipped(m_isImageFlipV)
				.mirrored(m_isImageFlipH)
				.rotated(Math::ToRadians(-getRotation().z))
				.drawAt(drawAtPos);
		}
	}

	void SSDrawCellPart::setCellmaps(ISSCellmaps* cellmaps)
	{
		m_pCellmaps = cellmaps;
	}
}
