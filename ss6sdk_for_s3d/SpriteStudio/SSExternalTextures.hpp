
# pragma once
# include <Siv3D.hpp>

namespace s3d
{
	class SSExternalTextures
	{
	public:

		SSExternalTextures() = default;

		~SSExternalTextures() = default;

		explicit SSExternalTextures(const XMLElement& ExternalTextures, bool createEditorParam);

		bool load(const XMLElement& ExternalTextures, bool createEditorParam);

	private:

		struct FileParam
		{
			struct Value
			{
				FilePath m_PathName{ U"" };
				int32    m_index   { 0 };
				int32    m_flags   { 0 };
			};
			Array<Value> m_values{};
		};

		// -------------------------------------------------
		// 解析用関数
		// -------------------------------------------------
		void parseFiles   (const XMLElement& files);
		void parseValue   (const XMLElement& value, FileParam& outParam);
		void parsePathName(const XMLElement& element, FileParam::Value& outValue);
		void parseIndex   (const XMLElement& element, FileParam::Value& outValue);
		void parseFlags   (const XMLElement& element, FileParam::Value& outValue);

		Array<FileParam> m_files{};
	};
}
