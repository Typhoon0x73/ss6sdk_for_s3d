
# pragma once
# include <Siv3D.hpp>
# include "SSProjectSettings.hpp"
# include "SSAnimeSettings.hpp"
# include "SSTexturePackSettings.hpp"
# include "SSExternalTextures.hpp"

namespace s3d
{
	class SSProject
	{
	public:

		/// @brief エディタ用パラメータを生成するかの列挙値
		enum class CreateEditorParam
		{
			NoCreate,                 //!< 生成しない
			Create,                   //!< 生成する
		};

		SSProject() = default;

		~SSProject() = default;

		explicit SSProject(FilePathView path, CreateEditorParam createEditorParam);

		bool load(FilePathView path, CreateEditorParam createEditorParam);

		bool isCreateEditorParam() const;

	private:

		// -------------------------------------------------
		// 解析用関数
		// -------------------------------------------------
		void parseFileName       (const XMLElement& element);
		void parseExportPath     (const XMLElement& element);
		void parseSettings       (const XMLElement& element);
		void parseAnimeSettings  (const XMLElement& element);
		void parseTexPackSettings(const XMLElement& element);

		CreateEditorParam     m_createEditorParam{};      //!< エディタ用パラメータを生成する設定
		FilePath              m_projectDirectory { U"" }; //!< exeからのプロジェクトのディレクトリ相対パス
		FilePath              m_projectFileName  { U"" }; //!< プロジェクトファイル名
		String                m_projectName      { U"" }; //!< プロジェクト名
		SSProjectSettings     m_projectSettings  {};      //!< プロジェクト設定
		SSAnimeSettings       m_animeSettings    {};      //!< アニメーション設定
		SSTexturePackSettings m_texPackSettings  {};      //!< テクスチャパック設定
		Array<FilePath>       m_cellmapNames     {};      //!< セルマップ名一覧
		Array<FilePath>       m_animepackNames   {};      //!< アニメーションパック名一覧
		Array<FilePath>       m_effectFileNames  {};      //!< エフェクトファイル名一覧
		SSExternalTextures    m_ExternalTextures {};      //!< 他プロジェクトの画像情報

		struct EditorParam
		{
			FilePath   m_projectExportPath{ U"" };   //!< プロジェクト出力先パス
			FilePath   m_lastAnimeFile    { U"" };   //!< 最後に開いていたアニメーションファイル
			String     m_lastAnimeName    { U"" };   //!< 最後に開いていたアニメーション名
			String     m_lastPart         { U"" };   //!< 最後に開いていたパーツ名
			FilePath   m_lastCellMapFile  { U"" };   //!< 最後に開いていたセルマップファイル
			String     m_lastCell         { U"" };   //!< 最後に開いていたセル名
			FilePath   m_lastCharMapFile  { U"" };   //!< 最後に開いていたキャラマップファイル
			FilePath   m_lastEffectFile   { U"" };   //!< 最後に開いていたエフェクトファイル
			bool       m_setupmode        { false }; //!< セットアップモードフラグ
			unknownStr m_expandAnimation  { U"" };   //!< ？
			unknownStr m_expandSequence   { U"" };   //!< ？
		};
		std::unique_ptr<EditorParam> m_pEditorParam{ nullptr }; //!< エディタ用パラメータ
	};
}
