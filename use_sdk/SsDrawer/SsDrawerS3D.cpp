#include "SsDrawerS3D.h"
#include "../ThirdParty/sssdk/Common/Loader/ssloader_ssae.h"
#include "../ThirdParty/sssdk/Common/Loader/ssloader_ssce.h"
#include "../ThirdParty/sssdk/Common/Loader/ssloader_ssee.h"
#include "../ThirdParty/sssdk/Common/Animator/ssplayer_cellmap.h"
#include "../ThirdParty/sssdk/Common/Animator/ssplayer_PartState.h"
#include "../ThirdParty/sssdk/Common/Animator/ssplayer_mesh.h"

namespace
{
	BlendState GetBlendState(spritestudio6::SsBlendType::_enum mode)
	{
		using spritestudio6::SsBlendType::_enum;
		switch (mode)
		{
		case _enum::mix:       return BlendState{ true, Blend::SrcAlpha    , Blend::InvSrcAlpha , BlendOp::Add };
		case _enum::mul:       return BlendState{ true, Blend::Zero        , Blend::SrcColor    , BlendOp::Add };
		case _enum::add:       return BlendState{ true, Blend::SrcAlpha    , Blend::One         , BlendOp::Add };
		case _enum::sub:       return BlendState{ true, Blend::SrcAlpha    , Blend::One         , BlendOp::RevSubtract, Blend::Zero, Blend::DestAlpha };
		case _enum::mulalpha:  return BlendState{ true, Blend::DestColor   , Blend::InvSrcAlpha , BlendOp::Add };
		case _enum::screen:    return BlendState{ true, Blend::InvDestColor, Blend::One         , BlendOp::Add };
		case _enum::exclusion: return BlendState{ true, Blend::InvDestColor, Blend::InvDestColor, BlendOp::Add };
		case _enum::invert:    return BlendState{ true, Blend::InvDestColor, Blend::Zero        , BlendOp::Add };

		default: break;
		}
		return BlendState{ BlendState::Default2D };
	}
}
namespace s3d
{

	SsDrawerS3D::SsDrawerS3D()
		: m_renderState{ none }
	{
	}

	SsDrawerS3D::~SsDrawerS3D()
	{
	}

	void SsDrawerS3D::initialize()
	{
		m_renderState.reset();
	}

	void SsDrawerS3D::renderSetup()
	{
		m_renderState.reset();
	}

	void SsDrawerS3D::renderPart(spritestudio6::SsPartState* state)
	{
		//非表示なので処理をしない
		if (state->hide)
		{
			return;
		}

		auto* cell = state->cellValue.cell;
		if (cell == nullptr)
		{
			return;
		}

		Texture& texture = state->cellValue.texture;
		if (texture.isEmpty())
		{
			return;
		}

		const Size& texturePixelSize = texture.size();

		this->execMask(state);

		float alpha = state->alpha;
		if (state->localalpha != 1.0f)
		{
			alpha = state->localalpha;
		}

		TextureFilter filterMode = TextureFilter::Nearest;
		if (state->cellValue.filterMode == spritestudio6::SsTexFilterMode::linear)
		{
			filterMode = TextureFilter::Linear;
		}

		TextureAddressMode wrapMode = TextureAddressMode::Repeat;
		if (state->cellValue.wrapMode == spritestudio6::SsTexWrapMode::clamp)
		{
			wrapMode = TextureAddressMode::Clamp;
		}
		if (state->cellValue.wrapMode == spritestudio6::SsTexWrapMode::mirror)
		{
			wrapMode = TextureAddressMode::Mirror;
		}

		const SamplerState samplerState{ wrapMode, filterMode };
		const ScopedRenderStates2D renderState{ GetBlendState(state->alphaBlendType), samplerState };

		if (state->partType == spritestudio6::SsPartType::mesh)
		{
			this->drawMesh(state->meshPart.get(), alpha);
			return;
		}

		if (state->noCells)
		{
			//セルが無いので描画を行わない
		}
		else
		{
			RectF srcRect{ RectF::Empty() };
			srcRect.setPos(static_cast<double>(cell->pos.x), static_cast<double>(cell->pos.y));
			srcRect.setSize(static_cast<double>(cell->size.x), static_cast<double>(cell->size.y));
			srcRect = srcRect.scaled(static_cast<double>(state->uvScale.x), static_cast<double>(state->uvScale.y));
			float dx = state->matrix[4 * 3 + 0] + cell->pivot.x * srcRect.w;
			float dy = state->matrix[4 * 3 + 1] - cell->pivot.y * srcRect.h;
			float sx = state->matrix[4 * 0 + 0];
			float sy = state->matrix[4 * 1 + 1];
			srcRect.moveBy(Vec2{ state->uvTranslate.x * texture.width(), state->uvTranslate.y * texture.height() });
			texture(srcRect)
				.scaled(1.0 / state->uvScale.x, 1.0 / state->uvScale.y)
				.flipped(state->imageFlipV)
				.mirrored(state->imageFlipH)
				.scaled(static_cast<double>(sx), static_cast<double>(sy))
				.drawAt(static_cast<double>(dx), static_cast<double>(-dy));
		}
	}

	void SsDrawerS3D::execMask(spritestudio6::SsPartState* state)
	{
	}

	void SsDrawerS3D::clearMask()
	{
	}

	void SsDrawerS3D::renderSpriteSimple(float matrix[16], int width, int height, spritestudio6::SsVector2& pivot, spritestudio6::SsVector2 uv1, spritestudio6::SsVector2 uv2, const spritestudio6::SsFColor& color)
	{
	}

	void SsDrawerS3D::SetAlphaBlendMode(spritestudio6::SsBlendType::_enum type)
	{
		m_renderState.emplace(GetBlendState(type));
	}

	void SsDrawerS3D::SetTexture(spritestudio6::SsCellValue* cell)
	{
	}

	void SsDrawerS3D::enableMask(bool flag)
	{
	}

	void SsDrawerS3D::drawMesh(spritestudio6::SsMeshPart* mesh, float alpha)
	{
		if (mesh == nullptr)
		{
			return;
		}
		if (alpha == 0.0f)
		{
			return;
		}
		spritestudio6::SsPartState* state = mesh->myPartState;
	}

}
