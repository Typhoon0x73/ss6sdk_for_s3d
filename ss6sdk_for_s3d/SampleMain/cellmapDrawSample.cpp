/**
*
* このファイルは .sspj に含まれる .ssce から、矩形切り抜きによるパーツを描画するサンプルです。
*
* 使い方.
* project_base_path こちらの変数に読み込む .sspj ファイルなどが含まれる相対パスを追記します。
* sspj_file_name　こちらの変数へ読み込む .sspj ファイル名を指定します。
* デフォルトではサンプルデータのRingo.sspjを読み込みます。
*
* 以上
*
*/

# include "__SampleDefine.h"

# if defined (CELLMAP_DRAW_SAMPLE_CPP)

# include <Siv3D.hpp> // OpenSiv3D v0.6.6
# include "../ThirdParty/ss6sdk/common/loader/ssloader_sspj.h"

using namespace spritestudio6;

/// @brief .sspjファイルの読み込み
/// @param path 読み込む.sspjファイルパス
/// @return 成功でプロジェクトデータ、失敗でnullptrを返す
SsProject* load_sspj(FilePathView path)
{
	SsProject* ss6project = ssloader_sspj::Load(path.narrow());
	if (ss6project == nullptr)
	{
		Console << U" failed to load file : " << path;
		__debugbreak();
		return nullptr;
	}
	return ss6project;
}

/// @brief セルマップ情報構造体
struct CellMapInfo
{
	/// @brief 切り抜き情報
	struct Pattern
	{
		/// @brief セルマップ名
		String name;

		/// @brief 切り抜き用矩形
		RectF rect;
	};

	/// @brief ssceファイル名
	String file_name;

	/// @brief 描画に利用する画像
	Texture texture;

	/// @brief 画像切り抜き用情報配列
	Array<Pattern> patterns;
};

void Main()
{
	/// @brief セル情報配列
	Array<CellMapInfo> cellmap_infomations;

	// 背景色の変更
	Scene::SetBackground(Palette::Gainsboro);

	// プロジェクトデータの読み込み
	std::unique_ptr<SsProject> ss_proj = nullptr;
	FilePath project_base_path = U"ss6sample/Ringo/";
	FilePath sspj_file_name = U"Ringo.sspj";
	ss_proj.reset(load_sspj(FileSystem::PathAppend(project_base_path, sspj_file_name)));
	if (ss_proj == nullptr) return;

	// セルマップの登録
	for (const auto& cellmap : ss_proj->getCellMapList())
	{
		FilePath texture_file_name = Unicode::FromUTF8(cellmap->imagePath);
		Texture texture{ FileSystem::PathAppend(project_base_path, texture_file_name) };
		if (texture.isEmpty()) return;

		CellMapInfo cellmap_info;
		cellmap_info.file_name = Unicode::FromUTF8(cellmap->fname);
		cellmap_info.texture   = texture;
		for (const auto& cell : cellmap->cells)
		{
			CellMapInfo::Pattern pattern{
				.name = Unicode::FromUTF8(cell->name),
				.rect = RectF{ cell->pos.x, cell->pos.y, cell->size.x, cell->size.y }
			};
			cellmap_info.patterns.emplace_back(pattern);
		}
		cellmap_infomations.emplace_back(cellmap_info);
	}

	int32 draw_cellmap_index = 0;
	int32 draw_pattern_index = 0;
	while (System::Update())
	{
		ClearPrint();
		Print << U"上下矢印キーでセルマップの変更";
		Print << U"左右矢印キーでセルマップ画像パターンの変更";

		if (KeyUp.down())    draw_cellmap_index--;
		if (KeyDown.down())  draw_cellmap_index++;
		if (KeyLeft.down())  draw_pattern_index--;
		if (KeyRight.down()) draw_pattern_index++;

		if (draw_cellmap_index < 0)
		{
			draw_cellmap_index = static_cast<int32>(cellmap_infomations.size()) - 1;
		}
		draw_cellmap_index %= cellmap_infomations.size();
		const auto& select_cellmap = cellmap_infomations[draw_cellmap_index];

		if (draw_pattern_index < 0)
		{
			draw_pattern_index = static_cast<int32>(select_cellmap.patterns.size()) - 1;
		}
		draw_pattern_index %= select_cellmap.patterns.size();
		const auto& select_pattern = select_cellmap.patterns[draw_pattern_index];

		const auto& texture   = select_cellmap.texture;
		const auto& clip_rect = select_pattern.rect;
		texture(clip_rect).drawAt(Scene::CenterF());

		Print << U"セルマップファイル名 : " << select_cellmap.file_name;
		Print << U"セルマップ名 : " << select_pattern.name;
	}
}

# endif //defined (CELLMAP_DRAW_SAMPLE_CPP)
