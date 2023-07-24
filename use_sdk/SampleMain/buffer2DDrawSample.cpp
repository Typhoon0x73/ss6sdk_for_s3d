/**
*
* このファイルは .sspj に含まれるアニメーションから、矩形切り抜きによるパーツをBuffer2Dを利用して描画するサンプルです。
*
* 使い方.
* project_base_path こちらの変数に読み込む .sspj ファイルなどが含まれる相対パスを追記します。
* sspj_file_name　こちらの変数へ読み込む .sspj ファイル名を指定します。
* animation_pack_name こちらの変数へ利用するアニメーションパック名を指定します。
* animation_name　こちらの変数へ利用するアニメーション名を指定します。
* デフォルトではサンプルデータのRingo.sspjを読み込みます。
*
* 左右キーで登録されているメッシュを切り替えます。
* 頂点をドラッグで移動させることができます。
* 
* 以上
*
*/

# include "__SampleDefine.h"

# if defined (BUFFER2D_DRAW_SAMPLE_CPP)

# include <Siv3D.hpp> // OpenSiv3D v0.6.6
# include "../ThirdParty/ss6sdk/Common/Loader/ssloader_sspj.h"

using namespace spritestudio6;

namespace s3d
{
	/// @brief プロジェクト素材管理
	struct ProjectResource
	{
		/// @brief プロジェクトに登録されている画像を読み込み、ファイル名でマップへ保存
		/// @param proj プロジェクトポインタ
		/// @param proj_path プロジェクトファイルまでのパス
		/// @return true : 成功, false : 失敗
		bool loadCellMapTextures(SsProject* proj, FilePathView proj_path)
		{
			for (const auto& cellmap : proj->getCellMapList())
			{
				String image_path = Unicode::FromUTF8(cellmap->imagePath);
				Texture texture(FileSystem::PathAppend(proj_path, image_path));
				if (texture.isEmpty())
				{
					return false;
				}
				texture_map.insert(std::make_pair(image_path, texture));
			}
			return true;
		}

		/// @brief 画像の取得
		/// @param image_path ファイル名
		/// @return 画像 or なければ none を返す
		Optional<Texture> getTexture(StringView image_path)
		{
			auto texture = texture_map.find(image_path);
			if (texture == texture_map.end())
			{
				return none;
			}
			return (texture->second);
		}

		HashTable<String, Texture> texture_map;
	};

	/// @brief 基底モーション
	struct AttributeMotionBase
	{
		AttributeMotionBase() = default;
		virtual ~AttributeMotionBase() = default;

		/// @brief モーションの更新
		/// @param delta 前フレーム更新に要した時間(deltaTime)
		/// @param frame 現在のアニメーションフレーム番号
		virtual void update(double delta, int32 frame) = 0;

		/// @brief キーフレーム情報の設定
		/// @param attrib キーフレーム情報
		virtual void setAttribute(SsAttribute* attrib)
		{
			if (attrib == nullptr) return;
			attribute = attrib;
		}

		/// @brief キーフレーム情報
		SsAttribute* attribute{ nullptr };
	};

	/// @brief floatのモーション
	struct FloatAttributeMotion : public AttributeMotionBase
	{
		FloatAttributeMotion() = default;
		virtual ~FloatAttributeMotion() override = default;

		/// @brief モーションの更新
		/// @param delta 前フレーム更新に要した時間(deltaTime)
		/// @param frame 現在のアニメーションフレーム番号
		virtual void update(double delta, int32 frame) override
		{
			if (attribute == nullptr) return;
			if (attribute->isEmpty()) return;
			if (frame < 0) return;

			auto tmp_l_value = attribute->findLeftKey(frame);
			if (tmp_l_value == nullptr) return;
			float start_value = 0.0f;
			if (tmp_l_value->value.type == SsKeyValueType::_int)   start_value = static_cast<float>(tmp_l_value->value._int);
			if (tmp_l_value->value.type == SsKeyValueType::_float) start_value = tmp_l_value->value._float;

			value = start_value;
			auto tmp_r_value = attribute->findRightKey(frame);
			if (tmp_r_value == nullptr) return;
			float end_value = 0.0f;
			if (tmp_r_value->value.type == SsKeyValueType::_int)   end_value = static_cast<float>(tmp_r_value->value._int);
			if (tmp_r_value->value.type == SsKeyValueType::_float) end_value = tmp_r_value->value._float;
			value = SsInterpolate(tmp_l_value->ipType, static_cast<float>(delta), start_value, end_value, &tmp_l_value->curve);
		}

		/// @brief モーション値
		float value{ 0.0f };
	};

	/// @brief 切り抜き用情報ペア
	struct CellTexture
	{
		Texture texture;
		RectF   pattern;
	};

	/// @brief パーツのモーション管理
	struct PartMotion
	{
		/// @brief モーションの更新
		/// @param delta 前フレーム更新に要した時間(deltaTime)
		/// @param frame 現在のアニメーションフレーム番号
		void update(double delta, int32 frame)
		{
			for (auto& motion : attribute_motions)
			{
				motion->update(delta, frame);
			}
		}

