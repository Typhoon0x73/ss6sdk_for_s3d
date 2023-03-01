
# pragma once
# include <Siv3D.hpp>

# define ASSERT(x) __debugbreak()

namespace s3d
{
	using unknown32  = int32;  // 数字か真偽値かわからない用
	using unknownStr = String; // 何用途かわからない用

	class SSRenderingSettings {};
	class SSEffectSettings {};

	/// @brief エディタ用パラメータを生成するかの列挙値
	enum class SSCreateEditorParam
	{
		NoCreate,                 //!< 生成しない
		Create,                   //!< 生成する
	};

	enum class SSProjectTag {
		name,
		exportPath,
		settings,
		animeSettings,
		texPackSettings,
		cellmapNames,
		animepackNames,
		effectFileNames,
		ExternalTextures,
	};
	constexpr size_t SSProjectTag_name             = static_cast<size_t>(SSProjectTag::name            );
	constexpr size_t SSProjectTag_exportPath       = static_cast<size_t>(SSProjectTag::exportPath      );
	constexpr size_t SSProjectTag_settings         = static_cast<size_t>(SSProjectTag::settings        );
	constexpr size_t SSProjectTag_animeSettings    = static_cast<size_t>(SSProjectTag::animeSettings   );
	constexpr size_t SSProjectTag_texPackSettings  = static_cast<size_t>(SSProjectTag::texPackSettings );
	constexpr size_t SSProjectTag_cellmapNames     = static_cast<size_t>(SSProjectTag::cellmapNames    );
	constexpr size_t SSProjectTag_animepackNames   = static_cast<size_t>(SSProjectTag::animepackNames  );
	constexpr size_t SSProjectTag_effectFileNames  = static_cast<size_t>(SSProjectTag::effectFileNames );
	constexpr size_t SSProjectTag_ExternalTextures = static_cast<size_t>(SSProjectTag::ExternalTextures);

	constexpr const char32_t* const SS_PROJECT_TAG_STRINGS[] = {
		U"name",
		U"exportPath",
		U"settings",
		U"animeSettings",
		U"texPackSettings",
		U"cellmapNames",
		U"animepackNames",
		U"effectFileNames",
		U"ExternalTextures",
	};

	enum class SSProjectSettingsTag {
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
	constexpr size_t SSProjectSettingsTag_animeBaseDirectory       = static_cast<size_t>(SSProjectSettingsTag::animeBaseDirectory      );
	constexpr size_t SSProjectSettingsTag_cellMapBaseDirectory     = static_cast<size_t>(SSProjectSettingsTag::cellMapBaseDirectory    );
	constexpr size_t SSProjectSettingsTag_imageBaseDirectory       = static_cast<size_t>(SSProjectSettingsTag::imageBaseDirectory      );
	constexpr size_t SSProjectSettingsTag_effectBaseDirectory      = static_cast<size_t>(SSProjectSettingsTag::effectBaseDirectory     );
	constexpr size_t SSProjectSettingsTag_exportBaseDirectory      = static_cast<size_t>(SSProjectSettingsTag::exportBaseDirectory     );
	constexpr size_t SSProjectSettingsTag_queryExportBaseDirectory = static_cast<size_t>(SSProjectSettingsTag::queryExportBaseDirectory);
	constexpr size_t SSProjectSettingsTag_wrapMode                 = static_cast<size_t>(SSProjectSettingsTag::wrapMode                );
	constexpr size_t SSProjectSettingsTag_filterMode               = static_cast<size_t>(SSProjectSettingsTag::filterMode              );
	constexpr size_t SSProjectSettingsTag_vertexAnimeFloat         = static_cast<size_t>(SSProjectSettingsTag::vertexAnimeFloat        );

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

	enum class SSAnimeSettingsTag {
		fps		   ,
		frameCount ,
		sortMode   ,
		canvasSize ,
		pivot	   ,
		bgColor	   ,
		gridSize   ,
		gridColor  ,
		ik_depth   ,
		startFrame ,
		endFrame   ,
		bgSettings ,
		outStartNum,
	};
	constexpr size_t SSAnimeSettingsTag_fps         = static_cast<size_t>(SSAnimeSettingsTag::fps        );
	constexpr size_t SSAnimeSettingsTag_frameCount  = static_cast<size_t>(SSAnimeSettingsTag::frameCount );
	constexpr size_t SSAnimeSettingsTag_sortMode    = static_cast<size_t>(SSAnimeSettingsTag::sortMode   );
	constexpr size_t SSAnimeSettingsTag_canvasSize  = static_cast<size_t>(SSAnimeSettingsTag::canvasSize );
	constexpr size_t SSAnimeSettingsTag_pivot       = static_cast<size_t>(SSAnimeSettingsTag::pivot      );
	constexpr size_t SSAnimeSettingsTag_bgColor     = static_cast<size_t>(SSAnimeSettingsTag::bgColor    );
	constexpr size_t SSAnimeSettingsTag_gridSize    = static_cast<size_t>(SSAnimeSettingsTag::gridSize   );
	constexpr size_t SSAnimeSettingsTag_gridColor   = static_cast<size_t>(SSAnimeSettingsTag::gridColor  );
	constexpr size_t SSAnimeSettingsTag_ik_depth    = static_cast<size_t>(SSAnimeSettingsTag::ik_depth   );
	constexpr size_t SSAnimeSettingsTag_startFrame  = static_cast<size_t>(SSAnimeSettingsTag::startFrame );
	constexpr size_t SSAnimeSettingsTag_endFrame    = static_cast<size_t>(SSAnimeSettingsTag::endFrame   );
	constexpr size_t SSAnimeSettingsTag_bgSettings  = static_cast<size_t>(SSAnimeSettingsTag::bgSettings );
	constexpr size_t SSAnimeSettingsTag_outStartNum = static_cast<size_t>(SSAnimeSettingsTag::outStartNum);

