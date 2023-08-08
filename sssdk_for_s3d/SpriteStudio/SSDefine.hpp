#pragma once
#ifndef SS_DEFINE_H_
#define SS_DEFINE_H_


namespace sssdk
{
	enum class SortMode
	{
		prio,
		z,
	};

	enum class ModelPartType
	{
		null,
		normal,
		shape,
		text,
		nines,
		instance,
		armature,
		effect,
		mesh,
		movenode,
		constraint,
		mask,
		joint,
		bonepoint,
		transform_constraint,
		camera,
		audio,
	};

	enum class ModelPartBoundsType
	{
		none,
		quad,
		aabb,
		circle,
		circle_scale_min,
		circle_scale_max,
	};

	enum class ModelPartInheritType
	{
		parant,
		self,
	};

	enum class ColorBlendTarget
	{
		invalid = -1,
		whole,
		vertex,
	};

	enum ATTRIBUTE_KIND
	{
		ATTRIBUTE_KIND_CELL, //!< [CELL]参照セル
		ATTRIBUTE_KIND_POSX, //!< [POSX]位置.X
		ATTRIBUTE_KIND_POSY, //!< [POSY]位置.Y
		ATTRIBUTE_KIND_POSZ, //!< [POSZ]位置.Z
		ATTRIBUTE_KIND_ROTX, //!< [ROTX]回転.X
		ATTRIBUTE_KIND_ROTY, //!< [ROTY]回転.Y
		ATTRIBUTE_KIND_ROTZ, //!< [ROTZ]回転.Z
		ATTRIBUTE_KIND_SCLX, //!< [SCLX]スケール.X
		ATTRIBUTE_KIND_SCLY, //!< [SCLY]スケール.Y
		ATTRIBUTE_KIND_LSCX, //!< [LSCX]ローカルスケール.X
		ATTRIBUTE_KIND_LSCY, //!< [LSCY]ローカルスケール.Y
		ATTRIBUTE_KIND_ALPH, //!< [ALPH]不透明度
		ATTRIBUTE_KIND_LALP, //!< [LALP]ローカル不透明度
		ATTRIBUTE_KIND_PRIO, //!< [PRIO]優先度
		ATTRIBUTE_KIND_FLPH, //!< [FLPH]左右反転(セルの原点を軸にする)
		ATTRIBUTE_KIND_FLPV, //!< [FLPV]上下反転(セルの原点を軸にする)
		ATTRIBUTE_KIND_HIDE, //!< [HIDE]非表示
		ATTRIBUTE_KIND_PCOL, //!< [PCOL]パーツカラー
		ATTRIBUTE_KIND_VCOL, //!< [VCOL]カラーブレンド
		ATTRIBUTE_KIND_SHDR, //!< [SHDR]シェーダー
		ATTRIBUTE_KIND_VERT, //!< [VERT]頂点変形
		ATTRIBUTE_KIND_PVTX, //!< [PVTX]原点オフセット.X
		ATTRIBUTE_KIND_PVTY, //!< [PVTY]原点オフセット.Y
		ATTRIBUTE_KIND_ANCX, //!< [ANCX]アンカーポイント.X
		ATTRIBUTE_KIND_ANCY, //!< [ANCY]アンカーポイント.Y
		ATTRIBUTE_KIND_SIZX, //!< [SIZX]表示サイズ.X
		ATTRIBUTE_KIND_SIZY, //!< [SIZY]表示サイズ.Y
		ATTRIBUTE_KIND_IFLH, //!< [IFLH]イメージ左右反転(常にイメージの中央を原点とする)
		ATTRIBUTE_KIND_IFLV, //!< [IFLV]イメージ上下反転(常にイメージの中央を原点とする)
		ATTRIBUTE_KIND_UVTX, //!< [UVTX]UVアニメ.移動.X
		ATTRIBUTE_KIND_UVTY, //!< [UVTY]UVアニメ.移動.Y
		ATTRIBUTE_KIND_UVRZ, //!< [UVRZ]UVアニメ.回転
		ATTRIBUTE_KIND_UVSX, //!< [UVSX]UVアニメ.スケール.X
		ATTRIBUTE_KIND_UVSY, //!< [UVSY]UVアニメ.スケール.Y
		ATTRIBUTE_KIND_BNDR, //!< [BNDR]当たり判定用の半径
		ATTRIBUTE_KIND_MASK, //!< [MASK]マスク閾値
		ATTRIBUTE_KIND_USER, //!< [USER]ユーザーデータ
		ATTRIBUTE_KIND_SIGN, //!< [SIGN]シグナル
		ATTRIBUTE_KIND_IPRM, //!< [IPRM]インスタンスパーツパラメータ
		ATTRIBUTE_KIND_EFCT, //!< [EFCT]エフェクトパラメータ
		ATTRIBUTE_KIND_DEFM, //!< [DEFM]デフォーム用パラメータ
		ATTRIBUTE_KIND_ADIO, //!< [ADIO]sound用パラメータ
		ATTRIBUTE_KIND_TCHG, //!< [TCHG]テクスチャー変更

		ATTRIBUTE_KIND_NUM
	};

