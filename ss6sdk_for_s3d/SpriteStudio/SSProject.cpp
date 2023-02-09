
# include "SSProject.hpp"

namespace s3d
{

	SSProject::SSProject(FilePathView path, bool createEditorParam)
		: SSProject()
	{
		load(path, createEditorParam);
	}

	bool SSProject::load(FilePathView path, bool createEditorParam)
	{
		XMLReader reader{ path };
		if (not reader.isOpen() or reader.isNull())
		{
			return false;
		}
		m_projectDirectory = FileSystem::RelativePath(FileSystem::ParentPath(path), FileSystem::ModulePath());
		m_projectFileName  = FileSystem::FileName(path);
		for (auto element = reader.firstChild(); element; element = element.nextSibling())
		{
			if (element.name().compare(SS_PROJECT_TAG_STRINGS[SSProjectTag_name]) == 0)
			{
				m_projectName = element.text();
			}
			else if (element.name().compare(SS_PROJECT_TAG_STRINGS[SSProjectTag_settings]) == 0)
			{
				m_projectSettings.load(element, createEditorParam);
			}
		}
		return true;
	}
}
