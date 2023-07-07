#include "sscharconverter.h"

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_MAC
        #include <CoreFoundation/CoreFoundation.h>
    #endif
#elif _WIN32 || _WIN64
    #include <Windows.h>
    #include <locale.h>
    #include <vector>
    #include <system_error>
#else

#endif

namespace spritestudio6
{

std::string SsCharConverter::utf8_to_sjis(const std::string &src) {
#if __APPLE__ && TARGET_OS_MAC
    CFStringRef srcStr = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *) src.c_str(), src.length(), kCFStringEncodingUTF8, FALSE);
    if(!srcStr) {
        return nullptr;
    }

    CFIndex length = CFStringGetLength(srcStr);
    CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingShiftJIS) + 1;
    char *buffer = (char *)calloc(maxSize, sizeof(char));
    if(!buffer) {
        CFRelease(srcStr);
        return nullptr;
    }

    CFStringGetCString(srcStr, buffer, maxSize, kCFStringEncodingShiftJIS);
    std::string dstStr(buffer);
    free(buffer); // If we failed
    CFRelease(srcStr);
    return dstStr;
#elif _WIN32 || _WIN64
    auto const srcSize = (int)src.size();
    auto const srcWCharSize = ::MultiByteToWideChar(CP_UTF8, 0U, src.data(), srcSize, nullptr, 0U);
    std::vector<wchar_t> srcWCharData(srcWCharSize, L'\0');
    if (::MultiByteToWideChar(CP_UTF8, 0U, src.data(), srcSize, srcWCharData.data(), (int)srcWCharSize) == 0) {
        throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
    }
//    srcWCharData.resize(std::char_traits<wchar_t>::length(srcWCharData.data()));
    srcWCharData.shrink_to_fit();

    std::wstring srcWChar(srcWCharData.begin(), srcWCharData.end());
    auto const destSize = ::WideCharToMultiByte(CP_ACP, 0U, srcWChar.data(), -1, nullptr, 0, nullptr, nullptr);
    std::vector<char> dest(destSize, '\0');
    if (::WideCharToMultiByte(CP_ACP, 0U, srcWChar.data(), -1, dest.data(), (int)(dest.size()), nullptr, nullptr) == 0) {
        throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
    }
    dest.resize(std::char_traits<char>::length(dest.data()));
    dest.shrink_to_fit();
    return std::string(dest.begin(), dest.end());
#else
    // TODO: impl
    return std::string(src);
#endif
}

std::string SsCharConverter::sjis_to_utf8(const std::string &src) {
#if __APPLE__ && TARGET_OS_MAC
    // TODO: impl
    /* MEMO: Macの場合は通してUTF-8なので、（原則として変換の必要はないので）今は複製だけして返しています。 */
    return std::string(src);
#elif _WIN32 || _WIN64
    enum Constant {
        SIZE_BUFFER_WTCHAR = 2048,
    };
    wchar_t buffer[Constant::SIZE_BUFFER_WTCHAR];   //終端文字込みの長さであることに注意
    size_t srcWCharSize = 0;
    // MEMO: srcが一度c_strを通ってdata()にバッファが形成されているか保証がないので、c_strをしておく（標準的なstd::stringの実装なら大丈夫なはずなので、念のため）。
    if (::_mbstowcs_s_l(&srcWCharSize, buffer, Constant::SIZE_BUFFER_WTCHAR, src.c_str(), _TRUNCATE, ::_create_locale(LC_ALL, "jpn")) != 0) {
        throw std::system_error{ errno, std::system_category() };
    }
    //MEMO: 先にバッファを指定長で設置して.data()に直接レンダリングしてしまうと、バッファに終端文字が入らない場合があるため……
    std::wstring srcWCharData(buffer);
    srcWCharData.shrink_to_fit();
    srcWCharSize = srcWCharData.size(); // 再取得

    auto const destByteSize = ::WideCharToMultiByte(CP_UTF8, 0U, srcWCharData.data(), (int)srcWCharSize, nullptr, 0, nullptr, nullptr);
    // MEMO: destByteSizeが0の場合は処理を避けないと、throwされてしまうので……
    if(destByteSize <= 0) {
        return std::string();
    }

    std::vector<char> dest(destByteSize, L'\0');
    if (::WideCharToMultiByte(CP_UTF8, 0U, srcWCharData.data(), (int)srcWCharSize, dest.data(), destByteSize, nullptr, nullptr) == 0) {
        throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
    }
//    dest.shrink_to_fit();

    return std::string(dest.begin(), dest.end());
#else
    // TODO: impl
    return std::string(src);
#endif
}


std::string SsCharConverter::convert_path_string(const std::string &str) {
    std::string dst;
#if _WIN32 || _WIN64
    dst = SsCharConverter::utf8_to_sjis(str);
#else
    // pass through
    dst = str;
#endif
    return dst;
}

}   // namespace spritestudio6
