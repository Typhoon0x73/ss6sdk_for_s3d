
#include "SSAnimationPack.hpp"

namespace sssdk
{
	SSAnimationPack::SSAnimationPack()
		: m_name{ U"" }
		, m_version{ U"" }
		, m_settings{}
		, m_model{}
		, m_cellmapNames{}
	{
	}

	SSAnimationPack::SSAnimationPack(FilePathView path)
		: SSAnimationPack()
	{
		load(path);
	}

	SSAnimationPack::~SSAnimationPack()
	{
	}

	bool SSAnimationPack::load(FilePathView path)
	{
		XMLReader reader{ path };
		if (not reader)
		{
			return false;
		}
		for (const auto& attribute : reader.attributes())
		{
			if (attribute.first == U"version")
			{
				m_version = attribute.second;
			}
		}
		for (auto child = reader.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"settings")
			{
				if (not m_settings.load(child))
				{
					return false;
				}
			}
			else if (name == U"name")
			{
				m_name = child.text();
			}
			else if (name == U"Model")
			{
				if (not m_model.load(child))
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
			else if (name == U"animeList")
			{
				for (auto anime = child.firstChild(); anime; anime = anime.nextSibling())
				{
					if (anime.name() == U"anime")
					{
						m_animations.emplace_back(anime);
					}
				}
			}
		}
		return true;
	}

	StringView SSAnimationPack::getVersion() const
	{
		return m_version;
	}

	const SSSettings& SSAnimationPack::getSettings() const
	{
		return m_settings;
	}

	StringView SSAnimationPack::getName() const
	{
		return m_name;
	}

	const SSModel& SSAnimationPack::getModel() const
	{
		return m_model;
	}

	const Array<String>& SSAnimationPack::getCellmapNames() const
	{
		return m_cellmapNames;
	}

	const Array<SSAnimation>& SSAnimationPack::getAnimations() const
	{
		return m_animations;
	}

	const SSAnimation* const SSAnimationPack::getAnimation(int32 index) const
	{
		if (0 > index or m_animations.size() <= index)
		{
			return nullptr;
		}
		return &m_animations[index];
	}

	const SSAnimation* const SSAnimationPack::getAnimation(StringView anim) const
	{
		for (const auto& it : m_animations)
		{
			if (it.getName() == anim)
			{
				return &it;
			}
		}
		return nullptr;
	}

	const SSAnimation* const SSAnimationPack::getSetupAnimation() const
	{
		for (const auto& it : m_animations)
		{
			if (it.isSetup())
			{
				return &it;
			}
		}
		return nullptr;
	}
}