	constexpr const char32_t* ATTRIBUTE_KIND_TAG_NAMES[] = {
		U"CELL",
		U"POSX",
		U"POSY",
		U"POSZ",
		U"ROTX",
		U"ROTY",
		U"ROTZ",
		U"SCLX",
		U"SCLY",
		U"LSCX",
		U"LSCY",
		U"ALPH",
		U"LALP",
		U"PRIO",
		U"FLPH",
		U"FLPV",
		U"HIDE",
		U"PCOL",
		U"VCOL",
		U"SHDR",
		U"VERT",
		U"PVTX",
		U"PVTY",
		U"ANCX",
		U"ANCY",
		U"SIZX",
		U"SIZY",
		U"IFLH",
		U"IFLV",
		U"UVTX",
		U"UVTY",
		U"UVRZ",
		U"UVSX",
		U"UVSY",
		U"BNDR",
		U"MASK",
		U"USER",
		U"SIGN",
		U"IPRM",
		U"EFCT",
		U"DEFM",
		U"ADIO",
		U"TCHG",
	};

	enum DRAW_MODE
	{
		DRAW_MODE_MIX      , //!< ブレンド
		DRAW_MODE_MUL      , //!< 乗算
		DRAW_MODE_ADD      , //!< 加算
		DRAW_MODE_SUB      , //!< 減算
		DRAW_MODE_MUL_ALPHA, //!< α乗算
		DRAW_MODE_SCREEN   , //!< スクリーン
		DRAW_MODE_EXCLUSION, //!< 除外
		DRAW_MODE_INVERT   , //!< 反転
	};
	constexpr const char32_t* DRAW_MODE_TAG_NAMES[] = {
		U"mix",
		U"mul",
		U"add",
		U"sub",
		U"mulalpha",
		U"screen",
		U"exclusion",
		U"invert",
	};

	enum class InterpolationType
	{
		invalid = -1,
		none,			//!< なし
		linear,			//!< 線形
		hermite,		//!< エルミート
		bezier,			//!< ベジェ
		acceleration,	//!< 加速度
		deceleration,	//!< 減速度

		easeIn,
		easeOut,
		easeInOut,

		easeExponentialIn,
		easeExponentialOut,
		easeExponentialInOut,

		easeSineIn,
		easeSineOut,
		easeSineInOut,

		easeElasticIn,
		easeElasticOut,
		easeElasticInOut,

		easeBounceIn,
		easeBounceOut,
		easeBounceInOut,

		easeBackIn,
		easeBackOut,
		easeBackInOut,
	};
	constexpr const char32_t* INTERPOLATION_TYPE_NAMES[] = {
		U"none",
		U"linear",
		U"hermite",
		U"bezier",
		U"acceleration",
		U"deceleration",
		U"easeIn",
		U"easeOut",
		U"easeInOut",
		U"easeExponentialIn",
		U"easeExponentialOut",
		U"easeExponentialInOut",
		U"easeSineIn",
		U"easeSineOut",
		U"easeSineInOut",
		U"easeElasticIn",
		U"easeElasticOut",
		U"easeElasticInOut",
		U"easeBounceIn",
		U"easeBounceOut",
		U"easeBounceInOut",
		U"easeBackIn",
		U"easeBackOut",
		U"easeBackInOut",
	};

	struct SSLabel
	{
		String name{ U"" };
		int32 time{ 0 };
	};

	struct SSCurve
	{
		float startTime{ 0.0f };
		float startValue{ 0.0f };
		float endTime{ 0.0f };
		float endValue{ 0.0f };
	};

	struct SSCellValue
	{
		uint64 mapId{ 0 };
		String name{ U"" };
	};

	struct SSVertexColorValue
	{
		Color col{ 0, 0, 0, 0 };
		float rate{ 0.0f };
	};

	struct SSPartColorValue
	{
		ColorBlendTarget target{ ColorBlendTarget::invalid };
		DRAW_MODE blendType{ DRAW_MODE_MIX };
		SSVertexColorValue vcol[4]; //[0:LT, 1:RT, 2:LB, 3:RB] or [0:whole]
	};

	struct SSVertexValue
	{
		Float2 leftTop{ Float2::Zero() };
		Float2 rightTop{ Float2::Zero() };
		Float2 leftBottom{ Float2::Zero() };
		Float2 rightBottom{ Float2::Zero() };
	};

	struct SSInstanceParamValue
	{
		int32 infinity{ 0 };
		int32 reverse{ 0 };
		int32 pingpong{ 0 };
		int32 independent{ 0 };
		int32 loopNum{ 0 };
		String startLabel{ U"" };
		int32 startOffset{ 0 };
		String endLabel{ U"" };
		int32 endOffset{ 0 };
		float speed{ 0.0f };
	};

	struct SSEffectValue
	{
		int32 startTime{ 0 };
		float speed{ 0.0f };
		int32 independent{ 0 };
	};

	struct SSDeformVertexValue
	{
		int32 vsize{ 0 };
		int32 arraySize{ 0 };
		struct vchg {
			int32 index{ 0 };
			Float2 point{ Float2::Zero() };
		};
		Array<vchg> vchgs;
	};

	struct SSAudioValue
	{
		uint64 soundListId{ 0 };
		String soundName{ U"" };
		int32 loopNum{ 0 };
	};

	struct SSTextureChangeValue
	{
		String name{ U"" };
	};
}

#endif // !SS_DEFINE_H_
