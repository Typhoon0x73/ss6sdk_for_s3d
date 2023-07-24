
#include "SSSettings.hpp"

namespace sssdk
{
	SSSettings::SSSettings()
		: m_animeBaseDirectory{ U"" }
	{
	}

	SSSettings::SSSettings(const XMLElement& element)
		: SSSettings()
	{
		load(element);
	}

	SSSettings::~SSSettings()
	{
	}

	bool SSSettings::load(const XMLElement& element)
	{
		if (element.name() != U"settings")
		{
			return false;
		}
		for (auto child = element.firstChild(); child; child = child.nextSibling())
		{
			if (child.name() == U"animeBaseDirectory")
			{
				m_animeBaseDirectory = child.text();
			}
			else if (child.name() == U"cellMapBaseDirectory")
			{
				m_cellmapBaseDirectory = child.text();
			}
			else if (child.name() == U"imageBaseDirectory")
			{
				m_imageBaseDirectory = child.text();
			}
			else if (child.name() == U"effectBaseDirectory")
			{
				m_effectBaseDirectory = child.text();
			}
		}
		return true;
	}

	const FilePath& SSSettings::getAnimeBaseDirectory() const
	{
		return m_animeBaseDirectory;
	}

	const FilePath& SSSettings::getCellmapBaseDirectory() const
	{
		return m_cellmapBaseDirectory;
	}

	const FilePath& SSSettings::getImageBaseDirectory() const
	{
		return m_imageBaseDirectory;
	}

	const FilePath& SSSettings::getEffectBaseDirectory() const
	{
		return m_effectBaseDirectory;
	}
}
