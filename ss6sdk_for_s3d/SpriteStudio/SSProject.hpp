
# pragma once
# include <Siv3D.hpp>
# include "SSCommon.hpp"
# include "SSProjectSettings.hpp"
# include "SSAnimeSettings.hpp"
# include "SSTexturePackSettings.hpp"
# include "SSExternalTextures.hpp"

namespace s3d
{
	class SSProject
	{
	public:

		SSProject() = default;

		~SSProject() = default;

		explicit SSProject(FilePathView sspj, bool createEditorParam);

		bool load(FilePathView sspj, bool createEditorParam);

		bool isCreateEditorParam() const;

		const Array<FilePath>& getCellMapNames() const;
		Optional<FilePathView> getCellMapName(size_t index) const;

		const Array<FilePath>& getAnimePackNames() const;
		Optional<FilePathView> getAnimePackName(size_t index) const;

	private:

		// -------------------------------------------------
		// 解析用関数
		// -------------------------------------------------
		void parseFileName        (const XMLElement& element);
		void parseExportPath      (const XMLElement& element);
		void parseSettings        (const XMLElement& element);
		void parseAnimeSettings   (const XMLElement& element);
		void parseTexPackSettings (const XMLElement& element);
		void parseCellmapNames    (const XMLElement& element);
		void parseAnimepackNames  (const XMLElement& element);
		void parseEffectFileNames (const XMLElement& element);
		void parseExternalTextures(const XMLElement& element);

		String                m_name            { U"" }; //!< プロジェクト名
		SSProjectSettings     m_settings        {};      //!< プロジェクト設定
		Array<FilePath>       m_cellmapNames    {};      //!< セルマップ名一覧
		Array<FilePath>       m_animepackNames  {};      //!< アニメーションパック名一覧
		Array<FilePath>       m_effectFileNames {};      //!< エフェクトファイル名一覧
		SSExternalTextures    m_ExternalTextures{};      //!< 他プロジェクトの画像情報

		struct EditorParam
		{
			SSAnimeSettings       m_animeSettings  {};        //!< デフォルトのアニメーション設定
			SSTexturePackSettings m_texPackSettings{};        //!< デフォルトのテクスチャパック設定
			FilePath              m_exportPath     { U"" };   //!< プロジェクト出力先パス
			FilePath              m_lastAnimeFile  { U"" };   //!< 最後に開いていたアニメーションファイル
			String                m_lastAnimeName  { U"" };   //!< 最後に開いていたアニメーション名
			String                m_lastPart       { U"" };   //!< 最後に開いていたパーツ名
			FilePath              m_lastCellMapFile{ U"" };   //!< 最後に開いていたセルマップファイル
			String                m_lastCell       { U"" };   //!< 最後に開いていたセル名
			FilePath              m_lastCharMapFile{ U"" };   //!< 最後に開いていたキャラマップファイル
			FilePath              m_lastEffectFile { U"" };   //!< 最後に開いていたエフェクトファイル
			bool                  m_setupmode      { false }; //!< セットアップモードフラグ
			unknownStr            m_expandAnimation{ U"" };   //!< ？
			unknownStr            m_expandSequence { U"" };   //!< ？
		};
		std::unique_ptr<EditorParam> m_pEditorParam{ nullptr }; //!< エディタ用パラメータ
	};
}
