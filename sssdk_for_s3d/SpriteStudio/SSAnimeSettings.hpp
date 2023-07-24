#pragma once
#ifndef SS_ANIME_SETTINGS_H_
#define SS_ANIME_SETTINGS_H_

#include <Siv3D.hpp>
#include "SSDefine.hpp"

namespace sssdk
{
	class SSAnimeSettings
	{
	public:

		explicit SSAnimeSettings();
		explicit SSAnimeSettings(const XMLElement& element);
		virtual ~SSAnimeSettings();

		bool load(const XMLElement& element);

		SIV3D_NODISCARD_CXX20 int32 getFps() const;
		SIV3D_NODISCARD_CXX20 int32 getFrameCount() const;

		SIV3D_NODISCARD_CXX20 SortMode getSortMode() const;

		SIV3D_NODISCARD_CXX20 const Size& getCanvasSize() const;
		SIV3D_NODISCARD_CXX20 const Vec2& getPivot() const;

		SIV3D_NODISCARD_CXX20 int32 getStartFrame() const;
		SIV3D_NODISCARD_CXX20 int32 getEndFrame() const;

	private:

		int32 m_fps;
		int32 m_frameCount;

		SortMode m_sortMode;

		Size m_canvasSize;
		Vec2 m_pivot;

		int32 m_startFrame;
		int32 m_endFrame;
	};
}

#endif // !SS_ANIME_SETTINGS_H_
