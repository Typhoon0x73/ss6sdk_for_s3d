
# pragma once
# include <Siv3D.hpp>
# include "SSAnimeSettings.hpp"

namespace s3d
{
	class SSModel {};
	class SSAnime {};

	class SSAnimePack
	{
	public:

		SSAnimePack() = default;

		~SSAnimePack() = default;

		explicit SSAnimePack(FilePathView ssae, bool createEditorParam);

		bool load(FilePathView ssae, bool createEditorParam);

	private:

		// -------------------------------------------------
		// 解析用関数
		// -------------------------------------------------
		void parseSettings    (const XMLElement& element);
		void parseName        (const XMLElement& element);
		void parseExportPath  (const XMLElement& element);
		void parseModel       (const XMLElement& element);
		void parseCellmapNames(const XMLElement& element);
		void parseAnimeList   (const XMLElement& element);

		SSAnimeSettings m_settings    {};
		FilePath        m_exportPath  { U"" };
		String          m_name        { U"" };
		SSModel         m_Model       {};
		Array<FilePath> m_cellmapNames{};
		Array<SSAnime>  m_animeList   {};

		bool            m_isCreateEditorParam{ false };
	};
}
