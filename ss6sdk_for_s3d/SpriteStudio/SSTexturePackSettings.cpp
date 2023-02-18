
# include "SSTexturePackSettings.hpp"
# include "SSCommon.hpp"
# include "SSParseUtilities.hpp"

namespace s3d
{
	SSTexturePackSettings::SSTexturePackSettings(const XMLElement& texPackSettings, bool createEditorParam)
	{
		load(texPackSettings, createEditorParam);
	}

	bool SSTexturePackSettings::load(const XMLElement& texPackSettings, bool createEditorParam)
	{
		static const HashTable<String, void(SSTexturePackSettings::*)(const XMLElement&)> PARSE_TABLE = {
			{ SS_TEXTURE_PACK_SETTINGS_TAG_STRINGS[SSTexturePackSettingsTag_maxSize    ], &SSTexturePackSettings::parseMaxSize     },
			{ SS_TEXTURE_PACK_SETTINGS_TAG_STRINGS[SSTexturePackSettingsTag_forcePo2   ], &SSTexturePackSettings::parseForcePo2    },
			{ SS_TEXTURE_PACK_SETTINGS_TAG_STRINGS[SSTexturePackSettingsTag_forceSquare], &SSTexturePackSettings::parseForceSquare },
			{ SS_TEXTURE_PACK_SETTINGS_TAG_STRINGS[SSTexturePackSettingsTag_margin     ], &SSTexturePackSettings::parseMargin      },
			{ SS_TEXTURE_PACK_SETTINGS_TAG_STRINGS[SSTexturePackSettingsTag_padding    ], &SSTexturePackSettings::parsePadding     },
		};
		for (auto element = texPackSettings.firstChild(); element; element = element.nextSibling())
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

	void SSTexturePackSettings::parseMaxSize(const XMLElement& element)
	{
		m_maxSize = SSParseUtilities::parsePoint(element.text());
	}

	void SSTexturePackSettings::parseForcePo2(const XMLElement& element)
	{
		m_forcePo2 = SSParseUtilities::parseIntToBool(element.text());
	}

	void SSTexturePackSettings::parseForceSquare(const XMLElement& element)
	{
		m_forceSquare = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSTexturePackSettings::parseMargin(const XMLElement& element)
	{
		m_margin = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSTexturePackSettings::parsePadding(const XMLElement& element)
	{
		m_padding = ParseOr<int32, int32>(element.text(), 0);
	}
}
