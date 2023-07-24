#pragma once
#ifndef SS_ANIMATION_H_
#define SS_ANIMATION_H_

#include <Siv3D.hpp>
#include "SSDefine.hpp"
#include "SSAnimeSettings.hpp"
#include "SSAnimationPart.hpp"

namespace sssdk
{
	class SSAnimation
	{
	public:

		explicit SSAnimation();
		explicit SSAnimation(const XMLElement& element);
		virtual ~SSAnimation();

		bool load(const XMLElement& element);

		SIV3D_NODISCARD_CXX20 StringView getName() const;
		SIV3D_NODISCARD_CXX20 bool isOverrideSettings() const;
		SIV3D_NODISCARD_CXX20 const SSAnimeSettings& getAnimeSettings() const;
		SIV3D_NODISCARD_CXX20 bool isSetup() const;
		SIV3D_NODISCARD_CXX20 const Array<SSLabel>& getLabels() const;
		SIV3D_NODISCARD_CXX20 const Array<SSAnimationPart>& getAnimParts() const;
		SIV3D_NODISCARD_CXX20 const SSAnimationPart* const getAnimPart(StringView name) const;

	private:

		String m_name;
		bool m_isOverrideSettings;
		SSAnimeSettings m_animeSettings;
		bool m_isSetup;
		Array<SSLabel> m_labels;
		Array<SSAnimationPart> m_animParts;
	};
}

#endif // !SS_ANIMATION_H_
