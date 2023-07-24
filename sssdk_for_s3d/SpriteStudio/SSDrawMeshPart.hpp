#pragma once
#ifndef SS_DRAW_MESH_PART_H_
#define SS_DRAW_MESH_PART_H_

#include "SSDrawCellPart.hpp"

namespace sssdk
{
	class SSDrawMeshPart : public SSDrawCellPart
	{
	public:

		explicit SSDrawMeshPart(StringView ssae, const SSAnimationPart* anim, ISSCellmaps* cellmaps);
		virtual ~SSDrawMeshPart();

		virtual void draw(const Vec2& canvasOffset) const override;

	protected:

		std::unique_ptr<MSRenderTexture> m_pTargetTexture;
		std::unique_ptr<Buffer2D> m_pMesh2D;

	};
}

#endif // !SS_DRAW_MESH_PART_H_
