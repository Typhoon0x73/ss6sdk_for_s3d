
# pragma once
# include <Siv3D.hpp>
# include "SSProject.hpp"
# include "SSCellMap.hpp"

namespace s3d
{
	class SSProjectDecoder
	{
	public:

		SSProjectDecoder() = default;

		~SSProjectDecoder() = default;

		explicit SSProjectDecoder(FilePathView sspj, SSCreateEditorParam createEditorParam);

		bool load(FilePathView sspj, SSCreateEditorParam createEditorParam);

	private:

		SSProject                         m_project         {};      //!< sspjデータ
		Array<std::unique_ptr<SSCellMap>> m_cellmaps        {};      //!< ssceデータ
		HashTable<String, Texture>        m_textureMap      {};      //!< 利用する画像(key = セルマップ名)
		FilePath                          m_projectDirectory{ U"" }; //!< exeからのプロジェクトのディレクトリ相対パス
		FilePath                          m_projectFileName { U"" }; //!< プロジェクトファイル名
	};
}
