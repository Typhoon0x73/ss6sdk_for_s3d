#pragma once
#ifndef SS_DRAW_CELL_PART_H_
#define SS_DRAW_CELL_PART_H_

#include "SSDrawPart.hpp"

namespace sssdk
{
	class ISSCellmaps;
	class SSDrawCellPart : public SSDrawPart
	{
	public:

		explicit SSDrawCellPart(const SSAnimationPart* setup, ISSCellmaps* cellmaps);
		virtual ~SSDrawCellPart();

		virtual void draw(const Vec2& canvasOffset) const override;

		void setCellmaps(ISSCellmaps* cellmaps);

	private:

		ISSCellmaps* m_pCellmaps;
		const SSCell* m_pCell;
		Texture m_texture;
	};
}

#endif // !SS_DRAW_CELL_PART_H_
