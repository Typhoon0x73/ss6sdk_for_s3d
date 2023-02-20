
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

		struct FileValue
		{
			FilePath m_pathName{ U"" };
			int32    m_index   { 0 };
			int32    m_flags   { 0 };
		};

		Array<FileValue> m_files{};
	};
}
