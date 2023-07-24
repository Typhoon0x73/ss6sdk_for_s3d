#pragma once
#ifndef SS_SETTINGS_H_
#define SS_SETTINGS_H_

#include <Siv3D.hpp>

namespace sssdk
{
	class SSSettings
	{
	public:

		explicit SSSettings();
		explicit SSSettings(const XMLElement& element);
		virtual ~SSSettings();

		bool load(const XMLElement& element);

		SIV3D_NODISCARD_CXX20 const FilePath& getAnimeBaseDirectory() const;
		SIV3D_NODISCARD_CXX20 const FilePath& getCellmapBaseDirectory() const;
		SIV3D_NODISCARD_CXX20 const FilePath& getImageBaseDirectory() const;
		SIV3D_NODISCARD_CXX20 const FilePath& getEffectBaseDirectory() const;

	private:

		FilePath m_animeBaseDirectory;
		FilePath m_cellmapBaseDirectory;
		FilePath m_imageBaseDirectory;
		FilePath m_effectBaseDirectory;
	};
}

#endif // !SS_SETTINGS_H_
