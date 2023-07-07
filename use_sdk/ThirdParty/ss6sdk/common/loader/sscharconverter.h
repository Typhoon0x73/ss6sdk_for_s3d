#ifndef SSSDK_SSCHARCONVERTER_H
#define SSSDK_SSCHARCONVERTER_H

#include <string>

namespace spritestudio6
{

class SsCharConverter {
public:
	static std::string utf8_to_sjis(const std::string &src);
	static std::string sjis_to_utf8(const std::string &src);
	static std::string convert_path_string(const std::string &str);
};

}	// namespace spritestudio6

#endif //SSSDK_SSCHARCONVERTER_H
