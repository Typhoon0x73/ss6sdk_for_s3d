
# include "SSAnimePack.hpp"
# include "SSCommon.hpp"

namespace s3d
{
	SSAnimePack::SSAnimePack(FilePathView ssae, bool createEditorParam)
	{
	}

	bool SSAnimePack::load(FilePathView ssae, bool createEditorParam)
	{
		XMLReader reader{ ssae };
		if (not reader.isOpen())
		{
			return false;
		}
		m_isCreateEditorParam = createEditorParam;
		static const HashTable<String, void(SSAnimePack::*)(const XMLElement&)> PARSE_TABLE = {
			{ SS_ANIME_PACK_TAG_STRINGS[SSAnimePackTag_settings    ], &SSAnimePack::parseSettings     },
			{ SS_ANIME_PACK_TAG_STRINGS[SSAnimePackTag_name        ], &SSAnimePack::parseName         },
			{ SS_ANIME_PACK_TAG_STRINGS[SSAnimePackTag_exportPath  ], &SSAnimePack::parseExportPath   },
			{ SS_ANIME_PACK_TAG_STRINGS[SSAnimePackTag_Model       ], &SSAnimePack::parseModel        },
			{ SS_ANIME_PACK_TAG_STRINGS[SSAnimePackTag_cellmapNames], &SSAnimePack::parseCellmapNames },
			{ SS_ANIME_PACK_TAG_STRINGS[SSAnimePackTag_animeList   ], &SSAnimePack::parseAnimeList    },
		};
		for (auto element = reader.firstChild(); element; element = element.nextSibling())
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

	void SSAnimePack::parseSettings(const XMLElement& element)
	{
		m_settings.load(element, m_isCreateEditorParam);
	}

	void SSAnimePack::parseName(const XMLElement& element)
	{
		m_name = element.text();
	}

	void SSAnimePack::parseExportPath(const XMLElement& element)
	{
		m_exportPath = element.text();
	}

	void SSAnimePack::parseModel(const XMLElement& element)
	{
		//TODO : create model load
	}

	void SSAnimePack::parseCellmapNames(const XMLElement& element)
	{
		for (auto e = element.firstChild(); e; e = e.nextSibling())
		{
			if (e.name() == U"value")
			{
				m_cellmapNames.emplace_back(e.text());
			}
		}
	}

	void SSAnimePack::parseAnimeList(const XMLElement& element)
	{
		for (auto e = element.firstChild(); e; e = e.nextSibling())
		{
			if (e.name() == U"anime")
			{
				//TODO : create anime load
				//m_animeList.emplace_back(SSAnime{ e });
			}
		}
	}
}
