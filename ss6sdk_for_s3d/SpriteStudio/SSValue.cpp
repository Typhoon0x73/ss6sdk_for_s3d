
#include "SSValue.hpp"

namespace
{
	static const HashTable<sssdk::ATTRIBUTE_KIND, sssdk::SSValue::TYPE> SSVALUE_TYPE_TABLE =
	{
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_CELL, sssdk::SSValue::TYPE::TYPE_CELL   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_POSX, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_POSY, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_POSZ, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_ROTX, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_ROTY, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_ROTZ, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_SCLX, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_SCLY, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_LSCX, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_LSCY, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_ALPH, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_LALP, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_PRIO, sssdk::SSValue::TYPE::TYPE_INT    },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_FLPH, sssdk::SSValue::TYPE::TYPE_BOOL   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_FLPV, sssdk::SSValue::TYPE::TYPE_BOOL   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_HIDE, sssdk::SSValue::TYPE::TYPE_BOOL   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_PCOL, sssdk::SSValue::TYPE::TYPE_PCOL   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_VCOL, sssdk::SSValue::TYPE::TYPE_UNKOWN },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_SHDR, sssdk::SSValue::TYPE::TYPE_UNKOWN },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_VERT, sssdk::SSValue::TYPE::TYPE_VERT   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_PVTX, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_PVTY, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_ANCX, sssdk::SSValue::TYPE::TYPE_UNKOWN },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_ANCY, sssdk::SSValue::TYPE::TYPE_UNKOWN },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_SIZX, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_SIZY, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_IFLH, sssdk::SSValue::TYPE::TYPE_BOOL   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_IFLV, sssdk::SSValue::TYPE::TYPE_BOOL   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_UVTX, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_UVTY, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_UVRZ, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_UVSX, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_UVSY, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_BNDR, sssdk::SSValue::TYPE::TYPE_FLOAT  },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_MASK, sssdk::SSValue::TYPE::TYPE_INT    },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_USER, sssdk::SSValue::TYPE::TYPE_HASH   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_SIGN, sssdk::SSValue::TYPE::TYPE_UNKOWN },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_IPRM, sssdk::SSValue::TYPE::TYPE_IPRM   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_EFCT, sssdk::SSValue::TYPE::TYPE_EFCT   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_DEFM, sssdk::SSValue::TYPE::TYPE_DEFM   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_ADIO, sssdk::SSValue::TYPE::TYPE_ADIO   },
		{ sssdk::ATTRIBUTE_KIND::ATTRIBUTE_KIND_TCHG, sssdk::SSValue::TYPE::TYPE_TCHG   },
	};
}

namespace sssdk
{
	SSValue::SSValue()
		: m_value{}
	{
	}

	SSValue::SSValue(ATTRIBUTE_KIND kind, const XMLElement& element)
	{
		set(kind, element);
	}

	SSValue::SSValue(TYPE type, const XMLElement& element)
	{
		set(type, element);
	}

	SSValue::~SSValue()
	{
	}

	void SSValue::set(ATTRIBUTE_KIND kind, const XMLElement& element)
	{
		TYPE type = TYPE_UNKOWN;
		for (const auto& it : SSVALUE_TYPE_TABLE)
		{
			if (it.first == kind)
			{
				type = it.second;
				break;
			}
		}
		set(type, element);
	}

