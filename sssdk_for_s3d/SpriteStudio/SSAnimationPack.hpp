#pragma once
#ifndef SS_ANIMATION_PACK_H_
#define SS_ANIMATION_PACK_H_

#include <Siv3D.hpp>
#include "SSSettings.hpp"
#include "SSModel.hpp"
#include "SSAnimation.hpp"

namespace sssdk
{
	class SSAnimationPack
	{
	public:

		explicit SSAnimationPack();
		explicit SSAnimationPack(FilePathView path);
		virtual ~SSAnimationPack();

		bool load(FilePathView path);

		SIV3D_NODISCARD_CXX20 StringView getVersion() const;
		SIV3D_NODISCARD_CXX20 const SSSettings& getSettings() const;
		SIV3D_NODISCARD_CXX20 StringView getName() const;
		SIV3D_NODISCARD_CXX20 const SSModel& getModel() const;
		SIV3D_NODISCARD_CXX20 const Array<String>& getCellmapNames() const;
		SIV3D_NODISCARD_CXX20 const Array<SSAnimation>& getAnimations() const;
		SIV3D_NODISCARD_CXX20 const SSAnimation* const getAnimation(int32 index) const;
		SIV3D_NODISCARD_CXX20 const SSAnimation* const getAnimation(StringView anim) const;
		SIV3D_NODISCARD_CXX20 const SSAnimation* const getSetupAnimation() const;

	private:

		String m_version;

		SSSettings m_settings;

		String m_name;

		SSModel m_model;

		Array<String> m_cellmapNames;

		Array<SSAnimation> m_animations;
	};
}

#endif // !SS_ANIMATION_PACK_H_
