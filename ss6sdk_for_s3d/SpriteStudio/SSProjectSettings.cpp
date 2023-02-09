
# include "SSProjectSettings.hpp"

namespace
{
	bool ParseIntToBool(StringView s)
	{
		int32 parse = ParseInt<int32>(s);
		return (parse > 0);
	}
}

namespace s3d
{

	bool SSProjectSettings::load(const XMLElement& settings, bool createEditorParam)
	{
		m_pEditorParam.reset();
		if (createEditorParam)
		{
			m_pEditorParam.reset(new SSProjectSettings::EditorParam());
		}
		static const HashTable<String, void(SSProjectSettings::*)(StringView)> SETTINGS_PARSE_TABLE = {
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingTag_animeBaseDirectory      ], &SSProjectSettings::parseAnimeBaseDirectory       },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingTag_cellMapBaseDirectory    ], &SSProjectSettings::parseCellMapBaseDirectory     },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingTag_imageBaseDirectory      ], &SSProjectSettings::parseImageBaseDirectory       },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingTag_effectBaseDirectory     ], &SSProjectSettings::parseEffectBaseDirectory      },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingTag_exportBaseDirectory     ], &SSProjectSettings::parseExportBaseDirectory      },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingTag_queryExportBaseDirectory], &SSProjectSettings::parseQueryExportBaseDirectory },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingTag_wrapMode                ], &SSProjectSettings::parseWrapMode                 },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingTag_filterMode              ], &SSProjectSettings::parseFilterMode               },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingTag_vertexAnimeFloat        ], &SSProjectSettings::parseVertexAnimeFloat         },
		};
		for (auto element = settings.firstChild(); element; element = element.nextSibling())
		{
			for (const auto& table : SETTINGS_PARSE_TABLE)
			{
				if (table.first == element.name())
				{
					(this->*(table.second))(element.text());
					break;
				}
			}

			// エディタ用パラメータの解析
			if (m_pEditorParam && createEditorParam)
			{

			}
		}
		return true;
	}

	void SSProjectSettings::parseAnimeBaseDirectory(StringView text)
	{
		m_animeBaseDirectory = text;
	}

	void SSProjectSettings::parseCellMapBaseDirectory(StringView text)
	{
		m_cellMapBaseDirectory = text;
	}

	void SSProjectSettings::parseImageBaseDirectory(StringView text)
	{
		m_imageBaseDirectory = text;
	}

	void SSProjectSettings::parseEffectBaseDirectory(StringView text)
	{
		m_effectBaseDirectory = text;
	}

	void SSProjectSettings::parseExportBaseDirectory(StringView text)
	{
		m_exportBaseDirectory = text;
	}

	void SSProjectSettings::parseQueryExportBaseDirectory(StringView text)
	{
		m_queryExportBaseDirectory = ParseIntToBool(text);
	}

	void SSProjectSettings::parseWrapMode(StringView text)
	{
		auto wrapMode = SS_TEXTURE_WRAP_MODE_TABLE.find(text);
		if (wrapMode != SS_TEXTURE_WRAP_MODE_TABLE.end())
		{
			m_wrapMode = wrapMode->second;
		}
		else
		{
			m_wrapMode = TextureAddressMode::Clamp;
		}
	}

	void SSProjectSettings::parseFilterMode(StringView text)
	{
		auto filterMode = SS_TEXTURE_FILTER_MODE_TABLE.find(text);
		if (filterMode != SS_TEXTURE_FILTER_MODE_TABLE.end())
		{
			m_filterMode = filterMode->second;
		}
		else
		{
			m_filterMode = TextureFilter::Linear;
		}
	}

	void SSProjectSettings::parseVertexAnimeFloat(StringView text)
	{
		m_vertexAnimeFloat = ParseIntToBool(text);
	}
}
