/**
* 
* このテストファイルは各サンプルファイルの .sspj の読み込み時間を計測するためのテストです。
* 
*/


# include "__TestDefine.h"

# if defined(SSPJ_LOADING_TIME_TEST_CPP)

# include <Siv3D.hpp> // OpenSiv3D v0.6.6
# include "../ThirdParty/ss6sdk/common/loader/ssloader_sspj.h"

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
	// 各サンプルファイル
	constexpr const char32_t* sspj_files[] =
	{
		U"ss6sample/animation_template/character_sample1/character_sample1.sspj",
		U"ss6sample/animation_template/character_template1/character_template1.sspj",

		U"ss6sample/AnimeMaking/AnimeMaking.sspj",

		U"ss6sample/basic_samples/Antarctic/AntarcticProject.sspj",
		U"ss6sample/basic_samples/Card/CardProject.sspj",
		U"ss6sample/basic_samples/Comipo/ComipoProject.sspj",
		U"ss6sample/basic_samples/Effect/EffectProject.sspj",
		U"ss6sample/basic_samples/Mask/Mask.sspj",
		U"ss6sample/basic_samples/MeshBone/Knight.sspj",
		U"ss6sample/basic_samples/Robo/RoboProject.sspj",

		U"ss6sample/chara_box/box_00_00.sspj",

		U"ss6sample/example_ui_button/button/original/button.sspj",
		U"ss6sample/example_ui_button/button/packed_texture/button.sspj",
		U"ss6sample/example_ui_button/effect/click.sspj",

		U"ss6sample/ParticleEffect/ParticleEffect.sspj",
		U"ss6sample/ParticleEffect_ACTMV/ParticleEffect_ACTMV.sspj",

		U"ss6sample/SPR/spr_00_00.sspj",

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
