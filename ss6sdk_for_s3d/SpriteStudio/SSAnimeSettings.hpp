
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

		explicit SSAnimeSettings(const XMLElement& animeSettings, bool createEditorParam);

		bool load(const XMLElement& animeSettings, bool createEditorParam);

	private:

		// -------------------------------------------------
		// 解析用関数
		// -------------------------------------------------
		void parseFps               (const XMLElement& element);
		void parseFrameCount        (const XMLElement& element);
		void parseSortMode          (const XMLElement& element);
		void parseCanvasSize        (const XMLElement& element);
		void parsePivot             (const XMLElement& element);
		void parseBackGroundColor   (const XMLElement& element);
		void parseGridSize          (const XMLElement& element);
		void parseGridColor         (const XMLElement& element);
		void parseIKDepth           (const XMLElement& element);
		void parseStartFrame        (const XMLElement& element);
		void parseEndFrame          (const XMLElement& element);
		void parseBackgroundSettings(const XMLElement& element);
		void parseOutStartNum       (const XMLElement& element);

		bool m_isCreateEditorParam{ false };

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
