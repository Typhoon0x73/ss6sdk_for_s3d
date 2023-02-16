
# include "SSProjectSettings.hpp"
# include "SSParseUtilities.hpp"

namespace s3d
{

	bool SSProjectSettings::load(const XMLElement& settings, bool createEditorParam)
	{
		m_pEditorParam.reset();
		if (createEditorParam)
		{
			m_pEditorParam.reset(new SSProjectSettings::EditorParam());
		}
		static const HashTable<String, void(SSProjectSettings::*)(const XMLElement&)> SETTINGS_PARSE_TABLE = {
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingsTag_animeBaseDirectory      ], &SSProjectSettings::parseAnimeBaseDirectory       },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingsTag_cellMapBaseDirectory    ], &SSProjectSettings::parseCellMapBaseDirectory     },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingsTag_imageBaseDirectory      ], &SSProjectSettings::parseImageBaseDirectory       },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingsTag_effectBaseDirectory     ], &SSProjectSettings::parseEffectBaseDirectory      },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingsTag_exportBaseDirectory     ], &SSProjectSettings::parseExportBaseDirectory      },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingsTag_queryExportBaseDirectory], &SSProjectSettings::parseQueryExportBaseDirectory },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingsTag_wrapMode                ], &SSProjectSettings::parseWrapMode                 },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingsTag_filterMode              ], &SSProjectSettings::parseFilterMode               },
			{ SS_PROJECT_SETTINGS_TAG_STRINGS[SSProjectSettingsTag_vertexAnimeFloat        ], &SSProjectSettings::parseVertexAnimeFloat         },
		};
		for (auto element = settings.firstChild(); element; element = element.nextSibling())
		{
			for (const auto& table : SETTINGS_PARSE_TABLE)
			{
				if (table.first == element.name())
				{
					(this->*(table.second))(element);
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

	void SSProjectSettings::parseAnimeBaseDirectory(const XMLElement& element)
	{
		m_animeBaseDirectory = element.text();
	}

	void SSProjectSettings::parseCellMapBaseDirectory(const XMLElement& element)
	{
		m_cellMapBaseDirectory = element.text();
	}

	void SSProjectSettings::parseImageBaseDirectory(const XMLElement& element)
	{
		m_imageBaseDirectory = element.text();
	}

	void SSProjectSettings::parseEffectBaseDirectory(const XMLElement& element)
	{
		m_effectBaseDirectory = element.text();
	}

	void SSProjectSettings::parseExportBaseDirectory(const XMLElement& element)
	{
		m_exportBaseDirectory = element.text();
	}

	void SSProjectSettings::parseQueryExportBaseDirectory(const XMLElement& element)
	{
		m_queryExportBaseDirectory = SSParseUtilities::parseIntToBool(element.text());
	}

	void SSProjectSettings::parseWrapMode(const XMLElement& element)
	{
		auto wrapMode = SS_TEXTURE_WRAP_MODE_TABLE.find(element.text());
		if (wrapMode != SS_TEXTURE_WRAP_MODE_TABLE.end())
		{
			m_wrapMode = wrapMode->second;
		}
		else
		{
			m_wrapMode = TextureAddressMode::Clamp;
		}
	}

	void SSProjectSettings::parseFilterMode(const XMLElement& element)
	{
		auto filterMode = SS_TEXTURE_FILTER_MODE_TABLE.find(element.text());
		if (filterMode != SS_TEXTURE_FILTER_MODE_TABLE.end())
		{
			m_filterMode = filterMode->second;
		}
		else
		{
			m_filterMode = TextureFilter::Linear;
		}
	}

	void SSProjectSettings::parseVertexAnimeFloat(const XMLElement& element)
	{
		m_vertexAnimeFloat = SSParseUtilities::parseIntToBool(element.text());
	}
}
