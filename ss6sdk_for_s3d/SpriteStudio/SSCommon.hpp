
# pragma once
# include <Siv3D.hpp>

# pragma region "setup macros"
# if defined(SS_ENUM_TO_CONST_NUM)
#     pragma push_macro("SS_ENUM_TO_CONST_NUM")
#     define IS_PUSH_MACRO_SS_ENUM_TO_CONST_NUM
# endif //defined(SS_ENUM_TO_CONST_NUM)

# if defined(SS_CONSTEXPR_STRINGS_BEGIN)
#     pragma push_macro("SS_CONSTEXPR_STRINGS_BEGIN")
#     define IS_PUSH_MACRO_SS_CONSTEXPR_STRINGS_BEGIN
# endif //defined(SS_CONSTEXPR_STRINGS_BEGIN)

# if defined(SS_CONSTEXPR_STRINGS_END)
#     pragma push_macro("SS_CONSTEXPR_STRINGS_END")
#     define IS_PUSH_MACRO_SS_CONSTEXPR_STRINGS_END
# endif //defined(SS_CONSTEXPR_STRINGS_END)

# if defined(SS_TO_U32_STR)
#     pragma push_macro("SS_TO_U32_STR")
#     define IS_PUSH_MACRO_SS_TO_U32_STR
# endif //defined(SS_TO_U32_STR)

