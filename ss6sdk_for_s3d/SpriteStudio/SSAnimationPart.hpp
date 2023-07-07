#pragma once
#ifndef SS_ANIMATION_PART_H_
#define SS_ANIMATION_PART_H_

#include <Siv3D.hpp>
#include "SSAttribute.hpp"

namespace sssdk
{
	class SSAnimationPart
	{
	public:

		explicit SSAnimationPart();
		explicit SSAnimationPart(const XMLElement& element);
		virtual ~SSAnimationPart();

		bool load(const XMLElement& element);

		SIV3D_NODISCARD_CXX20 StringView getName() const;
		SIV3D_NODISCARD_CXX20 const Array<SSAttribute>& getAttributes() const;

	private:

		String m_name;
		Array<SSAttribute> m_attributes;

	};
}

#endif // !SS_ANIMATION_PART_H_
