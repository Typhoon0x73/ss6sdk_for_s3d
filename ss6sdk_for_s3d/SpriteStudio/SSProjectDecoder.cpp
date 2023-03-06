
# include "SSProjectDecoder.hpp"

namespace s3d
{
	SSProjectDecoder::SSProjectDecoder(FilePathView sspj, SSCreateEditorParam createEditorParam)
	{
		load(sspj, createEditorParam);
	}

	bool SSProjectDecoder::load(FilePathView sspj, SSCreateEditorParam createEditorParam)
	{
		FilePath currentPath = FileSystem::CurrentDirectory();
		m_projectDirectory   = FileSystem::RelativePath(FileSystem::ParentPath(sspj));
		m_projectFileName    = FileSystem::FileName(sspj);
		FileSystem::ChangeCurrentDirectory(m_projectDirectory);

		bool isCreateEditorParam = (createEditorParam == SSCreateEditorParam::Create);
		if (not m_project.load(m_projectFileName, isCreateEditorParam))
		{
			return false;
		}

		// cellmaps
		const auto& cellmapNames = m_project.getCellMapNames();
		for (const auto& cellmapName : cellmapNames)
		{
			auto cellmap = std::make_unique<SSCellMap>(cellmapName, isCreateEditorParam);
			m_cellmaps.emplace_back(std::move(cellmap));
		}

		for (const auto& cellmap : m_cellmaps)
		{
			auto exportPath = cellmap->getExportPath();
			if (exportPath.isEmpty())
			{
				m_textureMap[cellmap->getName()] = Texture(cellmap->getImagePath());
			}
			else
			{
				FilePath oldPath = FileSystem::CurrentDirectory();
				FileSystem::ChangeCurrentDirectory(exportPath);
				m_textureMap[cellmap->getName()] = Texture(cellmap->getImagePath());
				FileSystem::ChangeCurrentDirectory(oldPath);
			}
		}

		// animePacks
		const auto& animePackNames = m_project.getAnimePackNames();
		for (const auto& animePackName : animePackNames)
		{
			auto animePack = std::make_unique<SSAnimePack>(animePackName, isCreateEditorParam);
			m_animePacks.emplace_back(std::move(animePack));
		}

		FileSystem::ChangeCurrentDirectory(currentPath);
		return true;
	}
}
