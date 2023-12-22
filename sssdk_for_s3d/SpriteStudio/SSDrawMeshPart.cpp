
#include "SSDrawMeshPart.hpp"

namespace sssdk
{
	SSDrawMeshPart::SSDrawMeshPart(StringView ssae, const SSAnimationPart* anim, const ISSCellmaps* cellmaps)
		: SSDrawCellPart{ ssae, anim, cellmaps }
		, m_pTargetTexture{ nullptr }
		, m_pMesh2D{ nullptr }
	{
		// TODO
	}

	SSDrawMeshPart::~SSDrawMeshPart()
	{
	}

	void SSDrawMeshPart::update(int32 frame)
	{
		// TODO
		SSDrawCellPart::update(frame);
	}

	void SSDrawMeshPart::draw(const Vec2& canvasOffset) const
	{
		// TODO
		SSDrawCellPart::draw(canvasOffset);
	}
}