		/// @brief モーションの設定
		/// @param proj プロジェクト
		/// @param pack アニメーションパック
		/// @param anim アニメーション
		/// @param resource プロジェクトリソース
		void setMotion(SsProject* proj, SsAnimePack* pack, SsPartAnime* anim, ProjectResource* resource)
		{
			// すべて必要なのでどれか一つでもなければ早期リターン
			if (proj == nullptr
				|| pack == nullptr
				|| anim == nullptr
				|| resource == nullptr)
				return;

			// パーツ名、情報の登録
			part_name = Unicode::FromUTF8(anim->partName);
			for (const auto& part : pack->Model.partList)
			{
				if (part_name.compare(Unicode::FromUTF8(part->name)) != 0)
				{
					continue;
				}
				part_info = part;
				break;
			}

			// 各キーフレームモーション値の取得
			for (auto& attribute : anim->attributes)
			{
				switch (attribute->tag)
				{
				case SsAttributeKind::cell:
				{
					cell_texture = std::make_unique<CellTexture>();
					SsRefCell ref_cell;
					GetSsRefCell(attribute->firstKey(), ref_cell);
					SsCellMap* cellmap = proj->getCellMap(ref_cell.mapid);
					if (auto texture = resource->getTexture(Unicode::FromUTF8(cellmap->imagePath)))
					{
						cell_texture->texture = texture.value();
					}
					for (const auto& cell : cellmap->cells)
					{
						if (cell->name.compare(ref_cell.name) != 0)
						{
							continue;
						}
						cell_texture->pattern.set(cell->pos.x, cell->pos.y, cell->size.x, cell->size.y);
						if (cell->ismesh)
						{
							Array<Vertex2D> vertices;
							Array<TriangleIndex> indices;
							int32 tw = cell_texture->texture.width();
							int32 th = cell_texture->texture.height();
							for (const auto& point : cell->meshPointList)
							{
								Vertex2D vertex{
									.pos{
										(Scene::CenterF().x - cell_texture->pattern.w * 0.5f) + point.x,
										(Scene::CenterF().y - cell_texture->pattern.h * 0.5f) + point.y
									},
									.tex{
										(cell_texture->pattern.x + point.x) / tw,
										(cell_texture->pattern.y + point.y) / th
									},
									.color{ Float4{ 1.0f, 1.0f, 1.0f, 1.0f } }
								};
								vertices.emplace_back(vertex);
							}
							for (const auto& triangle : cell->meshTriList)
							{
								TriangleIndex ti{
									.i0 = static_cast<uint16>(triangle.idxPo1),
									.i1 = static_cast<uint16>(triangle.idxPo2),
									.i2 = static_cast<uint16>(triangle.idxPo3)
								};
								indices.emplace_back(ti);
							}
							mesh2d.reset(new Buffer2D{ vertices, indices });
						}
						break;
					}
				}   break;
				case SsAttributeKind::posx:
					motion_pos_x.setAttribute(attribute);
					break;
				case SsAttributeKind::posy:
					motion_pos_y.setAttribute(attribute);
					break;
				case SsAttributeKind::rotz:
					motion_rot_z.setAttribute(attribute);
					break;
				default:
					break;
				}
			}
		}

		/// @brief 親の設定
		/// @param motions モーション一覧
		void setParent(Array<std::unique_ptr<PartMotion>>& motions)
		{
			// 自分の情報がなければ早期リターン
			if (part_info == nullptr) return;

			// 親を見つけ次第終了
			for (const auto& motion : motions)
			{
				if (motion->part_info->arrayIndex != part_info->parentIndex)
				{
					continue;
				}
				parent_motion = motion.get();
				break;
			}
		}

		/// @brief 親の位置を含むX座標の取得（ワールド座標）
		/// @return X座標
		float getPosX() const
		{
			float pos_x = motion_pos_x.value;
			if (parent_motion)
			{
				pos_x += parent_motion->getPosX();
			}
			return pos_x;
		}

		/// @brief 親の位置を含むY座標の取得（ワールド座標）
		/// @return Y座標
		float getPosY() const
		{
			float pos_y = motion_pos_y.value;
			if (parent_motion)
			{
				pos_y += parent_motion->getPosY();
			}
			return pos_y;
		}

		String part_name{ U"" };
		SsPart* part_info{ nullptr };
		PartMotion* parent_motion{ nullptr };
		std::unique_ptr<CellTexture> cell_texture{ nullptr };
		std::unique_ptr<Buffer2D> mesh2d{ nullptr };

		FloatAttributeMotion motion_pos_x{};
		FloatAttributeMotion motion_pos_y{};
		FloatAttributeMotion motion_rot_z{};

		Array<AttributeMotionBase*> attribute_motions{
			&motion_pos_x,
			&motion_pos_y,
			&motion_rot_z,
		};
	};

	/// @brief モーション管理
	struct MotionController
	{
		/// @brief モーションの更新
		/// @param delta 前フレーム更新に要した時間(deltaTime)
		void update(double delta)
		{
			motion_time += delta;
			const int32 frame = static_cast<int32>(motion_time / frame_time_sample);
			for (const auto& motion : motions)
			{
				motion->update(delta, frame);
			}
			if (settings == nullptr) return;
			if (is_loop && frame > settings->endFrame)
			{
				timeResetByFrame(settings->startFrame);
			}
		}

