
#include "SSAnimation.hpp"

namespace sssdk
{
	SSAnimation::SSAnimation()
		: m_name{ U"" }
		, m_isOverrideSettings{ false }
		, m_animeSettings{}
		, m_isSetup{ false }
		, m_labels{}
		, m_animParts{}
	{
	}

	SSAnimation::SSAnimation(const XMLElement& element)
		: SSAnimation()
	{
		load(element);
	}

	SSAnimation::~SSAnimation()
	{
	}

	bool SSAnimation::load(const XMLElement& element)
	{
		if (element.name() != U"anime")
		{
			return false;
		}
		for (auto child = element.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"name")
			{
				m_name = child.text();
			}
			else if (name == U"overrideSettings")
			{
				m_isOverrideSettings = (ParseOr<int32, int32>(child.text(), 0) == 1);
			}
			else if (name == U"settings")
			{
				m_animeSettings.load(child);
			}
			else if (name == U"labels")
			{
				for (auto label = child.firstChild(); label; label = label.nextSibling())
				{
					SSLabel labelData;
					for (auto value = label.firstChild(); value; value = value.nextSibling())
					{
						if (value.name() == U"name")
						{
							labelData.name = value.text();
						}
						else if (value.name() == U"time")
						{
							labelData.time = ParseOr<int32, int32>(value.text(), 0);
						}
					}
					m_labels.emplace_back(labelData);
				}
			}
			else if (name == U"isSetup")
			{
				m_isSetup = (ParseOr<int32, int32>(child.text(), 0) == 1);
			}
			else if (name == U"partAnimes")
			{
				for (auto part = child.firstChild(); part; part = part.nextSibling())
				{
					m_animParts.emplace_back(part);
				}
			}
		}
		return true;
	}

	StringView SSAnimation::getName() const
	{
		return m_name;
	}

	bool SSAnimation::isOverrideSettings() const
	{
		return m_isOverrideSettings;
	}

	const SSAnimeSettings& SSAnimation::getAnimeSettings() const
	{
		return m_animeSettings;
	}

	bool SSAnimation::isSetup() const
	{
		return m_isSetup;
	}

	const Array<SSLabel>& SSAnimation::getLabels() const
	{
		return m_labels;
	}

	const Array<SSAnimationPart>& SSAnimation::getAnimParts() const
	{
		return m_animParts;
	}

	const SSAnimationPart* const SSAnimation::getAnimPart(StringView name) const
	{
		for (const auto& animPart : m_animParts)
		{
			if (animPart.getName() == name)
			{
				return &animPart;
			}
		}
		return nullptr;
	}
}
