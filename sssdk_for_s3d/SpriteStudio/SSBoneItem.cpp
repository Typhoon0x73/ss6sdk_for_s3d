
#include "SSBoneItem.hpp"

namespace sssdk
{
	SSBoneItem::SSBoneItem()
		: m_key{ U"" }
		, m_index{ 0 }
	{
	}

	SSBoneItem::SSBoneItem(const XMLElement& element)
		: SSBoneItem()
	{
		load(element);
	}

	SSBoneItem::~SSBoneItem()
	{
	}

	bool SSBoneItem::load(const XMLElement& element)
	{
		if (element.name() != U"item")
		{
			return false;
		}
		for (const auto& attribute : element.attributes())
		{
			if (attribute.first == U"key")
			{
				m_key = attribute.second;
			}
		}
		m_index = ParseOr<int32, int32>(element.text(), 0);
		return true;
	}

	StringView SSBoneItem::getKey() const
	{
		return m_key;
	}

	int32 SSBoneItem::getIndex() const
	{
		return m_index;
	}
}
