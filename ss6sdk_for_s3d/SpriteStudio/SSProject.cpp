
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
		m_createEditorParam = createEditorParam;
		if (isCreateEditorParam())
		{
			m_pEditorParam.reset(new EditorParam());
		}

		static const HashTable<String, void (SSProject::*)(const XMLElement&)> PARSE_TABLE = {
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_name         ], &SSProject::parseFileName      },
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_settings     ], &SSProject::parseSettings      },
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_animeSettings], &SSProject::parseAnimeSettings },
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
		return (m_createEditorParam == CreateEditorParam::Create);
	}

	void SSProject::parseFileName(const XMLElement& element)
	{
		m_projectName = element.text();
	}

	void SSProject::parseExportPath(const XMLElement& element)
	{
		m_pEditorParam->m_projectExportPath = element.text();
	}

	void SSProject::parseSettings(const XMLElement& element)
	{
		m_projectSettings.load(element, isCreateEditorParam());
	}

	void SSProject::parseAnimeSettings(const XMLElement& element)
	{
		m_animeSettings.load(element, isCreateEditorParam());
	}
}
