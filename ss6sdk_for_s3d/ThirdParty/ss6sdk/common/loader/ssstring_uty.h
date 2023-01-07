#ifndef __SSSTRING_UTY__
#define __SSSTRING_UTY__


#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>

namespace spritestudio6
{

/*
 * @brief     文字列を指定のkeyで分割して返します。
 *
 * @param[in] in_str		分割する文字列
 * @param[in] key			分割のキーとなる文字列
 * @param[out] out_array	分割した文字列を格納する文字列リスト
 * @retval なし
*/
void	split_string( const std::string &in_str , 
						const char key, 
						std::vector<std::string>& out_array );

//bool	is_digit_string( std::string &in_str );
bool	is_digit_string( std::string &in_str , bool* is_priod = 0 );

/*
 * @brief ファイルのフルパスからフォルダパスのみを取得します。
 * @param[in] ファイルパス
 * @retval ファイルパスからファイル名を取り除いた文字列
*/
std::string path2dir(const std::string &path);


/*
 * @brief ファイルのフルパスからファイル名のみを取得します。
 * @param[in] ファイルパス
 * @retval ファイルパスからフォルダ名を取り除いた文字列
*/
std::string path2file(const std::string &path);


/*
 * @brief 相対パスを絶対パスへ変換する
 * param[in] basePath 基準ディレクトリ
 * param[int] relPath 相対パス
 * retval relpathを絶対パスへ変換した値
*/
std::string getFullPath( const std::string& basePath , const std::string &relPath);


std::string nomarizeFilename( std::string str );

/*
* @brief ファイルバージョンチェック
* param[in] fileVersion チェックするファイルのバージョン
* param[in] nowVersion  現在のバージョン
* retval 現在のバージョンより古い場合はfalse
*/
bool checkFileVersion(std::string fileVersion, std::string nowVersion);



class SsStringTokenizer
{
private:
	std::vector<std::string> string_array;
	int	tokenIndex;
	int	tokennum;

public:
	SsStringTokenizer() {}
	virtual ~SsStringTokenizer() {}

	SsStringTokenizer(std::string src_str ,  char token ) {
		split_string(src_str, token, string_array);
		tokenIndex = 0;
		tokennum = (int)string_array.size();
	}

	bool	get(int* out)
	{
		if (isEnd()) return false;
		std::string str = string_array[tokenIndex];
		*out = atoi(str.c_str());
		tokenIndex++;
		return !isEnd();
	}

	bool	get(float* out)
	{
		if (isEnd()) return false;
		std::string str = string_array[tokenIndex];
		*out = (float)atof(str.c_str());
		tokenIndex++;
		return !isEnd();
	}

	bool	get(std::string* str)
	{
		if (isEnd()) return false;
		*str = string_array[tokenIndex];

		tokenIndex++;
		return !isEnd();
	}

	int		tokenNum()
	{
		return tokennum;
	}

	bool	isEnd()
	{
		return (tokennum <= tokenIndex);
	}

};

}	// namespace spritestudio6

#endif
