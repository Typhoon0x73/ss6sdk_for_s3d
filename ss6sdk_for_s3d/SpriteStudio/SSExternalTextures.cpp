
# include "SSExternalTextures.hpp"
# include "SSCommon.hpp"

namespace s3d
{
	SSExternalTextures::SSExternalTextures(const XMLElement& ExternalTextures, bool createEditorParam)
	{
		load(ExternalTextures, createEditorParam);
	}

	bool SSExternalTextures::load(const XMLElement& ExternalTextures, bool createEditorParam)
	{
		static const HashTable<String, void(SSExternalTextures::*)(const XMLElement&)> PARSE_TABLE = {
			{ U"files", &SSExternalTextures::parseFiles },
		};
		for (XMLElement element = ExternalTextures.firstChild(); element; element = element.nextSibling())
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

	void SSExternalTextures::parseFiles(const XMLElement& files)
	{
		static const HashTable<String, void(SSExternalTextures::*)(const XMLElement&, FileParam&)> PARSE_TABLE = {
			{ U"value", &SSExternalTextures::parseValue },
		};
		FileParam file;
		for (XMLElement element = files.firstChild(); element; element = element.nextSibling())
		{
			for (const auto& table : PARSE_TABLE)
			{
				if (table.first == element.name())
				{
					(this->*(table.second))(element, file);
					break;
				}
			}
		}
		m_files.emplace_back(file);
	}

	void SSExternalTextures::parseValue(const XMLElement& value, FileParam& outParam)
	{
		static const HashTable<String, void(SSExternalTextures::*)(const XMLElement&, FileParam::Value&)> PARSE_TABLE = {
			{ SS_EXTERNAL_TEXTURES_TAG_STRINGS[SSExternalTexturesTag_PathName], &SSExternalTextures::parsePathName },
			{ SS_EXTERNAL_TEXTURES_TAG_STRINGS[SSExternalTexturesTag_index   ], &SSExternalTextures::parseIndex    },
			{ SS_EXTERNAL_TEXTURES_TAG_STRINGS[SSExternalTexturesTag_flags   ], &SSExternalTextures::parseFlags    },
		};
		FileParam::Value v;
		for (XMLElement element = value.firstChild(); element; element = element.nextSibling())
		{
			for (const auto& table : PARSE_TABLE)
			{
				if (table.first == element.name())
				{
					(this->*(table.second))(element, v);
					break;
				}
			}
		}
		outParam.m_values.emplace_back(v);
	}

	void SSExternalTextures::parsePathName(const XMLElement& element, FileParam::Value& outValue)
	{
		outValue.m_PathName = element.text();
	}

	void SSExternalTextures::parseIndex(const XMLElement& element, FileParam::Value& outValue)
	{
		outValue.m_index = ParseOr<int32, int32>(element.text(), 0);
	}

	void SSExternalTextures::parseFlags(const XMLElement& element, FileParam::Value& outValue)
	{
		outValue.m_flags = ParseOr<int32, int32>(element.text(), 0);
	}
}
