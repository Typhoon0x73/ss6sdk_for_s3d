
# pragma once
# include<Siv3D.hpp>

namespace s3d
{
	class SSTexturePackSettings
	{
	public:

		SSTexturePackSettings() = default;
		~SSTexturePackSettings() = default;

		explicit SSTexturePackSettings(const XMLElement& texPackSettings, bool createEditorParam);

		bool load(const XMLElement& texPackSettings, bool createEditorParam);

	private:

		// -------------------------------------------------
		// 解析用関数
		// -------------------------------------------------
		void parseMaxSize    (const XMLElement& element);
		void parseForcePo2   (const XMLElement& element);
		void parseForceSquare(const XMLElement& element);
		void parseMargin     (const XMLElement& element);
		void parsePadding    (const XMLElement& element);

		Point m_maxSize    { 0, 0 };
		bool  m_forcePo2   { false };
		int32 m_forceSquare{ 0 };
		int32 m_margin     { 0 };
		int32 m_padding    { 1 };

	};
}
