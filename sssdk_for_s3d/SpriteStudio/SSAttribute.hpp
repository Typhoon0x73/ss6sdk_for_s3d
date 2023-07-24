#pragma once
#ifndef SS_ATTRIBUTE_H_
#define SS_ATTRIBUTE_H_

#include <Siv3D.hpp>
#include "SSDefine.hpp"
#include "SSAttributeKeyValue.hpp"

namespace sssdk
{
	class SSAttribute
	{
	public:

		explicit SSAttribute();
		explicit SSAttribute(const XMLElement& element);
		virtual ~SSAttribute();

		bool load(const XMLElement& element);

		SIV3D_NODISCARD_CXX20 ATTRIBUTE_KIND getAttributeKind() const;
		SIV3D_NODISCARD_CXX20 const Array<SSAttributeKeyValue>& getKeyValues() const;
		SIV3D_NODISCARD_CXX20 const SSAttributeKeyValue* const getLeftKeyValue(int32 frame) const;
		SIV3D_NODISCARD_CXX20 const SSAttributeKeyValue* const getRightKeyValue(int32 frame) const;

	private:

		ATTRIBUTE_KIND m_tag;
		Array<SSAttributeKeyValue> m_keyValues;
	};
}

#endif // !SS_ATTRIBUTE_H_
