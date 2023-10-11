#pragma once
#include <Siv3D.hpp>
#include "../ThirdParty/sssdk/Common/Animator/ssplayer_render.h"

namespace spritestudio6
{
	class SsMeshPart;
}

namespace s3d
{
	class SsDrawerS3D : public spritestudio6::ISsRenderer
	{
	public:
		explicit SsDrawerS3D();
		virtual ~SsDrawerS3D();
		virtual void	initialize() override;
		virtual void	renderSetup() override;
		virtual void	renderPart(spritestudio6::SsPartState* state) override;
		virtual void	execMask(spritestudio6::SsPartState* state) override;
		virtual void	clearMask() override;

		virtual void	renderSpriteSimple(float matrix[16],
											int width, int height,
											spritestudio6::SsVector2& pivot,
											spritestudio6::SsVector2 uv1, spritestudio6::SsVector2 uv2,
											const spritestudio6::SsFColor& color) override;

		virtual void	SetAlphaBlendMode(spritestudio6::SsBlendType::_enum type) override;
		virtual void	SetTexture(spritestudio6::SsCellValue* cell) override;
		virtual void	enableMask(bool flag) override;

	private:

		virtual void drawMesh(spritestudio6::SsMeshPart* mesh, float alpha);

		Optional<ScopedRenderStates2D> m_renderState;
	};
}
