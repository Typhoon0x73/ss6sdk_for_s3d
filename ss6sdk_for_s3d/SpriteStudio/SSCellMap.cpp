
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
