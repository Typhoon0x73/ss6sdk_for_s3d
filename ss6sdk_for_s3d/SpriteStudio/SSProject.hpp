
# pragma once
# include <Siv3D.hpp>
# include "SSProjectSettings.hpp"

namespace s3d
{
	class SSProject
	{
	public:

		SSProject() = default;

		~SSProject() = default;

		explicit SSProject(FilePathView path, bool createEditorParam);

		bool load(FilePathView path, bool createEditorParam);

	private:

		FilePath          m_projectDirectory{ U"" }; //!< exeからのプロジェクトのディレクトリ相対パス
		FilePath          m_projectFileName { U"" }; //!< プロジェクトファイル名
		String            m_projectName     { U"" }; //!< プロジェクト名
		SSProjectSettings m_projectSettings {};      //!< プロジェクト設定
	};
}
