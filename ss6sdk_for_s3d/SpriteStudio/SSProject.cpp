
#include "SSProject.hpp"

namespace sssdk
{
	SSProject::SSProject()
		: m_version{ U"" }
		, m_ssEditorVersion{ U"" }
		, m_projectDirectiory{ U"" }
		, m_name{ U"" }
		, m_settings{}
		, m_animeSettings{}
		, m_cellmapNames{}
		, m_animepackNames{}
		, m_effectFileNames{}
		, m_cellmaps{}
		, m_animPacks{}
	{
	}

	SSProject::SSProject(FilePathView path)
		: SSProject()
	{
		load(path);
	}

	SSProject::~SSProject()
	{
	}

	bool SSProject::load(FilePathView path)
	{
		XMLReader reader{ path };
		if (not reader)
		{
			return false;
		}
		m_projectDirectiory = FileSystem::ParentPath(path);

		for (const auto& attribute : reader.attributes())
		{
			if (attribute.first == U"version")
			{
				m_version = attribute.second;
			}
			else if (attribute.first == U"editorVersion")
			{
				m_ssEditorVersion = attribute.second;
			}
		}
		for (auto child = reader.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"name")
			{
				m_name = child.text();
			}
			else if (name == U"settings")
			{
				if (not m_settings.load(child))
				{
					return false;
				}
			}
			else if (name == U"animeSettings")
			{
				if (not m_animeSettings.load(child))
				{
					return false;
				}
			}
			else if (name == U"cellmapNames")
			{
				for (auto value = child.firstChild(); value; value = value.nextSibling())
				{
					if (value.name() == U"value")
					{
						m_cellmapNames.emplace_back(value.text());
					}
				}
			}
			else if (name == U"animepackNames")
			{
				for (auto value = child.firstChild(); value; value = value.nextSibling())
				{
					if (value.name() == U"value")
					{
						m_animepackNames.emplace_back(value.text());
					}
				}
			}
			else if (name == U"effectFileNames")
			{
				for (auto value = child.firstChild(); value; value = value.nextSibling())
				{
					if (value.name() == U"value")
					{
						m_effectFileNames.emplace_back(value.text());
					}
				}
			}
			else if (name == U"ExternalTextures")
			{
				// TODO
			}
		}

		const FilePath& ssceDir = getSSCEFileDirectory();
		for (const auto& cellmapName : m_cellmapNames)
		{
			const FilePath& cellPath = FileSystem::PathAppend(ssceDir, cellmapName);
			m_cellmaps.emplace_back(this, cellPath);
		}

		const FilePath& ssaeDir = getSSAEFileDirectory();
		for (const auto& animepackName : m_animepackNames)
		{
			const FilePath& animPackPath = FileSystem::PathAppend(ssaeDir, animepackName);
			m_animPacks.emplace_back(animPackPath);
		}
		return true;
	}

	StringView SSProject::getVersion() const
	{
		return m_version;
	}

	StringView SSProject::getSpriteStudioEditorVersion() const
	{
		return m_ssEditorVersion;
	}

	const FilePath& SSProject::getProjectFileDirectory() const
	{
		return m_projectDirectiory;
	}

	FilePath SSProject::getImageFileDirectory() const
	{
		return FileSystem::PathAppend(m_projectDirectiory, m_settings.getImageBaseDirectory());
	}

	FilePath SSProject::getSSCEFileDirectory() const
	{
		return FileSystem::PathAppend(m_projectDirectiory, m_settings.getCellmapBaseDirectory());
	}

	FilePath SSProject::getSSAEFileDirectory() const
	{
		return FileSystem::PathAppend(m_projectDirectiory, m_settings.getAnimeBaseDirectory());
	}

	FilePath SSProject::getSSEEFileDirectory() const
	{
		return FileSystem::PathAppend(m_projectDirectiory, m_settings.getEffectBaseDirectory());
	}

	StringView SSProject::getName() const
	{
		return m_name;
	}

	const SSSettings& SSProject::getSettings() const
	{
		return m_settings;
	}

	const SSAnimeSettings& SSProject::getAnimeSettings() const
	{
		return m_animeSettings;
	}

	const Array<String>& SSProject::getCellmapNames() const
	{
		return m_cellmapNames;
	}

	const Array<String>& SSProject::getAnimepackNames() const
	{
		return m_animepackNames;
	}

	const Array<String>& SSProject::getEffectFileNames() const
	{
		return m_effectFileNames;
	}

	const SSCellmap* const SSProject::getCellmap(StringView ssce) const
	{
		for (const auto& it : m_cellmaps)
		{
			if (it.getName() == ssce)
			{
				return &it;
			}
		}
		return nullptr;
	}

	const SSCellmap* const SSProject::getCellmap(uint64 index) const
	{
		if (index < 0 or index >= m_cellmaps.size())
		{
			return nullptr;
		}
		return &m_cellmaps[index];
	}

	const SSAnimationPack* const SSProject::getAnimPack(StringView ssae) const
	{
		for (const auto& it : m_animPacks)
		{
			if (it.getName() == ssae)
			{
				return &it;
			}
		}
		return nullptr;
	}

	const SSAnimation* const SSProject::getAnimation(StringView ssae, StringView anim) const
	{
		const auto* animPack = getAnimPack(ssae);
		if (animPack == nullptr)
		{
			return nullptr;
		}
		return animPack->getAnimation(anim);
	}
}