# define SS_ENUM_TO_CONST_NUM(enumtype,name)  constexpr int64 enumtype##_##name = static_cast<int64>(enumtype##::##name);
# define SS_CONSTEXPR_STRINGS_BEGIN(enumtype) constexpr const char32_t* const enumtype##_STRINGS[] = {
# define SS_CONSTEXPR_STRINGS_END             };
# define SS_TO_U32_STR(name)                  U ## #name
# pragma endregion

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
		name            ,
		exportPath      ,
		settings        ,
		animeSettings   ,
		texPackSettings ,
		cellmapNames    ,
		animepackNames  ,
		effectFileNames ,
		ExternalTextures,
	};
	SS_ENUM_TO_CONST_NUM(SSProjectTag,name            )
	SS_ENUM_TO_CONST_NUM(SSProjectTag,exportPath      )
	SS_ENUM_TO_CONST_NUM(SSProjectTag,settings        )
	SS_ENUM_TO_CONST_NUM(SSProjectTag,animeSettings   )
	SS_ENUM_TO_CONST_NUM(SSProjectTag,texPackSettings )
	SS_ENUM_TO_CONST_NUM(SSProjectTag,cellmapNames    )
	SS_ENUM_TO_CONST_NUM(SSProjectTag,animepackNames  )
	SS_ENUM_TO_CONST_NUM(SSProjectTag,effectFileNames )
	SS_ENUM_TO_CONST_NUM(SSProjectTag,ExternalTextures)

	SS_CONSTEXPR_STRINGS_BEGIN(SS_PROJECT_TAG)
		SS_TO_U32_STR(name            ),
		SS_TO_U32_STR(exportPath      ),
		SS_TO_U32_STR(settings        ),
		SS_TO_U32_STR(animeSettings   ),
		SS_TO_U32_STR(texPackSettings ),
		SS_TO_U32_STR(cellmapNames    ),
		SS_TO_U32_STR(animepackNames  ),
		SS_TO_U32_STR(effectFileNames ),
		SS_TO_U32_STR(ExternalTextures),
	SS_CONSTEXPR_STRINGS_END

	enum class SSProjectSettingsTag {
		animeBaseDirectory      ,
		cellMapBaseDirectory    ,
		imageBaseDirectory      ,
		effectBaseDirectory     ,
		exportBaseDirectory     ,
		queryExportBaseDirectory,
		wrapMode                ,
		filterMode              ,
		vertexAnimeFloat        ,
	};
	SS_ENUM_TO_CONST_NUM(SSProjectSettingsTag,animeBaseDirectory      )
	SS_ENUM_TO_CONST_NUM(SSProjectSettingsTag,cellMapBaseDirectory    )
	SS_ENUM_TO_CONST_NUM(SSProjectSettingsTag,imageBaseDirectory      )
	SS_ENUM_TO_CONST_NUM(SSProjectSettingsTag,effectBaseDirectory     )
	SS_ENUM_TO_CONST_NUM(SSProjectSettingsTag,exportBaseDirectory     )
	SS_ENUM_TO_CONST_NUM(SSProjectSettingsTag,queryExportBaseDirectory)
	SS_ENUM_TO_CONST_NUM(SSProjectSettingsTag,wrapMode                )
	SS_ENUM_TO_CONST_NUM(SSProjectSettingsTag,filterMode              )
	SS_ENUM_TO_CONST_NUM(SSProjectSettingsTag,vertexAnimeFloat        )

	SS_CONSTEXPR_STRINGS_BEGIN(SS_PROJECT_SETTINGS_TAG)
		SS_TO_U32_STR(animeBaseDirectory      ),
		SS_TO_U32_STR(cellMapBaseDirectory    ),
		SS_TO_U32_STR(imageBaseDirectory      ),
		SS_TO_U32_STR(effectBaseDirectory     ),
		SS_TO_U32_STR(exportBaseDirectory     ),
		SS_TO_U32_STR(queryExportBaseDirectory),
		SS_TO_U32_STR(wrapMode                ),
		SS_TO_U32_STR(filterMode              ),
		SS_TO_U32_STR(vertexAnimeFloat        ),
	SS_CONSTEXPR_STRINGS_END

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
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,fps        )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,frameCount )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,sortMode   )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,canvasSize )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,pivot      )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,bgColor    )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,gridSize   )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,gridColor  )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,ik_depth   )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,startFrame )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,endFrame   )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,bgSettings )
	SS_ENUM_TO_CONST_NUM(SSAnimeSettingsTag,outStartNum)

	SS_CONSTEXPR_STRINGS_BEGIN(SS_ANIME_SETTINGS_TAG)
		SS_TO_U32_STR(fps        ),
		SS_TO_U32_STR(frameCount ),
		SS_TO_U32_STR(sortMode   ),
		SS_TO_U32_STR(canvasSize ),
		SS_TO_U32_STR(pivot      ),
		SS_TO_U32_STR(bgColor    ),
		SS_TO_U32_STR(gridSize   ),
		SS_TO_U32_STR(gridColor  ),
		SS_TO_U32_STR(ik_depth   ),
		SS_TO_U32_STR(startFrame ),
		SS_TO_U32_STR(endFrame   ),
		SS_TO_U32_STR(bgSettings ),
		SS_TO_U32_STR(outStartNum),
	SS_CONSTEXPR_STRINGS_END

	enum class SSBackgroundSettingsTag {
		imagePath  ,
		imageDisp  ,
		imageOffset,
		imageCanvas,
		imagePivot ,
	};
	SS_ENUM_TO_CONST_NUM(SSBackgroundSettingsTag,imagePath  )
	SS_ENUM_TO_CONST_NUM(SSBackgroundSettingsTag,imageDisp  )
	SS_ENUM_TO_CONST_NUM(SSBackgroundSettingsTag,imageOffset)
	SS_ENUM_TO_CONST_NUM(SSBackgroundSettingsTag,imageCanvas)
	SS_ENUM_TO_CONST_NUM(SSBackgroundSettingsTag,imagePivot )

	SS_CONSTEXPR_STRINGS_BEGIN(SS_BACKGROUND_SETTINGS_TAG)
		SS_TO_U32_STR(imagePath  ),
		SS_TO_U32_STR(imageDisp  ),
		SS_TO_U32_STR(imageOffset),
		SS_TO_U32_STR(imageCanvas),
		SS_TO_U32_STR(imagePivot ),
	SS_CONSTEXPR_STRINGS_END

	enum class SSTexturePackSettingsTag {
		maxSize    ,
		forcePo2   ,
		forceSquare,
		margin     ,
		padding    ,
	};
	SS_ENUM_TO_CONST_NUM(SSTexturePackSettingsTag,maxSize    )
	SS_ENUM_TO_CONST_NUM(SSTexturePackSettingsTag,forcePo2   )
	SS_ENUM_TO_CONST_NUM(SSTexturePackSettingsTag,forceSquare)
	SS_ENUM_TO_CONST_NUM(SSTexturePackSettingsTag,margin     )
	SS_ENUM_TO_CONST_NUM(SSTexturePackSettingsTag,padding    )

	SS_CONSTEXPR_STRINGS_BEGIN(SS_TEXTURE_PACK_SETTINGS_TAG)
		SS_TO_U32_STR(maxSize    ),
		SS_TO_U32_STR(forcePo2   ),
		SS_TO_U32_STR(forceSquare),
		SS_TO_U32_STR(margin     ),
		SS_TO_U32_STR(padding    ),
	SS_CONSTEXPR_STRINGS_END

	enum class SSExternalTexturesTag {
		PathName,
		index   ,
		flags   ,
	};
	SS_ENUM_TO_CONST_NUM(SSExternalTexturesTag,PathName)
	SS_ENUM_TO_CONST_NUM(SSExternalTexturesTag,index   )
	SS_ENUM_TO_CONST_NUM(SSExternalTexturesTag,flags   )

	SS_CONSTEXPR_STRINGS_BEGIN(SS_EXTERNAL_TEXTURES_TAG)
		SS_TO_U32_STR(PathName),
		SS_TO_U32_STR(index   ),
		SS_TO_U32_STR(flags   ),
	SS_CONSTEXPR_STRINGS_END

	enum class SSCellMapTag {
		name               ,
		exportPath         ,
		imagePath          ,
		pixelSize          ,
		overrideTexSettings,
		wrapMode           ,
		filterMode         ,
		cells              ,
	};
	SS_ENUM_TO_CONST_NUM(SSCellMapTag,name               )
	SS_ENUM_TO_CONST_NUM(SSCellMapTag,exportPath         )
	SS_ENUM_TO_CONST_NUM(SSCellMapTag,imagePath          )
	SS_ENUM_TO_CONST_NUM(SSCellMapTag,pixelSize          )
	SS_ENUM_TO_CONST_NUM(SSCellMapTag,overrideTexSettings)
	SS_ENUM_TO_CONST_NUM(SSCellMapTag,wrapMode           )
	SS_ENUM_TO_CONST_NUM(SSCellMapTag,filterMode         )
	SS_ENUM_TO_CONST_NUM(SSCellMapTag,cells              )

	SS_CONSTEXPR_STRINGS_BEGIN(SS_CELLMAP_TAG)
		SS_TO_U32_STR(name               ),
		SS_TO_U32_STR(exportPath         ),
		SS_TO_U32_STR(imagePath          ),
		SS_TO_U32_STR(pixelSize          ),
		SS_TO_U32_STR(overrideTexSettings),
		SS_TO_U32_STR(wrapMode           ),
		SS_TO_U32_STR(filterMode         ),
		SS_TO_U32_STR(cells              ),
	SS_CONSTEXPR_STRINGS_END

	enum class SSCellInfoTag {
		name         ,
		pos          ,
		size         ,
		pivot        ,
		rotated      ,
		orgImageName ,
		posStable    ,
		ismesh       ,
		divtype      ,
		innerPoint   ,
		outerPoint   ,
		meshPointList,
		meshTriList  ,
	};
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,name         )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,pos          )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,size         )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,pivot        )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,rotated      )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,orgImageName )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,posStable    )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,ismesh       )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,divtype      )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,innerPoint   )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,outerPoint   )
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,meshPointList)
	SS_ENUM_TO_CONST_NUM(SSCellInfoTag,meshTriList  )

	SS_CONSTEXPR_STRINGS_BEGIN(SS_CELLINFO_TAG)
		SS_TO_U32_STR(name         ),
		SS_TO_U32_STR(pos          ),
		SS_TO_U32_STR(size         ),
		SS_TO_U32_STR(pivot        ),
		SS_TO_U32_STR(rotated      ),
		SS_TO_U32_STR(orgImageName ),
		SS_TO_U32_STR(posStable    ),
		SS_TO_U32_STR(ismesh       ),
		SS_TO_U32_STR(divtype      ),
		SS_TO_U32_STR(innerPoint   ),
		SS_TO_U32_STR(outerPoint   ),
		SS_TO_U32_STR(meshPointList),
		SS_TO_U32_STR(meshTriList  ),
	SS_CONSTEXPR_STRINGS_END

	enum class SSAnimePackTag {
		settings    ,
		name        ,
		exportPath  ,
		Model       ,
		cellmapNames,
		animeList   ,
	};
	SS_ENUM_TO_CONST_NUM(SSAnimePackTag,settings    )
	SS_ENUM_TO_CONST_NUM(SSAnimePackTag,name        )
	SS_ENUM_TO_CONST_NUM(SSAnimePackTag,exportPath  )
	SS_ENUM_TO_CONST_NUM(SSAnimePackTag,Model       )
	SS_ENUM_TO_CONST_NUM(SSAnimePackTag,cellmapNames)
	SS_ENUM_TO_CONST_NUM(SSAnimePackTag,animeList   )

	SS_CONSTEXPR_STRINGS_BEGIN(SS_ANIME_PACK_TAG)
		SS_TO_U32_STR(settings    ),
		SS_TO_U32_STR(name        ),
		SS_TO_U32_STR(exportPath  ),
		SS_TO_U32_STR(Model       ),
		SS_TO_U32_STR(cellmapNames),
		SS_TO_U32_STR(animeList   ),
	SS_CONSTEXPR_STRINGS_END
}

