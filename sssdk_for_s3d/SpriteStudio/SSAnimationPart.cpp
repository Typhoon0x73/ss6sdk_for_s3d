
#include "SSAnimationPart.hpp"

namespace sssdk
{
	SSAnimationPart::SSAnimationPart()
		: m_name{ U"" }
		, m_attributes{}
	{
	}

	SSAnimationPart::SSAnimationPart(const XMLElement& element)
		: SSAnimationPart()
	{
		load(element);
	}

	SSAnimationPart::~SSAnimationPart()
	{
	}

	bool SSAnimationPart::load(const XMLElement& element)
	{
		if (element.name() != U"partAnime")
		{
			return false;
		}
		for (auto child = element.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"partName")
			{
				m_name = child.text();
			}
			else if (name == U"attributes")
			{
				for (auto attribute = child.firstChild(); attribute; attribute = attribute.nextSibling())
				{
					m_attributes.emplace_back(attribute);
				}
			}
		}
		return true;
	}

	StringView SSAnimationPart::getName() const
	{
		return m_name;
	}

	const Array<SSAttribute>& SSAnimationPart::getAttributes() const
	{
		return m_attributes;
	}
}
