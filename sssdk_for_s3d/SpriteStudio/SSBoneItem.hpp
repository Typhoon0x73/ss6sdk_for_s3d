#pragma once
#ifndef SS_BONE_ITEM_H_
#define SS_BONE_ITEM_H_

#include <Siv3D.hpp>

namespace sssdk
{
	class SSBoneItem
	{
	public:

		explicit SSBoneItem();
		explicit SSBoneItem(const XMLElement& element);
		virtual ~SSBoneItem();

		bool load(const XMLElement& element);

		SIV3D_NODISCARD_CXX20 StringView getKey() const;
		SIV3D_NODISCARD_CXX20 int32 getIndex() const;

	private:

		String m_key;
		int32 m_index;
	};
}

#endif // !SS_BONE_ITEM_H_