	constexpr const char32_t* const SS_ANIME_SETTINGS_TAG_STRINGS[] = {
		U"fps",
		U"frameCount",
		U"sortMode",
		U"canvasSize",
		U"pivot",
		U"bgColor",
		U"gridSize",
		U"gridColor",
		U"ik_depth",
		U"startFrame",
		U"endFrame",
		U"bgSettings",
		U"outStartNum",
	};

	enum class SSBackgroundSettingsTag {
		value,
	};
	constexpr size_t SSBackGroundSettingsTag_value = static_cast<size_t>(SSBackgroundSettingsTag::value);

	constexpr const char32_t* const SS_BACKGROUND_SETTINGS_TAG_STRINGS[] = {
		U"value",
	};

	enum class SSBackgroundSettingsValueTag {
		imagePath,
		imageDisp,
		imageOffset,
		imageCanvas,
		imagePivot,
	};
	constexpr size_t SSBackGroundSettingsValueTag_imagePath   = static_cast<size_t>(SSBackgroundSettingsValueTag::imagePath  );
	constexpr size_t SSBackGroundSettingsValueTag_imageDisp   = static_cast<size_t>(SSBackgroundSettingsValueTag::imageDisp  );
	constexpr size_t SSBackGroundSettingsValueTag_imageOffset = static_cast<size_t>(SSBackgroundSettingsValueTag::imageOffset);
	constexpr size_t SSBackGroundSettingsValueTag_imageCanvas = static_cast<size_t>(SSBackgroundSettingsValueTag::imageCanvas);
	constexpr size_t SSBackGroundSettingsValueTag_imagePivot  = static_cast<size_t>(SSBackgroundSettingsValueTag::imagePivot );

	constexpr const char32_t* const SS_BACKGROUND_SETTINGS_VALUE_TAG_STRINGS[] = {
		U"imagePath",
		U"imageDisp",
		U"imageOffset",
		U"imageCanvas",
		U"imagePivot",
	};

	enum class SSTexturePackSettingsTag {
		maxSize,
		forcePo2,
		forceSquare,
		margin,
		padding,
	};
	constexpr size_t SSTexturePackSettingsTag_maxSize     = static_cast<size_t>(SSTexturePackSettingsTag::maxSize    );
	constexpr size_t SSTexturePackSettingsTag_forcePo2    = static_cast<size_t>(SSTexturePackSettingsTag::forcePo2   );
	constexpr size_t SSTexturePackSettingsTag_forceSquare = static_cast<size_t>(SSTexturePackSettingsTag::forceSquare);
	constexpr size_t SSTexturePackSettingsTag_margin      = static_cast<size_t>(SSTexturePackSettingsTag::margin     );
	constexpr size_t SSTexturePackSettingsTag_padding     = static_cast<size_t>(SSTexturePackSettingsTag::padding    );

	constexpr const char32_t* const SS_TEXTURE_PACK_SETTINGS_TAG_STRINGS[] = {
		U"maxSize",
		U"forcePo2",
		U"forceSquare",
		U"margin",
		U"padding",
	};

	enum class SSExternalTexturesTag {
		files,
	};
	constexpr size_t SSExternalTexturesTag_files = static_cast<size_t>(SSExternalTexturesTag::files);

	constexpr const char32_t* const SS_EXTERNAL_TEXTURES_TAG_STRINGS[] = {
		U"files",
	};

	enum class SSExternalTexturesFilesTag {
		value,
	};
	constexpr size_t SSExternalTexturesFilesTag_value = static_cast<size_t>(SSExternalTexturesFilesTag::value);

	constexpr const char32_t* const SS_EXTERNAL_TEXTURES_FILES_TAG_STRINGS[] = {
		U"value",
	};

	enum class SSExternalTexturesFilesValueTag {
		PathName,
		index,
		flags,
	};
	constexpr size_t SSExternalTexturesFilesValueTag_PathName = static_cast<size_t>(SSExternalTexturesFilesValueTag::PathName);
	constexpr size_t SSExternalTexturesFilesValueTag_index    = static_cast<size_t>(SSExternalTexturesFilesValueTag::index   );
	constexpr size_t SSExternalTexturesFilesValueTag_flags    = static_cast<size_t>(SSExternalTexturesFilesValueTag::flags   );

	constexpr const char32_t* const SS_EXTERNAL_TEXTURES_FILES_VALUE_TAG_STRINGS[] = {
		U"PathName",
		U"index",
		U"flags",
	};

	static const HashTable<String, TextureAddressMode> SS_TEXTURE_WRAP_MODE_TABLE = {
		{ U"clamp", TextureAddressMode::Clamp },
	};
	static const HashTable<String, TextureFilter> SS_TEXTURE_FILTER_MODE_TABLE = {
		{ U"linear", TextureFilter::Linear },
	};
}