		/// @brief アニメーション時間のリセット
		/// @param t リセットする時間(デフォルト:0)
		void timeReset(double t = 0.0)
		{
			motion_time = t;
		}

		/// @brief アニメーションフレームのリセット
		/// @param t リセットするフレーム(デフォルト:0)
		void timeResetByFrame(int32 frame = 0)
		{
			if (frame < 0) return;
			if (settings && settings->frameCount <= frame) return;
			motion_time = frame * frame_time_sample;
		}

		/// @brief モーションの作成
		/// @param proj プロジェクト
		/// @param pack アニメーションパック
		/// @param anim アニメーション
		/// @param resource プロジェクトリソース
		void createMotions(SsProject* proj, SsAnimePack* pack, SsAnimation* anim, ProjectResource* resource)
		{
			if (proj == nullptr
				|| pack == nullptr
				|| anim == nullptr
				|| resource == nullptr)
				return;

			project = proj;
			anim_pack = pack;
			animation = anim;
			project_resource = resource;

			settings = (anim->overrideSettings ? &anim->settings : &pack->settings);

			if (settings->fps > 0)
			{
				frame_time_sample = 1.0 / static_cast<double>(settings->fps);
			}

			for (const auto& part_anim : animation->partAnimes)
			{
				auto motion = std::make_unique<PartMotion>();
				motion->setMotion(project, anim_pack, part_anim, project_resource);
				motions.push_back(std::move(motion));
			}
			for (auto& motion : motions)
			{
				motion->setParent(motions);
			}
		}

		/// @brief モーションのループ設定
		/// @param enable true : ループ, false : ループしない
		void setLoop(bool enable)
		{
			is_loop = enable;
		}

		SsProject* project{ nullptr };
		SsAnimePack* anim_pack{ nullptr };
		SsAnimation* animation{ nullptr };
		ProjectResource* project_resource{ nullptr };
		SsAnimationSettings* settings{ nullptr };

		Array<std::unique_ptr<PartMotion>> motions;

		bool is_loop{ false };
		double motion_time{ 0.0 };
		double frame_time_sample{ 1.0 / 60.0 };
	};
}

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

void Main()
{
	Scene::SetBackground(Palette::Darkslateblue);

	// プロジェクトデータの読み込み
	std::unique_ptr<SsProject> ss_proj = nullptr;
	FilePath project_base_path = U"ss6sample/Ringo/";
	FilePath sspj_file_name = U"Ringo.sspj";
	ss_proj.reset(load_sspj(FileSystem::PathAppend(project_base_path, sspj_file_name)));
	if (ss_proj == nullptr) return;

	// プロジェクトリソースの読み込み
	ProjectResource proj_resource;
	proj_resource.loadCellMapTextures(ss_proj.get(), project_base_path);

	// アニメーションパックの取得
	SsString animation_pack_name = "Ringo";
	SsAnimePack* animation_pack = ss_proj->findAnimationPack(animation_pack_name);
	if (animation_pack == nullptr) return;

	// アニメーションの取得
	SsString animation_name = "attack1";
	SsAnimation* animation = animation_pack->findAnimation(animation_name);
	if (animation == nullptr) return;

	// アニメーションコントローラーの作成
	s3d::MotionController attack_1_controller;
	attack_1_controller.createMotions(ss_proj.get(), animation_pack, animation, &proj_resource);

	// メッシュパーツのみを取得
	Array<PartMotion*> meshs;
	for (const auto& motion : attack_1_controller.motions)
	{
		if (motion->mesh2d)
		{
			meshs.push_back(motion.get());
		}
	}

	// 移動用
	Float2* forcus = nullptr;

	// 描画中のメッシュ番号
	size_t drawIndex = 0;

	// ゲームループ
	while (System::Update())
	{
		// 左右キーでメッシュの選択
		if (KeyLeft.down())  drawIndex--;
		if (KeyRight.down()) drawIndex++;

		if (drawIndex < 0)
		{
			drawIndex = static_cast<int32>(meshs.size()) - 1;
		}
		drawIndex %= meshs.size();

		if (auto& m = meshs[drawIndex])
		{
			// メッシュの描画
			m->mesh2d->draw(m->cell_texture->texture);

			// 各頂点の更新&描画
			if (forcus == nullptr)
			{
				for (auto& v : m->mesh2d->vertices)
				{
					Circle c{ v.pos.x, v.pos.y, 3 };
					if (c.leftClicked())
					{
						forcus = &v.pos;
					}
					c.drawFrame(1.0, (c.mouseOver() ? Palette::Orange : Palette::White));
				}
			}
			else
			{
				forcus->set(Cursor::PosF());
				Circle c{ forcus->x, forcus->y, 3 };
				if (c.leftReleased())
				{
					forcus = nullptr;
				}
				c.drawFrame(1.0, Palette::Orange);
			}
		}
	}
}

# endif // !defined(BUFFER2D_DRAW_SAMPLE_CPP)
