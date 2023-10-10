#include "SsDrawerS3D.h"
#include "../ThirdParty/sssdk/Common/Loader/ssloader_ssae.h"
#include "../ThirdParty/sssdk/Common/Loader/ssloader_ssce.h"
#include "../ThirdParty/sssdk/Common/Loader/ssloader_ssee.h"
#include "../ThirdParty/sssdk/Common/Animator/ssplayer_cellmap.h"
#include "../ThirdParty/sssdk/Common/Animator/ssplayer_PartState.h"

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

}
