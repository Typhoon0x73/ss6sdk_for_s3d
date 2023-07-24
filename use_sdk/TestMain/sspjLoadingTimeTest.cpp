/**
* 
* このテストファイルは各サンプルファイルの .sspj の読み込み時間を計測するためのテストです。
*
* 使い方.
* sspj_files こちらの変数に必要なパスを追記し、実行します。
* デフォルトではサンプルデータのRingo.sspjを読み込みます。
*
* 以上
* 
*/


# include "__TestDefine.h"

# if defined(SSPJ_LOADING_TIME_TEST_CPP)

# include <Siv3D.hpp> // OpenSiv3D v0.6.10
# include "../ThirdParty/sssdk/Common/Loader/ssloader_sspj.h"

using namespace spritestudio6;

/// @brief スプライトスタジオのデータを読み込み
/// @param path 読み込むsspjのファイルパス
/// @return true : 成功, false : 失敗
bool load_test(FilePathView path)
{
	// 時間計測用タイマー
	Stopwatch timeWatch(StartImmediately::Yes);

	std::unique_ptr<SsProject> ss6project = nullptr;
	ss6project.reset(ssloader_sspj::Load(path.narrow()));

	if (ss6project == nullptr)
	{
		Console << U" failed to load file : " << path;
		__debugbreak();
		return false;
	}

	Console << FileSystem::FileName(path) << U" loading time : " << timeWatch.sF() << U"sec";
	return true;
}

void Main()
{
	// 計測に影響があるので先に開いておく
	Console.open();

	// 各ファイルパス
	constexpr const char32_t* sspj_files[] =
	{		
		U"ss6sample/Ringo/Ringo.sspj",
	};

	// 各サンプルファイルの読み込み
	for (const auto& path : sspj_files)
	{
		load_test(path);
	}

	while (System::Update())
	{
	}
}

# endif //defined(SSPJ_LOAD_TIME_TEST_CPP)
