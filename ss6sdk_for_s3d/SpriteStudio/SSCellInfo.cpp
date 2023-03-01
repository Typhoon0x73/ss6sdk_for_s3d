
# include "SSCellInfo.hpp"

namespace s3d
{
	SSCellInfo::SSCellInfo(const XMLElement& cell, bool createEditorParam)
	{
		load(cell, createEditorParam);
	}

	bool SSCellInfo::load(const XMLElement& cell, bool createEditorParam)
	{
		return false;
	}
}
