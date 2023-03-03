
# pragma once
# include <Siv3D.hpp>

namespace s3d
{

	static const HashTable<String, TextureAddressMode> SS_TEXTURE_WRAP_MODE_TABLE = {
		{ U"clamp", TextureAddressMode::Clamp },
	};
	static const HashTable<String, TextureFilter> SS_TEXTURE_FILTER_MODE_TABLE = {
		{ U"linear", TextureFilter::Linear },
	};

	class SSParseUtilities
	{
	private:

		SSParseUtilities()  = delete;
		~SSParseUtilities() = delete;

		inline static String convertSpaceToComma(const String& text)
		{
			return text.replaced_if([](const char32& c) { return c == U' '; }, U',');
		}

	public:

		inline static Point parsePoint(const String& text)
		{
			String tmpStr = U"({})"_fmt(convertSpaceToComma(text));
			return ParseOr<Point>(tmpStr, Point{ 0, 0 });
		}

		inline static Size parseSize(const String& text)
		{
			return parsePoint(text);
		}

		inline static Float2 parseFloat2(const String& text)
		{
			String tmpStr = U"({})"_fmt(convertSpaceToComma(text));
			return ParseOr<Float2>(tmpStr, Float2{ 0.0f, 0.0f });
		}

		inline static Vec2 parseVec2(const String& text)
		{
			String tmpStr = U"({})"_fmt(convertSpaceToComma(text));
			return ParseOr<Vec2>(tmpStr, Vec2{ 0.0f, 0.0f });
		}

		inline static ColorF parseColorF(StringView text)
		{
			// AARRGGBB -> RRGGBBAA
			String tmpStr = text.substr(2, 6) + text.substr(0, 2);
			return ParseOr<ColorF>(U"#{}"_fmt(tmpStr), ColorF{ 1.0 });
		}

		inline static bool parseIntToBool(StringView text)
		{
			int32 parse = ParseInt<int32>(text);
			return (parse > 0);
		}

		inline static TextureAddressMode parseTextureAddressMode(StringView text)
		{
			const auto& data = SS_TEXTURE_WRAP_MODE_TABLE.find(text);
			if (data == SS_TEXTURE_WRAP_MODE_TABLE.end())
			{
				return TextureAddressMode::Clamp;
			}
			return data->second;
		}

		inline static TextureFilter parseTextureFilterMode(StringView text)
		{
			const auto& data = SS_TEXTURE_FILTER_MODE_TABLE.find(text);
			if (data == SS_TEXTURE_FILTER_MODE_TABLE.end())
			{
				return TextureFilter::Linear;
			}
			return data->second;
		}
	};
}
