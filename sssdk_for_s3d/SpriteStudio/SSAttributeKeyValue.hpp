#pragma once
#ifndef SS_ATTRIBUTE_KEY_VALUE_H_
#define SS_ATTRIBUTE_KEY_VALUE_H_

#include <Siv3D.hpp>
#include "SSDefine.hpp"
#include "SSValue.hpp"

namespace sssdk
{
	class SSAttributeKeyValue
	{
	public:

		explicit SSAttributeKeyValue();
		explicit SSAttributeKeyValue(ATTRIBUTE_KIND kind, const XMLElement& element);
		virtual ~SSAttributeKeyValue();

		bool load(ATTRIBUTE_KIND kind, const XMLElement& element);

		SIV3D_NODISCARD_CXX20 int32 getKeyFrame() const;
		SIV3D_NODISCARD_CXX20 InterpolationType getIpType() const;
		SIV3D_NODISCARD_CXX20 const SSValue& getValue() const;
		SIV3D_NODISCARD_CXX20 const SSCurve& getCurve() const;
		SIV3D_NODISCARD_CXX20 float getEaseRate() const;

	private:

		int32 m_keyFrame;
		InterpolationType m_ipType;
		SSValue m_value;
		SSCurve m_curve;
		float m_easeRate;
	};
}

#endif // !SS_ATTRIBUTE_KEY_VALUE_H_
