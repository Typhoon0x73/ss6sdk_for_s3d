
#include "SSCell.hpp"

namespace sssdk
{
	SSCell::SSCell()
		: m_name{ U"" }
		, m_pos{ 0, 0 }
		, m_size{ 0, 0 }
		, m_pivot{ 0.0, 0.0 }
		, m_rotated{ 0.0 }
		, m_orgImageName{ U"" }
		, m_isMesh{ false }
		, m_innerPoints{}
		, m_outerPoints{}
		, m_meshPoints{}
		, m_meshTriangleIndexs{}
	{
	}

	SSCell::SSCell(const XMLElement& element)
		: SSCell()
	{
		load(element);
	}

	SSCell::~SSCell()
	{
	}

	bool SSCell::load(const XMLElement& element)
	{
		if (element.name() != U"cell")
		{
			return false;
		}
		for (auto child = element.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"name")
			{
				m_name = child.text();
			}
			else if (name == U"pos")
			{
				const auto& lines = child.text().split(U' ');
				m_pos.x = ParseOr<int32, int32>(lines[0], 0);
				m_pos.y = ParseOr<int32, int32>(lines[1], 0);
			}
			else if (name == U"size")
			{
				const auto& lines = child.text().split(U' ');
				m_size.x = ParseOr<int32, int32>(lines[0], 0);
				m_size.y = ParseOr<int32, int32>(lines[1], 0);
			}
			else if (name == U"pivot")
			{
				const auto& lines = child.text().split(U' ');
				m_pivot.x = ParseOr<double, double>(lines[0], 0.0);
				m_pivot.y = ParseOr<double, double>(lines[1], 0.0);
			}
			else if (name == U"rotated")
			{
				m_rotated = ParseOr<double, double>(child.text(), 0.0);
			}
			else if (name == U"orgImageName")
			{
				m_orgImageName = child.text();
			}
			else if (name == U"ismesh")
			{
				m_isMesh = (ParseOr<int32, int32>(child.text(), 0) == 1);
			}
			else if (name == U"innerPoint")
			{
				for (auto value = child.firstChild(); value; value = value.nextSibling())
				{
					if (value.name() == U"value")
					{
						const auto& lines = value.text().split(U' ');
						if (not lines.isEmpty())
						{
							int32 x = ParseOr<int32, int32>(lines[0], 0);
							int32 y = ParseOr<int32, int32>(lines[1], 0);
							m_innerPoints.emplace_back(x, y);
						}
					}
				}
			}
			else if (name == U"outerPoint")
			{
				for (auto value = child.firstChild(); value; value = value.nextSibling())
				{
					if (value.name() == U"value")
					{
						const auto& lines = value.text().split(U' ');
						if (not lines.isEmpty())
						{
							int32 x = ParseOr<int32, int32>(lines[0], 0);
							int32 y = ParseOr<int32, int32>(lines[1], 0);
							m_outerPoints.emplace_back(x, y);
						}
					}
				}
			}
			else if (name == U"meshPointList")
			{
				for (auto value = child.firstChild(); value; value = value.nextSibling())
				{
					if (value.name() == U"value")
					{
						const auto& lines = value.text().split(U' ');
						if (not lines.isEmpty())
						{
							int32 x = ParseOr<int32, int32>(lines[0], 0);
							int32 y = ParseOr<int32, int32>(lines[1], 0);
							m_meshPoints.emplace_back(x, y);
						}
					}
				}
			}
			else if (name == U"meshTriList")
			{
				for (auto value = child.firstChild(); value; value = value.nextSibling())
				{
					if (value.name() == U"value")
					{
						const auto& lines = value.text().split(U' ');
						if (not lines.isEmpty())
						{
							int32 x = ParseOr<int32, int32>(lines[0], 0);
							int32 y = ParseOr<int32, int32>(lines[1], 0);
							int32 z = ParseOr<int32, int32>(lines[2], 0);
							m_meshTriangleIndexs.emplace_back(x, y, z);
						}
					}
				}
			}
		}
		return true;
	}

	StringView SSCell::getName() const
	{
		return m_name;
	}

	const Point& SSCell::getPos() const
	{
		return m_pos;
	}

	const Size& SSCell::getSize() const
	{
		return m_size;
	}

	const Vec2& SSCell::getPivot() const
	{
		return m_pivot;
	}

	double SSCell::getRotated() const
	{
		return m_rotated;
	}

	StringView SSCell::getOrgImageName() const
	{
		return m_orgImageName;
	}

	bool SSCell::isMesh() const
	{
		return m_isMesh;
	}

	const Array<Point>& SSCell::getInnerPoints() const
	{
		return m_innerPoints;
	}

	const Array<Point>& SSCell::getOuterPoints() const
	{
		return m_outerPoints;
	}

	const Array<Point>& SSCell::getMeshPoints() const
	{
		return m_meshPoints;
	}

	const Array<TriangleIndex32>& SSCell::getMeshTriangleIndexs() const
	{
		return m_meshTriangleIndexs;
	}
}
