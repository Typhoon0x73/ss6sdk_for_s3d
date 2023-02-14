
# pragma once
# include <Siv3D.hpp>
# include "SSBackgroundSettings.hpp"

namespace s3d
{

	class SSAnimeSettings
	{
	public:

		enum class SortMode
		{
			prio,
			posz,
		};

		SSAnimeSettings() = default;

		~SSAnimeSettings() = default;

	private:

		int32                m_fps        { 0 };
		int32                m_frameCount { 0 };
		SortMode             m_sortMode   {};
		Size                 m_canvasSize { 0, 0 };
		Float2               m_pivot      { 0.0f, 0.0f };
		ColorF               m_bgColor    {};
		int32                m_gridSize   { 0 };
		ColorF               m_gridColor  {};
		int32                m_ik_depth   { 0 };
		int32                m_startFrame { 0 };
		int32                m_endFrame   { 0 };
		SSBackgroundSettings m_bgSettings {};
		int32                m_outStartNum{ 0 };

	};
}
