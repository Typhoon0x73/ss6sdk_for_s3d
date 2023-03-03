
# include "SSCellMap.hpp"
# include "SSParseUtilities.hpp"

namespace s3d
{
	SSCellMap::SSCellMap(FilePathView path, SSCreateEditorParam createEditorParam)
	{
		load(path, createEditorParam);
	}

	bool SSCellMap::load(FilePathView path, SSCreateEditorParam createEditorParam)
	{
		XMLReader reader{ path };
		if (not reader.isOpen() or reader.isNull())
		{
			return false;
		}

		static const HashTable<String, void(SSCellMap::*)(const XMLElement&)> PARSE_TABLE = {
			{ SS_CELLMAP_TAG_STRINGS[SSCellMapTag_name      ], &SSCellMap::parseName       },
			{ SS_CELLMAP_TAG_STRINGS[SSCellMapTag_exportPath], &SSCellMap::parseExportPath },
			{ SS_CELLMAP_TAG_STRINGS[SSCellMapTag_imagePath ], &SSCellMap::parseImagePath  },
			{ SS_CELLMAP_TAG_STRINGS[SSCellMapTag_pixelSize ], &SSCellMap::parsePixelSize  },
			{ SS_CELLMAP_TAG_STRINGS[SSCellMapTag_wrapMode  ], &SSCellMap::parseWrapMode   },
			{ SS_CELLMAP_TAG_STRINGS[SSCellMapTag_filterMode], &SSCellMap::parseFilterMode },
			{ SS_CELLMAP_TAG_STRINGS[SSCellMapTag_cells     ], &SSCellMap::parseCells      },
		};

		for (auto element = reader.firstChild(); element; element = element.nextSibling())
		{
			for (const auto& table : PARSE_TABLE)
			{
				if (table.first == element.name())
				{
					(this->*(table.second))(element);
					break;
				}
			}
		}
		return true;
	}

	bool SSCellMap::isCreateEditorParam() const
	{
		return (m_pEditorParam.get() != nullptr);
	}

	void SSCellMap::parseName(const XMLElement& element)
	{
		m_name = element.text();
	}

	void SSCellMap::parseExportPath(const XMLElement& element)
	{
		m_exportPath = element.text();
	}

	void SSCellMap::parseImagePath(const XMLElement& element)
	{
		m_imagePath = element.text();
	}

	void SSCellMap::parsePixelSize(const XMLElement& element)
	{
		m_pixelSize = SSParseUtilities::parseSize(element.text());
	}

	void SSCellMap::parseWrapMode(const XMLElement& element)
	{
		m_wrapMode = SSParseUtilities::parseTextureAddressMode(element.text());
	}

	void SSCellMap::parseFilterMode(const XMLElement& element)
	{
		m_filterMode = SSParseUtilities::parseTextureFilterMode(element.text());
	}

	void SSCellMap::parseCells(const XMLElement& element)
	{
		for (XMLElement e = element.firstChild(); e; e = e.nextSibling())
		{
			if (U"cell" == e.name())
			{
				m_cells.emplace_back(SSCellInfo(e, isCreateEditorParam()));
			}
		}
	}
}
