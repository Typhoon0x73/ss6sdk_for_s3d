
# pragma once
# include <Siv3D.hpp>

namespace s3d
{

	class SSBackgroundSettings
	{
	public:

		SSBackgroundSettings() = default;

		~SSBackgroundSettings() = default;

		explicit SSBackgroundSettings(const XMLElement& bgSettings, bool createEditorParam);

		bool load(const XMLElement& bgSettings, bool createEditorParam);

	private:
		
		struct Value
		{
			FilePath m_imagePath  { U"" };
			int32    m_imageDisp  { 0 };
			Point    m_imageOffset{ 0, 0 };
			Size     m_imageCanvas{ 0, 0 };
			Float2   m_imagePivot { 0.0f, 0.0f };
		};

		// -------------------------------------------------
		// 解析用関数
		// -------------------------------------------------
		void parseValue           (const XMLElement& value);
		void parseValueImagePath  (const XMLElement& element, Value& out);
		void parseValueImageDisp  (const XMLElement& element, Value& out);
		void parseValueImageOffset(const XMLElement& element, Value& out);
		void parseValueImageCanvas(const XMLElement& element, Value& out);
		void parseValueImagePivot (const XMLElement& element, Value& out);

		Array<Value> m_values{};
	};
}
