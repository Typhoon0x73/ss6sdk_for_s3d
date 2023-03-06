
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

		// -------------------------------------------------
		// 解析用関数
		// -------------------------------------------------
		void parseName         (const XMLElement& element);
		void parsePos          (const XMLElement& element);
		void parseSize         (const XMLElement& element);
		void parsePivot        (const XMLElement& element);
		void parseRotated      (const XMLElement& element);
		void parseOrgImageName (const XMLElement& element);
		void parsePosStable    (const XMLElement& element);
		void parseIsmesh       (const XMLElement& element);
		void parseDivtype      (const XMLElement& element);
		void parseInnerPoint   (const XMLElement& element);
		void parseOuterPoint   (const XMLElement& element);
		void parseMeshPointList(const XMLElement& element);
		void parseMeshTriList  (const XMLElement& element);

		String               m_name         { U"" };
		Point                m_pos          { 0, 0 };
		Size                 m_size         { 0, 0 };
		Float2               m_pivot        { 0.0f, 0.0f };
		int32                m_rotated      { 0 };
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