	void SSValue::set(TYPE type, const XMLElement& element)
	{
		switch (type)
		{
		case SSValue::TYPE_STRING:
			m_value = element.text();
			break;
		case SSValue::TYPE_INT:
			m_value = ParseOr<int32, int32>(element.text(), 0);
			break;
		case SSValue::TYPE_FLOAT:
			m_value = ParseOr<float, float>(element.text(), 0.0f);
			break;
		case SSValue::TYPE_BOOL:
			m_value = (ParseOr<int32, int32>(element.text(), 0) == 1);
			break;
		case SSValue::TYPE_HASH:
		{
			auto value = HashTable<String, SSValue>();
			for (auto child = element.firstChild(); child; child = child.nextSibling())
			{
				value.insert(std::make_pair(child.name(), SSValue(SSValue::TYPE_STRING, child)));
			}
			m_value = value;
		}
			break;
		case SSValue::TYPE_ARRAY:
			break;
		case SSValue::TYPE_CELL:
		{
			SSCellValue cellValue;
			for (auto child = element.firstChild(); child; child = child.nextSibling())
			{
				const auto& name = child.name();
				if (name == U"mapId")
				{
					cellValue.mapId = ParseOr<uint64, uint64>(child.text(), 0U);
				}
				else if (name == U"name")
				{
					cellValue.name = child.text();
				}
			}
			m_value = cellValue;
		}
			break;
		case SSValue::TYPE_PCOL:
		{
			SSPartColorValue pcolValue;
			pcolValue.target = ColorBlendTarget::invalid;
			auto setvcol = [](SSVertexColorValue& v, const XMLElement& e) {
				for (auto child = e.firstChild(); child; child = child.nextSibling())
				{
					const auto& name = child.name();
					if (name == U"rgba")
					{
						const auto& col = child.text();
						v.col = Color{ U"x" + col.substr(2, 6) + col.substr(0, 2) };
					}
					else if (name == U"rate")
					{
						v.rate = ParseOr<float, float>(child.text(), 0.0f);
					}
				}
			};
			for (auto child = element.firstChild(); child; child = child.nextSibling())
			{
				const auto& name = child.name();
				if (name == U"target")
				{
					if (child.text() == U"whole")
					{
						pcolValue.target = ColorBlendTarget::whole;
					}
					else if (child.text() == U"vertex")
					{
						pcolValue.target = ColorBlendTarget::vertex;
					}
				}
				else if (name == U"blendType")
				{
					for (auto i : step(std::size(DRAW_MODE_TAG_NAMES)))
					{
						if (child.text().lowercased() == DRAW_MODE_TAG_NAMES[i])
						{
							pcolValue.blendType = static_cast<DRAW_MODE>(i);
							break;
						}
					}
				}
				else if (name == U"color" or name == U"LT")
				{
					setvcol(pcolValue.vcol[0], child);
				}
				else if (name == U"RT")
				{
					setvcol(pcolValue.vcol[1], child);
				}
				else if (name == U"LB")
				{
					setvcol(pcolValue.vcol[2], child);
				}
				else if (name == U"RB")
				{
					setvcol(pcolValue.vcol[3], child);
				}
			}
			m_value = pcolValue;
		}
			break;
		case SSValue::TYPE_VERT:
		{
			SSVertexValue value;
			for (auto child = element.firstChild(); child; child = child.nextSibling())
			{
				const auto& name = child.name();
				if (name == U"LT")
				{
					const auto& lines = child.text().split(U' ');
					value.leftTop.x = ParseOr<float, float>(lines[0], 0.0f);
					value.leftTop.y = ParseOr<float, float>(lines[1], 0.0f);
				}
				else if (name == U"RT")
				{
					const auto& lines = child.text().split(U' ');
					value.rightTop.x = ParseOr<float, float>(lines[0], 0.0f);
					value.rightTop.y = ParseOr<float, float>(lines[1], 0.0f);
				}
				else if (name == U"LB")
				{
					const auto& lines = child.text().split(U' ');
					value.leftBottom.x = ParseOr<float, float>(lines[0], 0.0f);
					value.leftBottom.y = ParseOr<float, float>(lines[1], 0.0f);
				}
				else if (name == U"RB")
				{
					const auto& lines = child.text().split(U' ');
					value.rightBottom.x = ParseOr<float, float>(lines[0], 0.0f);
					value.rightBottom.y = ParseOr<float, float>(lines[1], 0.0f);
				}
			}
			m_value = value;
		}
			break;
		case SSValue::TYPE_IPRM:
		{
			SSInstanceParamValue value;
			for (auto child = element.firstChild(); child; child = child.nextSibling())
			{
				const auto& name = child.name();
				if (name == U"infinity")
				{
					value.infinity = ParseOr<int32, int32>(child.text(), 0);
				}
				else if (name == U"reverse")
				{
					value.reverse = ParseOr<int32, int32>(child.text(), 0);
				}
				else if (name == U"pingpong")
				{
					value.pingpong = ParseOr<int32, int32>(child.text(), 0);
				}
				else if (name == U"independent")
				{
					value.independent = ParseOr<int32, int32>(child.text(), 0);
				}
				else if (name == U"loopNum")
				{
					value.loopNum = ParseOr<int32, int32>(child.text(), 0);
				}
				else if (name == U"startLabel")
				{
					value.startLabel = child.text();
				}
				else if (name == U"startOffset")
				{
					value.startOffset = ParseOr<int32, int32>(child.text(), 0);
				}
				else if (name == U"endLabel")
				{
					value.endLabel = child.text();
				}
				else if (name == U"endOffset")
				{
					value.endOffset = ParseOr<int32, int32>(child.text(), 0);
				}
				else if (name == U"speed")
				{
					value.speed = ParseOr<float, float>(child.text(), 0.0);
				}
			}
			m_value = value;
		}
			break;
		case SSValue::TYPE_EFCT:
		{
			SSEffectValue value;
			for (auto child = element.firstChild(); child; child = child.nextSibling())
			{
				const auto& name = child.name();
				if (name == U"startTime")
				{
					value.startTime = ParseOr<int32, int32>(child.text(), 0);
				}
				else if (name == U"speed")
				{
					value.speed = ParseOr<float, float>(child.text(), 0.0f);
				}
				else if (name == U"independent")
				{
					value.independent = ParseOr<int32, int32>(child.text(), 0);
				}
			}
			m_value = value;
		}
			break;
		case SSValue::TYPE_DEFM:
		{
			SSDeformVertexValue value;
			for (auto child = element.firstChild(); child; child = child.nextSibling())
			{
				const auto& name = child.name();
				if (name == U"vsize")
				{
					value.vsize = ParseOr<int32, int32>(child.text(), 0U);
				}
				else if (name == U"vchg")
				{
					auto lines = child.text().split(U' ');
					if (lines.size() > 0)
					{
						value.arraySize = ParseOr<int32, int32>(lines[0], 0);
					}
					for (int32 i = 1; i < value.arraySize; i += 3)
					{
						SSDeformVertexValue::vchg vchg;
						vchg.index = ParseOr<int32, int32>(lines[i], 0);
						vchg.point.x = ParseOr<float, float>(lines[i + 1], 0);
						vchg.point.y = ParseOr<float, float>(lines[i + 2], 0);
						value.vchgs.emplace_back(vchg);
					}
				}
			}
			m_value = value;
		}
			break;
		case SSValue::TYPE_ADIO:
		{
			SSAudioValue value;
			for (auto child = element.firstChild(); child; child = child.nextSibling())
			{
				const auto& name = child.name();
				if (name == U"soundListId")
				{
					value.soundListId = ParseOr<uint64, uint64>(child.text(), 0);
				}
				else if (name == U"soundName")
				{
					value.soundName = child.text();
				}
				else if (name == U"loopNum")
				{
					value.loopNum = ParseOr<int32, int32>(child.text(), 0);
				}
			}
			m_value = value;
		}
			break;
		case SSValue::TYPE_TCHG:
		{
			SSTextureChangeValue value;
			for (auto child = element.firstChild(); child; child = child.nextSibling())
			{
				const auto& name = child.name();
				if (name == U"TextureName")
				{
					value.name = child.text();
				}
			}
			m_value = value;
		}
			break;
		case SSValue::TYPE_UNKOWN:
		default:
			break;
		}
	}

	SSValue::TYPE SSValue::getType() const
	{
		return static_cast<TYPE>(m_value.index());
	}
}

