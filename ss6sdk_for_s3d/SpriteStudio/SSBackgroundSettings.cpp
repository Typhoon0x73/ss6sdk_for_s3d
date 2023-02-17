#include "SSBackgroundSettings.hpp"
#include "SSCommon.hpp"
#include "SSParseUtilities.hpp"

namespace s3d
{
	SSBackgroundSettings::SSBackgroundSettings(const XMLElement& bgSettings, bool createEditorParam)
	{
		load(bgSettings, createEditorParam);
	}

	bool SSBackgroundSettings::load(const XMLElement& bgSettings, bool createEditorParam)
	{
		static const HashTable<String, void(SSBackgroundSettings::*)(const XMLElement&)> PARSE_TABLE = {
			{ SS_BACKGROUND_SETTINGS_TAG_STRINGS[SSBackGroundSettingsTag_value], &SSBackgroundSettings::parseValue },
		};
		for (auto element = bgSettings.firstChild(); element; element = element.nextSibling())
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

	void SSBackgroundSettings::parseValue(const XMLElement& value)
	{
		static const HashTable<String, void(SSBackgroundSettings::*)(const XMLElement&, Value&)> PARSE_TABLE = {
			{ SS_BACKGROUND_SETTINGS_VALUE_TAG_STRINGS[SSBackGroundSettingsValueTag_imagePath  ], &SSBackgroundSettings::parseValueImagePath   },
			{ SS_BACKGROUND_SETTINGS_VALUE_TAG_STRINGS[SSBackGroundSettingsValueTag_imageDisp  ], &SSBackgroundSettings::parseValueImageDisp   },
			{ SS_BACKGROUND_SETTINGS_VALUE_TAG_STRINGS[SSBackGroundSettingsValueTag_imageOffset], &SSBackgroundSettings::parseValueImageOffset },
			{ SS_BACKGROUND_SETTINGS_VALUE_TAG_STRINGS[SSBackGroundSettingsValueTag_imageCanvas], &SSBackgroundSettings::parseValueImageCanvas },
			{ SS_BACKGROUND_SETTINGS_VALUE_TAG_STRINGS[SSBackGroundSettingsValueTag_imagePivot ], &SSBackgroundSettings::parseValueImagePivot  },
		};
		Value outValue;
		for (auto element = value.firstChild(); element; element = element.nextSibling())
		{
			for (const auto& table : PARSE_TABLE)
			{
				if (table.first == element.name())
				{
					(this->*(table.second))(element, outValue);
					break;
				}
			}
		}
		m_values.emplace_back(outValue);
	}

	void SSBackgroundSettings::parseValueImagePath(const XMLElement& element, Value& out)
	{
		out.m_imagePath = element.text();
	}

	void SSBackgroundSettings::parseValueImageDisp(const XMLElement& element, Value& out)
	{
		out.m_imageDisp = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSBackgroundSettings::parseValueImageOffset(const XMLElement& element, Value& out)
	{
		out.m_imageOffset = SSParseUtilities::parsePoint(element.text());
	}

	void SSBackgroundSettings::parseValueImageCanvas(const XMLElement& element, Value& out)
	{
		out.m_imageCanvas = SSParseUtilities::parseSize(element.text());
	}

	void SSBackgroundSettings::parseValueImagePivot(const XMLElement& element, Value& out)
	{
		out.m_imagePivot = SSParseUtilities::parseFloat2(element.text());
	}
}
