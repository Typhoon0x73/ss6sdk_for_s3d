
# pragma once
# include <Siv3D.hpp>

namespace s3d
{
	class SSCellInfo
	{
	public:

		SSCellInfo() = default;

		~SSCellInfo() = default;

		explicit SSCellInfo(const XMLElement& cell, bool createEditorParam);

		bool load(const XMLElement& cell, bool createEditorParam);

	private:

		String               m_name         { U"" };
		Point                m_pos          { 0, 0 };
		Size                 m_size         { 0, 0 };
		Float2               m_pivot        { 0.0f, 0.0f };
		int32_t              m_rotated      { 0 };
		String               m_orgImageName { U"" };
		bool                 m_posStable    { false };
		bool                 m_ismesh       { false };
		String               m_divtype      { U"" };
		Array<Point>         m_innerPoint   {};
		Array<Point>         m_outerPoint   {};
		Array<Point>         m_meshPointList{};
		Array<TriangleIndex> m_meshTriList  {};
	};
}
