
# pragma once
# include <Siv3D.hpp>
# include "SSCommon.hpp"
# include "SSCellInfo.hpp"
# include "SSTexturePackSettings.hpp"

namespace s3d
{
	class SSCellMap
	{
	public:

		SSCellMap() = default;

		~SSCellMap() = default;

		explicit SSCellMap(FilePathView ssce, bool createEditorParam);

		bool load(FilePathView ssce, bool createEditorParam);

		bool isCreateEditorParam() const;

		StringView getName() const;

		FilePathView getExportPath() const;

		FilePathView getImagePath() const;

		TextureAddressMode getWrapMode() const;

		TextureFilter getFilterMode() const;

	private:

		// -------------------------------------------------
		// 解析用関数
		// -------------------------------------------------
		void parseName      (const XMLElement& element);
		void parseExportPath(const XMLElement& element);
		void parseImagePath (const XMLElement& element);
		void parsePixelSize (const XMLElement& element);
		void parseWrapMode  (const XMLElement& element);
		void parseFilterMode(const XMLElement& element);
		void parseCells     (const XMLElement& element);

		String             m_name      { U"" };
		FilePath           m_exportPath{ U"" };
		FilePath           m_imagePath { U"" };
		Size               m_pixelSize { 0, 0 };
		TextureAddressMode m_wrapMode  {};
		TextureFilter      m_filterMode{};
		Array<SSCellInfo>  m_cells     {};

		struct EditorParam
		{
			unknownStr            m_generator          { U"" };
			unknown32             m_packed             { 0 };
			bool                  m_overrideTexSettings{ false };
			unknownStr            m_imagePathAtImport  { U"" };
			FilePath              m_packInfoFilePath   { U"" };
			SSTexturePackSettings m_texPackSettings    {};        //!< 現状、sspjと同様の設定のため退避
		};
		std::unique_ptr<EditorParam> m_pEditorParam{ nullptr };
	};
}
