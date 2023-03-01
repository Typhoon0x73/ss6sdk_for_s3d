
# include "SSCellMap.hpp"

namespace s3d
{
	SSCellMap::SSCellMap(FilePathView path, SSCreateEditorParam createEditorParam)
	{
		load(path, createEditorParam);
	}

	bool SSCellMap::load(FilePathView path, SSCreateEditorParam createEditorParam)
	{
		return false;
	}

	bool SSCellMap::isCreateEditorParam() const
	{
		return (m_pEditorParam.get() != nullptr);
	}

	void SSCellMap::parseName(const XMLElement& element)
	{
	}

	void SSCellMap::parseExportPath(const XMLElement& element)
	{
	}

	void SSCellMap::parseImagePath(const XMLElement& element)
	{
	}

	void SSCellMap::parsePixelSize(const XMLElement& element)
	{
	}

	void SSCellMap::parseWrapMode(const XMLElement& element)
	{
	}

	void SSCellMap::parseFilterMode(const XMLElement& element)
	{
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
