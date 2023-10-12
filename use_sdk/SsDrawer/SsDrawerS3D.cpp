﻿#include "SsDrawerS3D.h"
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

		ColorF colors[4];
		if (state->is_parts_color)
		{

		}
		else
		{
			// パーツカラー無し
			for (int i = 0; i < 5; i++)
			{
				state->colors[i * 4 + 3] = alpha;
			}
		}
		for (int i = 0; i < 4; i++)
		{
			colors[i].set(
				state->colors[i * 4 + 0],
				state->colors[i * 4 + 1],
				state->colors[i * 4 + 2],
				state->colors[i * 4 + 3]);
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
			float dx = state->matrixLocal[4 * 3 + 0] - cell->pivot.x * srcRect.w - state->pivotOffset.x * srcRect.w;
			float dy = state->matrixLocal[4 * 3 + 1] - cell->pivot.y * srcRect.h - state->pivotOffset.y * srcRect.h * -1;
			float sx = state->scale.x * state->localscale.x;
			float sy = state->scale.y * state->localscale.y;
			srcRect.moveBy(Vec2{ state->uvTranslate.x * texture.width(), state->uvTranslate.y * texture.height() });
			texture(srcRect)
				.scaled(1.0 / state->uvScale.x, 1.0 / state->uvScale.y)
				.flipped(state->imageFlipV)
				.mirrored(state->imageFlipH)
				.scaled(static_cast<double>(sx), static_cast<double>(sy))
				.rotated(Math::ToRadians(static_cast<double>(-state->rotation.z)))
				.drawAt(Vec2{ static_cast<double>(dx), static_cast<double>(-dy) },
					colors[0]/*, colors[1], colors[2], colors[3]*/);
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
		Buffer2D* pDrawMesh = mesh->buffer2D.get();
		if (pDrawMesh == nullptr)
		{
			return;
		}
		auto* state = mesh->myPartState;
		if (state->is_parts_color)
		{
			// パーツカラーがある時だけブレンド計算する
			Float4 col{ Float4::One() };
			if (state->partsColorValue.target == spritestudio6::SsColorBlendTarget::whole)
			{
				// 単色
				const auto& colorBlendValue = state->partsColorValue.color;
				// TODO:
			}
			else
			{
				//4頂点はとりあえず左上のRGBAと Rate
				const auto& colorBlendValue = state->partsColorValue.colors[0];
				// TODO:
				if (state->partsColorValue.blendType == spritestudio6::SsBlendType::mix)
				{
					col.w = 1; // ミックス-頂点単位では A に Rate が入っておりアルファは無効なので1にしておく。
				}
			}
			for (size_t i = 0; i < mesh->ver_size; i++)
			{
				pDrawMesh->vertices[i].color.x = col.x;
				pDrawMesh->vertices[i].color.y = col.y;
				pDrawMesh->vertices[i].color.z = col.z;
				pDrawMesh->vertices[i].color.w = col.w * alpha; // 不透明度を適用する。
			}
		}
		else
		{
			for (size_t i = 0; i < mesh->ver_size; i++)
			{
				pDrawMesh->vertices[i].color.x = 1.0f;
				pDrawMesh->vertices[i].color.y = 1.0f;
				pDrawMesh->vertices[i].color.z = 1.0f;
				pDrawMesh->vertices[i].color.w = alpha;
			}
		}
		pDrawMesh->draw(mesh->targetTexture);
	}

}
