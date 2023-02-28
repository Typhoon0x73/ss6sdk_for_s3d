﻿
# include "SSProject.hpp"

namespace s3d
{

	SSProject::SSProject(FilePathView path, CreateEditorParam createEditorParam)
		: SSProject()
	{
		load(path, createEditorParam);
	}

	bool SSProject::load(FilePathView path, CreateEditorParam createEditorParam)
	{
		XMLReader reader{ path };
		if (not reader.isOpen() or reader.isNull())
		{
			return false;
		}
		m_projectDirectory  = FileSystem::RelativePath(FileSystem::ParentPath(path), FileSystem::ModulePath());
		m_projectFileName   = FileSystem::FileName(path);
		if (createEditorParam == CreateEditorParam::Create)
		{
			m_pEditorParam.reset(new EditorParam());
		}

		static const HashTable<String, void (SSProject::*)(const XMLElement&)> PARSE_TABLE = {
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_name            ], &SSProject::parseFileName         },
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_settings        ], &SSProject::parseSettings         },
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_animeSettings   ], &SSProject::parseAnimeSettings    },
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_texPackSettings ], &SSProject::parseTexPackSettings  },
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_cellmapNames    ], &SSProject::parseCellmapNames     },
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_animepackNames  ], &SSProject::parseAnimepackNames   },
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_effectFileNames ], &SSProject::parseEffectFileNames  },
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_ExternalTextures], &SSProject::parseExternalTextures },
		};
		static const HashTable<String, void (SSProject::*)(const XMLElement&)> EDITOR_PARSE_TABLE = {
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_exportPath], &SSProject::parseExportPath },
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
		if (m_pEditorParam && isCreateEditorParam())
		{
			for (auto element = reader.firstChild(); element; element = element.nextSibling())
			{
				for (const auto& table : EDITOR_PARSE_TABLE)
				{
					if (table.first == element.name())
					{
						(this->*(table.second))(element);
						break;
					}
				}
			}
		}
		return true;
	}

	bool SSProject::isCreateEditorParam() const
	{
		return (m_pEditorParam.get() != nullptr);
	}

	void SSProject::parseFileName(const XMLElement& element)
	{
		m_name = element.text();
	}

	void SSProject::parseExportPath(const XMLElement& element)
	{
		m_pEditorParam->m_exportPath = element.text();
	}

	void SSProject::parseSettings(const XMLElement& element)
	{
		m_settings.load(element, isCreateEditorParam());
	}

	void SSProject::parseAnimeSettings(const XMLElement& element)
	{
		m_animeSettings.load(element, isCreateEditorParam());
	}

	void SSProject::parseTexPackSettings(const XMLElement& element)
	{
		m_texPackSettings.load(element, isCreateEditorParam());
	}

	void SSProject::parseCellmapNames(const XMLElement& element)
	{
		for (auto e = element.firstChild(); e; e = e.nextSibling())
		{
			if (U"value" == e.name())
			{
				m_cellmapNames.emplace_back(e.text());
			}
		}
	}

	void SSProject::parseAnimepackNames(const XMLElement& element)
	{
		for (auto e = element.firstChild(); e; e = e.nextSibling())
		{
			if (U"value" == e.name())
			{
				m_animepackNames.emplace_back(e.text());
			}
		}
	}

	void SSProject::parseEffectFileNames(const XMLElement& element)
	{
		for (auto e = element.firstChild(); e; e = e.nextSibling())
		{
			if (U"value" == e.name())
			{
				m_effectFileNames.emplace_back(e.text());
			}
		}
	}

	void SSProject::parseExternalTextures(const XMLElement& element)
	{
		m_ExternalTextures.load(element, isCreateEditorParam());
	}
}
