#include "ssstring_uty.h"
#include "sscharconverter.h"

#ifdef _WIN32
#include <direct.h>
#include <algorithm>
#else
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#endif

namespace spritestudio6
{


//=========================================================================================
//! 文字列の切り分け
/*!
	検索キーを元に文字列を切り分け、文字列の配列を返します。
	@param[in] in_str 入力文字列
	@param[in] key 検索キー
	@param[out] out_array 出力文字列配列
	@retval なし

*/
//=========================================================================================
void	split_string( const std::string &in_str , 
						const char key, 
						std::vector<std::string>& out_array )
{
	std::istringstream in(in_str);
	char c;
	out_array.clear();
	while (in)
	{
		std::string index;
		while (in.get(c) && (c != key))	index.push_back(c);

		out_array.push_back( index );
	}	
		
}


std::string path2dir(const std::string &path) {
	// MEMO: find_last_ofが未発見時に-1を返すので、maxをsize_tで実体化したらダメ
	const std::string::size_type pos = std::max<signed>((signed)path.find_last_of('/'), (signed)path.find_last_of('\\'));
	return (pos == std::string::npos) ? std::string()
		: path.substr(0, pos + 1);
}

std::string path2file(const std::string &path) {
	// MEMO: find_last_ofが未発見時に-1を返すので、maxをsize_tで実体化したらダメ
	//       ※一度posに入れているのは、C26451の警告回避のため
	signed pos = std::max<signed>((signed)path.find_last_of('/'), (signed)path.find_last_of('\\')) + 1;
	return path.substr((size_t)pos);
}



bool	is_digit_string( std::string &in_str , bool* is_priod )
{
	std::istringstream in(in_str);
	 char c;
	
	if ( is_priod != NULL )*is_priod = false;

	while (in)
	{
		in.get(c);
		if ( c < 0 ) return false;

		if ( c =='.' )
		{
			if ( is_priod != NULL )*is_priod = true;
		}
		if ( !(isdigit( c ) || c =='.' || c=='-' || c=='+' ) )
		{
			return false;
		}
	}
	return true;
}


std::string getFullPath( const std::string& basePath , const std::string &relPath )
{
#ifdef _WIN32
	char	curPath[_MAX_PATH];
	char	buffer_[_MAX_PATH];
	std::string basePathFs;
	std::string relPathFs;
	if( basePath.size() > 0 )
		basePathFs = SsCharConverter::convert_path_string( basePath );
	if( relPath.size() > 0 )
		relPathFs = SsCharConverter::convert_path_string( relPath );

	_getcwd( curPath , _MAX_PATH ); 
	_chdir( basePathFs.c_str() );

	_fullpath( buffer_ , relPathFs.c_str() , _MAX_PATH );
	
	_chdir( curPath );

	std::string temp( buffer_ );
	temp+= "\\";
	// MEMO: Windowsの場合、上記API群で返ってくるパスのエンコードはSJISなことに注意
	std::string rv = SsCharConverter::sjis_to_utf8( temp );

	return rv;
#else
	char	buffer_[2048];
	char	curPath[2048];
	char	curPath2[2048];
    getwd(curPath);
    
    if ( basePath[0]!='/')
    {
        std::string str = curPath;
        str+="/";
        str+= basePath;
        realpath( str.c_str() , curPath2 );
        chdir( curPath2 );
        std::string temp = relPath;
        realpath(temp.c_str(),buffer_ );
        //std::string ret_str = relPath + "/";

#if 0	// MEMO: エンバグしていると困るので、一応まだ残しておく
#else
        chdir(curPath);
#endif
        return relPath;
    }else{
        chdir( basePath.c_str());
        std::string temp = basePath + relPath;
        realpath(temp.c_str(),buffer_ );
    }
        
    
    chdir(curPath);
    std::string ret_temp;
    ret_temp = buffer_;
    ret_temp+="/";
    return ret_temp;
#endif

}
std::string Replace( std::string String1, std::string String2, std::string String3 )
{
    std::string::size_type  Pos( String1.find( String2 ) );

    while( Pos != std::string::npos )
    {
        String1.replace( Pos, String2.length(), String3 );
        Pos = String1.find( String2, Pos + String3.length() );
    }

    return String1;
}

std::string nomarizeFilename( std::string str )
{
	std::string ret = str;
	ret = Replace( str , "\\\\" , "\\" );

	return ret;
}

bool checkFileVersion(std::string fileVersion, std::string nowVersion)
{
	fileVersion =  Replace(fileVersion, ".", "" );
	nowVersion  =  Replace(nowVersion , ".", "" );

	bool ret = true;
	int fv = std::stoi(fileVersion);	//確認するファイルのバージョン
	int nv = std::stoi(nowVersion);		//現在のバージョン

	if (fv < nv)
	{
		ret = false;
	}

	return ret;
}


}	// namespace spritestudio6
