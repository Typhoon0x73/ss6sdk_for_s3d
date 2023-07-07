
#include "SSAttribute.hpp"

namespace sssdk
{
	SSAttribute::SSAttribute()
		: m_tag{ ATTRIBUTE_KIND_NUM }
		, m_keyValues{}
	{
	}

	SSAttribute::SSAttribute(const XMLElement& element)
		: SSAttribute()
	{
		load(element);
	}

	SSAttribute::~SSAttribute()
	{
	}

	bool SSAttribute::load(const XMLElement& element)
	{
		if (element.name() != U"attribute")
		{
			return false;
		}
		for (const auto& attribute : element.attributes())
		{
			if (attribute.first == U"tag")
			{
				for (auto i : step(std::size(ATTRIBUTE_KIND_TAG_NAMES)))
				{
					if (ATTRIBUTE_KIND_TAG_NAMES[i] != attribute.second)
					{
						continue;
					}
					m_tag = static_cast<ATTRIBUTE_KIND>(i);
					break;
				}
			}
		}
		for (auto child = element.firstChild(); child; child = child.nextSibling())
		{
			const auto& name = child.name();
			if (name == U"key")
			{
				m_keyValues.emplace_back(m_tag, child);
			}
		}
		return true;
	}

	ATTRIBUTE_KIND SSAttribute::getAttributeKind() const
	{
		return m_tag;
	}

	const Array<SSAttributeKeyValue>& SSAttribute::getKeyValues() const
	{
		return m_keyValues;
	}

	const SSAttributeKeyValue* const SSAttribute::getLeftKeyValue(int32 frame) const
	{
		for (auto it = m_keyValues.rbegin(); it != m_keyValues.rend(); ++it)
		{
			if (it->getKeyFrame() <= frame)
			{
				return &(*it);
			}
		}
		return nullptr;
	}

	const SSAttributeKeyValue* const SSAttribute::getRightKeyValue(int32 frame) const
	{
		for (const auto& it : m_keyValues)
		{
			if (it.getKeyFrame() > frame)
			{
				return &it;
			}
		}
		return nullptr;
	}
}
