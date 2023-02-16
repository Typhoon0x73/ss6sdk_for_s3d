
# include "SSAnimeSettings.hpp"
# include "SSCommon.hpp"
# include "SSParseUtilities.hpp"

namespace s3d
{
	SSAnimeSettings::SSAnimeSettings(const XMLElement& animeSettings, bool createEditorParam)
	{
		load(animeSettings, createEditorParam);
	}

	bool SSAnimeSettings::load(const XMLElement& animeSettings, bool createEditorParam)
	{
		m_isCreateEditorParam = createEditorParam;
		static const HashTable<String, void(SSAnimeSettings::*)(const XMLElement&)> PARSE_TABLE = {
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_fps        ], SSAnimeSettings::parseFps                },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_frameCount ], SSAnimeSettings::parseFrameCount         },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_sortMode   ], SSAnimeSettings::parseSortMode           },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_canvasSize ], SSAnimeSettings::parseCanvasSize         },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_pivot      ], SSAnimeSettings::parsePivot              },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_bgColor    ], SSAnimeSettings::parseBackGroundColor    },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_gridSize   ], SSAnimeSettings::parseGridSize           },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_gridColor  ], SSAnimeSettings::parseGridColor          },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_ik_depth   ], SSAnimeSettings::parseIKDepth            },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_startFrame ], SSAnimeSettings::parseStartFrame         },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_endFrame   ], SSAnimeSettings::parseEndFrame           },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_bgSettings ], SSAnimeSettings::parseBackgroundSettings },
			{ SS_ANIME_SETTINGS_TAG_STRINGS[SSAnimeSettingsTag_outStartNum], SSAnimeSettings::parseOutStartNum        },
		};
		for (auto element = animeSettings.firstChild(); element; element = element.nextSibling())
		{
			for (const auto& table : PARSE_TABLE)
			{
				if (table.first == element.name())
				{
					(this->*(table.second))(element);
					break;
				}
			}
		}
		return true;
	}

	void SSAnimeSettings::parseFps(const XMLElement& element)
	{
		m_fps = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSAnimeSettings::parseFrameCount(const XMLElement& element)
	{
		m_frameCount = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSAnimeSettings::parseSortMode(const XMLElement& element)
	{
		if (element.text() == U"prio")
		{
			m_sortMode = SortMode::prio;
		}
		else if (element.text() == U"posz")
		{
			m_sortMode = SortMode::posz;
		}
	}

	void SSAnimeSettings::parseCanvasSize(const XMLElement& element)
	{
		m_canvasSize = SSParseUtilities::parseSize(element.text());
	}

	void SSAnimeSettings::parsePivot(const XMLElement& element)
	{
		m_pivot = SSParseUtilities::parseFloat2(element.text());
	}

	void SSAnimeSettings::parseBackGroundColor(const XMLElement& element)
	{
		m_bgColor = SSParseUtilities::parseColorF(element.text());
	}

	void SSAnimeSettings::parseGridSize(const XMLElement& element)
	{
		m_gridSize = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSAnimeSettings::parseGridColor(const XMLElement& element)
	{
		m_gridColor = SSParseUtilities::parseColorF(element.text());
	}

	void SSAnimeSettings::parseIKDepth(const XMLElement& element)
	{
		m_ik_depth = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSAnimeSettings::parseStartFrame(const XMLElement& element)
	{
		m_startFrame = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSAnimeSettings::parseEndFrame(const XMLElement& element)
	{
		m_endFrame = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSAnimeSettings::parseBackgroundSettings(const XMLElement& element)
	{
		m_bgSettings.load(element, m_isCreateEditorParam);
	}

	void SSAnimeSettings::parseOutStartNum(const XMLElement& element)
	{
		m_outStartNum = ParseOr<int32, int32>(element.text(), 0);
	}
}
