
#include "SSAnimeSettings.hpp"

namespace sssdk
{
	SSAnimeSettings::SSAnimeSettings()
		: m_fps{ 30 }
		, m_frameCount{ 0 }
		, m_sortMode{ SortMode::prio }
		, m_canvasSize{ 800, 600 }
		, m_pivot{ 0.0, 0.0 }
		, m_startFrame{ 0 }
		, m_endFrame{ 0 }
	{
	}

	SSAnimeSettings::SSAnimeSettings(const XMLElement& element)
		: SSAnimeSettings()
	{
		load(element);
	}

	SSAnimeSettings::~SSAnimeSettings()
	{
	}

	bool SSAnimeSettings::load(const XMLElement& element)
	{
		if (not (element.name() == U"animeSettings"
			or element.name() == U"settings"))
		{
			return false;
		}
		for (auto child = element.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"fps")
			{
				m_fps = ParseOr<int32, int32>(child.text(), 0);
			}
			else if (name == U"frameCount")
			{
				m_frameCount = ParseOr<int32, int32>(child.text(), 0);
			}
			else if (name == U"sortMode")
			{
				const auto& text = child.text();
				if (text == U"prio")
				{
					m_sortMode = SortMode::prio;
				}
				else if (text == U"z")
				{
					m_sortMode = SortMode::z;
				}
			}
			else if (name == U"canvasSize")
			{
				const auto& lines = child.text().split(U' ');
				m_canvasSize.x = ParseOr<int32, int32>(lines[0], 0);
				m_canvasSize.y = ParseOr<int32, int32>(lines[1], 0);
			}
			else if (name == U"pivot")
			{
				const auto& lines = child.text().split(U' ');
				m_pivot.x = ParseOr<double, double>(lines[0], 0.0);
				m_pivot.y = ParseOr<double, double>(lines[1], 0.0);
			}
			else if (name == U"startFrame")
			{
				m_startFrame = ParseOr<int32, int32>(child.text(), 0);
			}
			else if (name == U"endFrame")
			{
				m_endFrame = ParseOr<int32, int32>(child.text(), 0);
			}
		}
		return true;
	}
}




SIV3D_NODISCARD_CXX20 int32 sssdk::SSAnimeSettings::getFps() const
{
    return m_fps;
}

SIV3D_NODISCARD_CXX20 int32 sssdk::SSAnimeSettings::getFrameCount() const
{
    return m_frameCount;
}

SIV3D_NODISCARD_CXX20 sssdk::SortMode sssdk::SSAnimeSettings::getSortMode() const
{
    return m_sortMode;
}

SIV3D_NODISCARD_CXX20 const Size& sssdk::SSAnimeSettings::getCanvasSize() const
{
	return m_canvasSize;
}

SIV3D_NODISCARD_CXX20 const Vec2& sssdk::SSAnimeSettings::getPivot() const
{
	return m_pivot;
}

SIV3D_NODISCARD_CXX20 int32 sssdk::SSAnimeSettings::getStartFrame() const
{
    return m_startFrame;
}

SIV3D_NODISCARD_CXX20 int32 sssdk::SSAnimeSettings::getEndFrame() const
{
    return m_endFrame;
}