# pragma region "cleanup macros"
# undef SS_TO_U32_STR
# undef SS_CONSTEXPR_STRINGS_END            
# undef SS_CONSTEXPR_STRINGS_BEGIN
# undef SS_ENUM_TO_CONST_NUM

# if defined(IS_PUSH_MACRO_SS_TO_U32_STR)
#     undef IS_PUSH_MACRO_SS_TO_U32_STR
#     pragma pop_macro("SS_TO_U32_STR")
# endif// defined(IS_PUSH_MACRO_SS_TO_U32_STR)

# if defined(IS_PUSH_MACRO_SS_CONSTEXPR_STRINGS_END)
#     undef IS_PUSH_MACRO_SS_CONSTEXPR_STRINGS_END
#     pragma pop_macro("SS_CONSTEXPR_STRINGS_END")
# endif// defined(IS_PUSH_MACRO_SS_CONSTEXPR_STRINGS_END)

# if defined(IS_PUSH_MACRO_SS_CONSTEXPR_STRINGS_BEGIN)
#     undef IS_PUSH_MACRO_SS_CONSTEXPR_STRINGS_BEGIN
#     pragma pop_macro("SS_CONSTEXPR_STRINGS_BEGIN")
# endif// defined(IS_PUSH_MACRO_SS_CONSTEXPR_STRINGS_BEGIN)

# if defined(IS_PUSH_MACRO_SS_ENUM_TO_CONST_NUM)
#     undef IS_PUSH_MACRO_SS_ENUM_TO_CONST_NUM
#     pragma pop_macro("SS_ENUM_TO_CONST_NUM")
# endif// defined(IS_PUSH_MACRO_SS_ENUM_TO_CONST_NUM)
# pragma endregion
