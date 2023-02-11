
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

		static const HashTable<String, void (SSProject::*)(const XMLElement&)> PARSE_TABLE = {
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_name    ], &SSProject::parseFileName },
			{ SS_PROJECT_TAG_STRINGS[SSProjectTag_settings], &SSProject::parseSettings },
		};

		for (auto element = reader.firstChild(); element; element = element.nextSibling())
		{
			for(const auto& table : PARSE_TABLE)
			if (table.first == element.name())
			{
				(this->*(table.second))(element);
				break;
			}
		}
		return true;
	}

	void SSProject::parseFileName(const XMLElement& element)
	{
		m_projectName = element.text();
	}

	void SSProject::parseSettings(const XMLElement& element)
	{
		m_projectSettings.load(element, (m_createEditorParam == CreateEditorParam::Create));
	}
}
