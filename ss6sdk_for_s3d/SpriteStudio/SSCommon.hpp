
# pragma once
# include <Siv3D.hpp>

# define ASSERT(x) __debugbreak()

namespace s3d
{
	using unknown32  = int32;
	using unknownStr = String;

	class SSRenderingSettings {};
	class SSEffectSettings {};

	enum class SSProjectTag {
		name,
		settings,
	};
	constexpr size_t SSProjectTag_name      = static_cast<size_t>(SSProjectTag::name    );
	constexpr size_t SSProjectTag_settings  = static_cast<size_t>(SSProjectTag::settings);

	constexpr const char32_t* const SS_PROJECT_TAG_STRINGS[] = {
		U"name",
		U"settings",
	};

	enum class SSProjectSettingTag {
		animeBaseDirectory,
		cellMapBaseDirectory,
		imageBaseDirectory,
		effectBaseDirectory,
		exportBaseDirectory,
		queryExportBaseDirectory,
		wrapMode,
		filterMode,
		vertexAnimeFloat,
	};
	constexpr size_t SSProjectSettingTag_animeBaseDirectory       = static_cast<size_t>(SSProjectSettingTag::animeBaseDirectory      );
	constexpr size_t SSProjectSettingTag_cellMapBaseDirectory     = static_cast<size_t>(SSProjectSettingTag::cellMapBaseDirectory    );
	constexpr size_t SSProjectSettingTag_imageBaseDirectory       = static_cast<size_t>(SSProjectSettingTag::imageBaseDirectory      );
	constexpr size_t SSProjectSettingTag_effectBaseDirectory      = static_cast<size_t>(SSProjectSettingTag::effectBaseDirectory     );
	constexpr size_t SSProjectSettingTag_exportBaseDirectory      = static_cast<size_t>(SSProjectSettingTag::exportBaseDirectory     );
	constexpr size_t SSProjectSettingTag_queryExportBaseDirectory = static_cast<size_t>(SSProjectSettingTag::queryExportBaseDirectory);
	constexpr size_t SSProjectSettingTag_wrapMode                 = static_cast<size_t>(SSProjectSettingTag::wrapMode                );
	constexpr size_t SSProjectSettingTag_filterMode               = static_cast<size_t>(SSProjectSettingTag::filterMode              );
	constexpr size_t SSProjectSettingTag_vertexAnimeFloat         = static_cast<size_t>(SSProjectSettingTag::vertexAnimeFloat        );

	constexpr const char32_t* const SS_PROJECT_SETTINGS_TAG_STRINGS[] = {
		U"animeBaseDirectory",
		U"cellMapBaseDirectory",
		U"imageBaseDirectory",
		U"effectBaseDirectory",
		U"exportBaseDirectory",
		U"queryExportBaseDirectory",
		U"wrapMode",
		U"filterMode",
		U"vertexAnimeFloat",
	};

	static const HashTable<String, TextureAddressMode> SS_TEXTURE_WRAP_MODE_TABLE = {
		{ U"clamp", TextureAddressMode::Clamp },
	};
	static const HashTable<String, TextureFilter> SS_TEXTURE_FILTER_MODE_TABLE = {
		{ U"linear", TextureFilter::Linear },
	};
}
