
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
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_name         ], &SSCellInfo::parseName          },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_pos          ], &SSCellInfo::parsePos           },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_size         ], &SSCellInfo::parseSize          },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_pivot        ], &SSCellInfo::parsePivot         },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_rotated      ], &SSCellInfo::parseRotated       },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_orgImageName ], &SSCellInfo::parseOrgImageName  },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_posStable    ], &SSCellInfo::parsePosStable     },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_ismesh       ], &SSCellInfo::parseIsmesh        },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_divtype      ], &SSCellInfo::parseDivtype       },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_innerPoint   ], &SSCellInfo::parseInnerPoint    },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_outerPoint   ], &SSCellInfo::parseOuterPoint    },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_meshPointList], &SSCellInfo::parseMeshPointList },
			{ SS_CELLINFO_TAG_STRINGS[SSCellInfoTag_meshTriList  ], &SSCellInfo::parseMeshTriList   },
		};
		for (auto element = cell.firstChild(); element; element = element.nextSibling())
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

	void SSCellInfo::parseName(const XMLElement& element)
	{
		m_name = element.text();
	}

	void SSCellInfo::parsePos(const XMLElement& element)
	{
		m_pos = SSParseUtilities::parsePoint(element.text());
	}

	void SSCellInfo::parseSize(const XMLElement& element)
	{
		m_size = SSParseUtilities::parseSize(element.text());
	}

	void SSCellInfo::parsePivot(const XMLElement& element)
	{
		m_pivot = SSParseUtilities::parseFloat2(element.text());
	}

	void SSCellInfo::parseRotated(const XMLElement& element)
	{
		m_rotated = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSCellInfo::parseOrgImageName(const XMLElement& element)
	{
		m_orgImageName = element.text();
	}

	void SSCellInfo::parsePosStable(const XMLElement& element)
	{
		m_posStable = SSParseUtilities::parseIntToBool(element.text());
	}

	void SSCellInfo::parseIsmesh(const XMLElement& element)
	{
		m_ismesh = SSParseUtilities::parseIntToBool(element.text());
	}

	void SSCellInfo::parseDivtype(const XMLElement& element)
	{
		m_divtype = element.text();
	}

	void SSCellInfo::parseInnerPoint(const XMLElement& element)
	{
		for (auto e = element.firstChild(); e; e = e.nextSibling())
		{
			if (e.name() == U"value")
			{
				m_innerPoint.emplace_back(SSParseUtilities::parsePoint(e.text()));
			}
		}
	}

	void SSCellInfo::parseOuterPoint(const XMLElement& element)
	{
		for (auto e = element.firstChild(); e; e = e.nextSibling())
		{
			if (e.name() == U"value")
			{
				m_outerPoint.emplace_back(SSParseUtilities::parsePoint(e.text()));
			}
		}
	}

	void SSCellInfo::parseMeshPointList(const XMLElement& element)
	{
		for (auto e = element.firstChild(); e; e = e.nextSibling())
		{
			if (e.name() == U"value")
			{
				m_meshPointList.emplace_back(SSParseUtilities::parsePoint(e.text()));
			}
		}
	}

	void SSCellInfo::parseMeshTriList(const XMLElement& element)
	{
		for (auto e = element.firstChild(); e; e = e.nextSibling())
		{
			if (e.name() == U"value")
			{
				m_meshTriList.emplace_back(SSParseUtilities::parseTriangleIndex(e.text()));
			}
		}
	}
}
