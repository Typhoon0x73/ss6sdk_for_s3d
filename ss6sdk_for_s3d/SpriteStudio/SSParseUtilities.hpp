
# pragma once
# include <Siv3D.hpp>

namespace s3d
{

	static const HashTable<String, TextureAddressMode> SS_TEXTURE_WRAP_MODE_TABLE = {
		{ U"clamp" , TextureAddressMode::Clamp  },
		{ U"repeat", TextureAddressMode::Repeat },
		{ U"mirror", TextureAddressMode::Mirror },
	};
	static const HashTable<String, TextureFilter> SS_TEXTURE_FILTER_MODE_TABLE = {
		{ U"nearlest", TextureFilter::Nearest },
		{ U"linear"  , TextureFilter::Linear  },
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
			return ParseOr<Point>(tmpStr, Point::Zero());
		}

		inline static Size parseSize(const String& text)
		{
			return parsePoint(text);
		}

		inline static TriangleIndex parseTriangleIndex(const String& text)
		{
			TriangleIndex re = TriangleIndex::Zero();
			size_t firstPos  = text.indexOf(U' ', 0);
			size_t secondPos = text.indexOf(U' ', firstPos + 1);
			re.i0 = ParseOr<int32, int32>(text.substr(0, firstPos), 0);
			re.i1 = ParseOr<int32, int32>(text.substr((firstPos + 1), secondPos - (firstPos + 1)), 0);
			re.i2 = ParseOr<int32, int32>(text.substr((secondPos + 1)), 0);
			return re;
		}

		inline static Float2 parseFloat2(const String& text)
		{
			String tmpStr = U"({})"_fmt(convertSpaceToComma(text));
			return ParseOr<Float2>(tmpStr, Float2::Zero());
		}

		inline static Vec2 parseVec2(const String& text)
		{
			String tmpStr = U"({})"_fmt(convertSpaceToComma(text));
			return ParseOr<Vec2>(tmpStr, Vec2::Zero());
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
