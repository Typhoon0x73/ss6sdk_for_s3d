
# pragma once
# include <Siv3D.hpp>
# include "SSProjectSettings.hpp"
# include "SSAnimeSettings.hpp"

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
		void parseFileName     (const XMLElement& element);
		void parseExportPath   (const XMLElement& element);
		void parseSettings     (const XMLElement& element);
		void parseAnimeSettings(const XMLElement& element);

		CreateEditorParam m_createEditorParam{};      //!< エディタ用パラメータを生成する設定
		FilePath          m_projectDirectory { U"" }; //!< exeからのプロジェクトのディレクトリ相対パス
		FilePath          m_projectFileName  { U"" }; //!< プロジェクトファイル名
		String            m_projectName      { U"" }; //!< プロジェクト名
		SSProjectSettings m_projectSettings  {};      //!< プロジェクト設定
		SSAnimeSettings   m_animeSettings    {};      //!< アニメーション設定

		struct EditorParam
		{
			FilePath m_projectExportPath{ U"" }; //!< プロジェクト出力先パス
		};
		std::unique_ptr<EditorParam> m_pEditorParam{ nullptr }; //!< エディタ用パラメータ
	};
}
