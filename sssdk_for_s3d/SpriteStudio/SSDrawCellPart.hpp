#pragma once
#ifndef SS_DRAW_CELL_PART_H_
#define SS_DRAW_CELL_PART_H_

#include "SSDrawPart.hpp"

namespace sssdk
{
	class SSCell;
	class SSCellmap;
	class ISSCellmaps;
	class SSDrawCellPart : public SSDrawPart
	{
	public:

		explicit SSDrawCellPart(StringView ssae, const SSAnimationPart* anim, ISSCellmaps* cellmaps);
		virtual ~SSDrawCellPart();

		virtual void update(int32 frame) override;
		virtual void draw(const Vec2& canvasOffset) const override;

		void setCellmaps(ISSCellmaps* cellmaps);

	protected:

		BlendState getBlendState(DRAW_MODE mode) const;

		String m_packName;
		const SSCellmap* m_pCellmap;
		ISSCellmaps* m_pCellmaps;
		const SSCell* m_pCell;
		Texture m_texture;
	};
}

#endif // !SS_DRAW_CELL_PART_H_
