#pragma once
#ifndef SS_CELL_H_
#define SS_CELL_H_

#include <Siv3D.hpp>

namespace sssdk
{
	class SSCell
	{
	public:

		explicit SSCell();
		explicit SSCell(const XMLElement& element);
		virtual ~SSCell();

		bool load(const XMLElement& element);

		SIV3D_NODISCARD_CXX20 StringView getName() const;

		SIV3D_NODISCARD_CXX20 const Point& getPos() const;
		SIV3D_NODISCARD_CXX20 const Size& getSize() const;
		SIV3D_NODISCARD_CXX20 const Vec2& getPivot() const;
		SIV3D_NODISCARD_CXX20 double getRotated() const;

		SIV3D_NODISCARD_CXX20 StringView getOrgImageName() const;

		SIV3D_NODISCARD_CXX20 bool isMesh() const;

		SIV3D_NODISCARD_CXX20 const Array<Point>& getInnerPoints() const;
		SIV3D_NODISCARD_CXX20 const Array<Point>& getOuterPoints() const;
		SIV3D_NODISCARD_CXX20 const Array<Point>& getMeshPoints() const;
		SIV3D_NODISCARD_CXX20 const Array<TriangleIndex32>& getMeshTriangleIndices() const;

	private:

		String m_name;

		Point m_pos;
		Size m_size;
		Vec2 m_pivot;
		double m_rotated;

		String m_orgImageName;

		bool m_isMesh;
		Array<Point> m_innerPoints;
		Array<Point> m_outerPoints;
		Array<Point> m_meshPoints;
		Array<TriangleIndex32> m_meshTriangleIndices;
	};
}

#endif // !SS_CELL_H_
