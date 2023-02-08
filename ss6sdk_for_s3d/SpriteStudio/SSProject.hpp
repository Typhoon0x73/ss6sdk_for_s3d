
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

		explicit SSProject(FilePathView path);

		bool load(FilePathView path);

	private:

		FilePath          m_projectDirectory{ U"" }; //!< exeからのプロジェクトのディレクトリ相対パス
		FilePath          m_projectFileName { U"" }; //!< プロジェクトファイル名
		SSProjectSettings m_projectSettings {};      //!< プロジェクト設定
	};
}
