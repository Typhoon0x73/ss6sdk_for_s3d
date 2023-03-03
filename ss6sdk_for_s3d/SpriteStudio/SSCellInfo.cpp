
# include "SSCellInfo.hpp"
# include "SSCommon.hpp"
# include "SSParseUtilities.hpp"

namespace s3d
{
	SSCellInfo::SSCellInfo(const XMLElement& cell, bool createEditorParam)
	{
		load(cell, createEditorParam);
	}

	bool SSCellInfo::load(const XMLElement& cell, bool createEditorParam)
	{
		static const HashTable<String, void(SSCellInfo::*)(const XMLElement&)> PARSE_TABLE = {
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_name         ], &SSCellInfo::parseName          },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_pos          ], &SSCellInfo::parsePos           },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_size         ], &SSCellInfo::parseSize          },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_pivot        ], &SSCellInfo::parsePivot         },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_rotated      ], &SSCellInfo::parseRotated       },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_orgImageName ], &SSCellInfo::parseOrgImageName  },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_posStable    ], &SSCellInfo::parsePosStable     },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_ismesh       ], &SSCellInfo::parseIsmesh        },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_divtype      ], &SSCellInfo::parseDivtype       },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_innerPoint   ], &SSCellInfo::parseInnerPoint    },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_outerPoint   ], &SSCellInfo::parseOuterPoint    },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_meshPointList], &SSCellInfo::parseMeshPointList },
			{ SS_CELLMAP_TAG_STRINGS[SSCellInfoTag_meshTriList  ], &SSCellInfo::parseMeshTriList   },
		};
		return true;
	}

	void SSCellInfo::parseName(const XMLElement& element)
	{
	}

	void SSCellInfo::parsePos(const XMLElement& element)
	{
	}

	void SSCellInfo::parseSize(const XMLElement& element)
	{
	}

	void SSCellInfo::parsePivot(const XMLElement& element)
	{
	}

	void SSCellInfo::parseRotated(const XMLElement& element)
	{
	}

	void SSCellInfo::parseOrgImageName(const XMLElement& element)
	{
	}

	void SSCellInfo::parsePosStable(const XMLElement& element)
	{
	}

	void SSCellInfo::parseIsmesh(const XMLElement& element)
	{
	}

	void SSCellInfo::parseDivtype(const XMLElement& element)
	{
	}

	void SSCellInfo::parseInnerPoint(const XMLElement& element)
	{
	}

	void SSCellInfo::parseOuterPoint(const XMLElement& element)
	{
	}

	void SSCellInfo::parseMeshPointList(const XMLElement& element)
	{
	}

	void SSCellInfo::parseMeshTriList(const XMLElement& element)
	{
	}
}
