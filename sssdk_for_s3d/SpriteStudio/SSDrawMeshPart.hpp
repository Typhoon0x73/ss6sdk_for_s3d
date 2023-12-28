#pragma once
#ifndef SS_DRAW_MESH_PART_H_
#define SS_DRAW_MESH_PART_H_

#include "SSBoneWeight.hpp"
#include "SSDrawCellPart.hpp"

namespace sssdk
{
	class SSDrawMeshPart : public SSDrawCellPart
	{
	public:

		explicit SSDrawMeshPart(StringView ssae, const SSAnimationPart* anim, const ISSCellmaps* cellmaps);
		virtual ~SSDrawMeshPart();

		void updateTransformMesh();
		virtual void draw() const override;

		size_t getVertexNum() const;
		Array<SSBoneWeight>& getBindBoneInfomations();

	private:

		void makeMesh();

		Float2 getOffsetWorldVerticesFromKey(size_t index);
		Float2 getOffsetLocalVertices(size_t index);

	protected:

		std::unique_ptr<MSRenderTexture> m_pTargetTexture;
		std::unique_ptr<Buffer2D> m_pMesh2D;
		Array<Float2> m_offsetWorldVertices;
		Array<Float2> m_vertices;
		Array<ColorF> m_weightColors;
		Array<SSBoneWeight> m_bindBoneInfomations;

	};
}

#endif // !SS_DRAW_MESH_PART_H_
