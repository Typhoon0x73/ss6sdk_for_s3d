#pragma once
#ifndef SS_PROJECT_H_
#define SS_PROJECT_H_

#include <Siv3D.hpp>
#include "SSSettings.hpp"
#include "SSAnimeSettings.hpp"
#include "SSCellmap.hpp"
#include "SSAnimationPack.hpp"

namespace sssdk
{
	class ISSCellmaps
	{
	public:
		SIV3D_NODISCARD_CXX20 virtual const SSCellmap* const getCellmap(StringView) const = 0;
		SIV3D_NODISCARD_CXX20 virtual const SSCellmap* const getCellmap(StringView, uint64) const = 0;
	};

	class SSProject : public ISSCellmaps
	{
	public:
		explicit SSProject();
		explicit SSProject(FilePathView path);
		virtual ~SSProject();

		bool load(FilePathView path);

		SIV3D_NODISCARD_CXX20 StringView getVersion() const;

		SIV3D_NODISCARD_CXX20 StringView getSpriteStudioEditorVersion() const;

		SIV3D_NODISCARD_CXX20 const FilePath& getProjectFileDirectory() const;
		SIV3D_NODISCARD_CXX20 FilePath getImageFileDirectory() const;
		SIV3D_NODISCARD_CXX20 FilePath getSSCEFileDirectory() const;
		SIV3D_NODISCARD_CXX20 FilePath getSSAEFileDirectory() const;
		SIV3D_NODISCARD_CXX20 FilePath getSSEEFileDirectory() const;

		SIV3D_NODISCARD_CXX20 StringView getName() const;

		SIV3D_NODISCARD_CXX20 const SSSettings& getSettings() const;

		SIV3D_NODISCARD_CXX20 const SSAnimeSettings& getAnimeSettings() const;

		SIV3D_NODISCARD_CXX20 const Array<String>& getCellmapNames() const;
		SIV3D_NODISCARD_CXX20 const Array<String>& getAnimepackNames() const;
		SIV3D_NODISCARD_CXX20 const Array<String>& getEffectFileNames() const;

		SIV3D_NODISCARD_CXX20 virtual const SSCellmap* const getCellmap(StringView ssce) const override;
		SIV3D_NODISCARD_CXX20 virtual const SSCellmap* const getCellmap(StringView ssae, uint64 index) const override;
		SIV3D_NODISCARD_CXX20 const SSAnimationPack* const getAnimPack(StringView ssae) const;
		SIV3D_NODISCARD_CXX20 const SSAnimation* const getAnimation(StringView ssae, StringView anim) const;

	private:

		String m_version;
		String m_ssEditorVersion;

		FilePath m_projectDirectiory;

		String m_name;

		SSSettings m_settings;

		SSAnimeSettings m_animeSettings;

		Array<String> m_cellmapNames;
		Array<String> m_animepackNames;
		Array<String> m_effectFileNames;

		Array<SSCellmap> m_cellmaps;
		Array<SSAnimationPack> m_animPacks;
	};
}

#endif // !SS_PROJECT_H_
