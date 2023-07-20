
#include "SSDrawMeshPart.hpp"

namespace sssdk
{
	SSDrawMeshPart::SSDrawMeshPart(const SSAnimationPart* setup, ISSCellmaps* cellmaps)
		: SSDrawCellPart{ setup, cellmaps }
		, m_pTargetTexture{ nullptr }
		, m_pMesh2D{ nullptr }
	{
		// TODO
	}

	SSDrawMeshPart::~SSDrawMeshPart()
	{
	}

	void SSDrawMeshPart::draw(const Vec2& canvasOffset) const
	{
		// TODO
	}
}
