#ifndef __SSLOADER_SSAE__
#define __SSLOADER_SSAE__

#include "sstypes.h"
#include "ssarchiver.h"
#include "ssattribute.h"

#define SPRITESTUDIO6_SSAEVERSION "2.00.01"

namespace spritestudio6
{

class SsAnimation;





/// アニメーション再生設定情報です。
class SsAnimationSettings
{
public:
	int						fps;			//!< 再生FPS
	int						frameCount;		//!< フレーム数
	SsPartsSortMode::_enum	sortMode;		//!< パーツのソートモード
	SsPoint2				canvasSize;		//!< キャンバスサイズ(元基準枠)。ビューポートのサイズとイコールではない。
	SsVector2				pivot;			//!< キャンバスの原点。0,0 が中央。-0.5, +0.5 が左上
	int						startFrame;		//!< アニメーションの開始フレーム
	int						endFrame;		//!< アニメーションの終了フレーム

	SsAnimationSettings(){}
	virtual ~SsAnimationSettings(){}
public:

	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( fps );
		SPRITESTUDIO6SDK_SSAR_DECLARE( frameCount );
		SPRITESTUDIO6SDK_SSAR_DECLARE( canvasSize );
		SPRITESTUDIO6SDK_SSAR_DECLARE( pivot );
		SPRITESTUDIO6SDK_SSAR_DECLARE_ENUM(sortMode);
		//タグが存在しない（Ver5.8以前のデータ）場合は初期値を入れる
		if (SPRITESTUDIO6SDK_SSAR_DECLARE(startFrame) == false)
		{
			startFrame = 0;
		}
		if (SPRITESTUDIO6SDK_SSAR_DECLARE(endFrame) == false)
		{
			endFrame = frameCount - 1;
		}
	}
};



//パーツ一つあたりの情報を保持するデータです。
class	SsPart
{
public:
	SsString				name;
	int						arrayIndex;		//!< ツリーを配列に展開した時のインデックス
	int						parentIndex;	//!< 親パーツのインデックス
	SsPartType::_enum		type;			//!< 種別
	SsBoundsType::_enum		boundsType;		//!< 当たり判定として使うか？使う場合はその形状。
	SsInheritType::_enum	inheritType;	//!< アトリビュート値の継承方法
	SsBlendType::_enum		alphaBlendType;	//!< αブレンドの演算式
	int						show;			//!< [編集用データ] パーツの表示・非常時
	int						locked;			//!< [編集用データ] パーツのロック状態
	SsString				colorLabel;		//!< カラーラベル
	bool					maskInfluence;	//!< マスクの影響を受けるかどうか

	float					inheritRates[(int)SsAttributeKind::num];	///< 親の値の継承率。SS4との互換性のため残されているが0 or 1

	//参照アニメーション名
	//ツールはともかくランタイムでは勿体ない使い方なので何らか考えること
	//--------------------------------------------------------------------------------------
	//インスタンスパーツパラメータ
	//--------------------------------------------------------------------------------------
	SsString				refAnimePack;	//!< 参照アニメ名
	SsString				refAnime;		//!< 参照アニメ名			

	//--------------------------------------------------------------------------------------
	//エフェクトパーツパラメータ
	//--------------------------------------------------------------------------------------
	SsString				refEffectName;	//!< 割り当てたパーティクル名

	//--------------------------------------------------------------------------------------
	//ボーンパーツパラメータ
	//--------------------------------------------------------------------------------------
	int						boneLength;		//!< ボーンの長さ
	SsVector2				bonePosition;	//!< ボーンの座標
	float					boneRotation;	//!< ボーンの角度

	SsVector2				weightPosition;	//!< ウェイトの位置
	float					weightImpact;	//!< ウェイトの強さ

	//--------------------------------------------------------------------------------------
	//メッシュパーツパラメータ
	//--------------------------------------------------------------------------------------
	int						meshWeightType;	//!< ウェイトの種類[エディタ用]
	int						meshWeightStrong;//!< ウェイトの強さ[エディタ用]

	//--------------------------------------------------------------------------------------
	//コンストレイントパーツパラメータ
	//--------------------------------------------------------------------------------------
	int							IKDepth;		//!< IK深度
	SsIkRotationArrow::_enum	IKRotationArrow;//!< 回転方向

public:
	SsPart() : 
	  name("") , arrayIndex(0), parentIndex(0) , show(0) , locked(0) , maskInfluence(true)
	  {
			refEffectName = "";
			boneLength = 0;
			bonePosition = SsVector2(0,0);
			boneRotation = 0;
			weightPosition = SsVector2(0, 0);
			weightImpact = 0;
			meshWeightType = 0;
			meshWeightStrong = 0;
			IKDepth = 0;
			IKRotationArrow = SsIkRotationArrow::arrowfree;

		
			//memset( inheritRates , 0 , sizeof( float) * SsAttributeKind::num );
			for (int i = 0; i < (int)SsAttributeKind::num ; ++i)
				inheritRates[i] = 1.f;

			//継承率の初期値
			//継承のタグがない場合に、この値が適用される
			// 反転・非表示フラグは継承しない from 旧SS
			//SS5　では初期値は１
/*
			inheritRates[(int)SsAttributeKind::fliph] = 0.f;
			inheritRates[(int)SsAttributeKind::flipv] = 0.f;
			inheritRates[(int)SsAttributeKind::hide] = 0.f;
*/
			// イメージ反転も継承しない 2013.04.03 水 15:08
			inheritRates[(int)SsAttributeKind::imgfliph] = 0.f;
			inheritRates[(int)SsAttributeKind::imgflipv] = 0.f;
	  }
	virtual ~SsPart(){}


	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( name );
		SPRITESTUDIO6SDK_SSAR_DECLARE( arrayIndex );
		SPRITESTUDIO6SDK_SSAR_DECLARE( parentIndex );

		SPRITESTUDIO6SDK_SSAR_DECLARE_ENUM( type );
		SPRITESTUDIO6SDK_SSAR_DECLARE_ENUM( boundsType );
		SPRITESTUDIO6SDK_SSAR_DECLARE_ENUM( inheritType );
		SPRITESTUDIO6SDK_SSAR_DECLARE_ENUM( alphaBlendType );
		SPRITESTUDIO6SDK_SSAR_DECLARE( show );
		SPRITESTUDIO6SDK_SSAR_DECLARE( locked );
		SPRITESTUDIO6SDK_SSAR_DECLARE( colorLabel );
		SPRITESTUDIO6SDK_SSAR_DECLARE( maskInfluence );

		SPRITESTUDIO6SDK_SSAR_DECLARE( refAnimePack );
		SPRITESTUDIO6SDK_SSAR_DECLARE( refAnime );

		SPRITESTUDIO6SDK_SSAR_DECLARE( refEffectName );

		SPRITESTUDIO6SDK_SSAR_DECLARE( boneLength );
		SPRITESTUDIO6SDK_SSAR_DECLARE( bonePosition );
		SPRITESTUDIO6SDK_SSAR_DECLARE( boneRotation );
		SPRITESTUDIO6SDK_SSAR_DECLARE( weightPosition );
		SPRITESTUDIO6SDK_SSAR_DECLARE( weightImpact );
		SPRITESTUDIO6SDK_SSAR_DECLARE( meshWeightType );
		SPRITESTUDIO6SDK_SSAR_DECLARE( meshWeightStrong );
		SPRITESTUDIO6SDK_SSAR_DECLARE( IKDepth );
		SPRITESTUDIO6SDK_SSAR_DECLARE_ENUM( IKRotationArrow );

		//継承率後に改良を実施
		if ( ar->getType() == EnumSsArchiver::in )
		{
			libXML::XMLElement* e = ar->getxml()->FirstChildElement( "ineheritRates" );
			if ( e )
			{
				libXML::XMLElement* ec = e->FirstChildElement();
				while(ec)
				{
					//継承設定の取得
					const char* tag = ec->Value();
					SsAttributeKind::_enum enumattr;

					__StringToEnum_( tag , enumattr );
					inheritRates[(int)enumattr] = (float)atof( ec->GetText() );
					ec = ec->NextSiblingElement();
				}
			}
		}
	}
};

enum 
{
	// MEMO: ssplayer_mesh.hにSSMESHPART_BONEMAXがあるので統合を考えること。
	SSMESHBIND_BONEMAX = 128,
};
class SsMeshBindInfo
{
public:
	int			weight[SSMESHBIND_BONEMAX];
	SsString	boneName[SSMESHBIND_BONEMAX];
	int			boneIndex[SSMESHBIND_BONEMAX];
	SsVector3   offset[SSMESHBIND_BONEMAX];
	int			bindBoneNum;

	SsMeshBindInfo()
	{
		for (int i = 0; i < SSMESHBIND_BONEMAX; i++)
		{
			weight[i] = 0;
			boneName[i] = "";
			boneIndex[i] = 0;
			offset[i] = SsVector3(0, 0, 0);
		}
		bindBoneNum = 0;

	}
	virtual ~SsMeshBindInfo() {}

	void	fromString(SsString str);

};



//メッシュ1枚毎の情報
class  SsMeshBind 
{
public:
	SsString     meshName;

	std::vector<SsMeshBindInfo>   meshVerticesBindArray;

public:
	SsMeshBind() {}
	virtual ~SsMeshBind() {}

	void	loader(ISsXmlArchiver* ar);

	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		loader(ar);
	}


};


//アニメーションを構成するパーツをリスト化したものです。
class SsModel
{
public:
	//MEMO: SsPartとSsMeshBindのポインタはXMLからの生成などの都合、スマートポインタ化していません
	std::vector<SsPart*>	partList;	//!<格納されているパーツのリスト
	SsAnimation*			setupAnimation;	///< 参照するセットアップアニメ

	std::vector<SsMeshBind*> 		meshList;
	std::map<SsString,int>			boneList;		///<何に使っているか調査　要らなければ削除


public:
	SsModel(){}
	virtual~ SsModel()
	{
		for ( std::vector<SsPart*>::iterator itr = partList.begin() ; 
			itr != partList.end() ; itr ++ ) delete (*itr);

		for ( std::vector<SsMeshBind*>::iterator itr = meshList.begin() ; 
			itr != meshList.end() ; itr ++ ) delete (*itr);
	}

	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		std::vector<SsString> tempMeshList;

		SPRITESTUDIO6SDK_SSAR_DECLARE_LIST( partList );
		SPRITESTUDIO6SDK_SSAR_DECLARE( boneList );
		SPRITESTUDIO6SDK_SSAR_DECLARE_LIST( meshList );
		setupAnimation = NULL;
	}
};


typedef std::vector<SsAttribute*>	SsAttributeList;

class SsPartAnime
{
public:
	SsString					partName;
	SsAttributeList				attributes;

public:
	SsPartAnime(){}
	virtual ~SsPartAnime(){
		for ( SsAttributeList::iterator itr = attributes.begin() ; 
			itr != attributes.end() ; itr ++ ) delete (*itr);	
	}

	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( partName );
		SPRITESTUDIO6SDK_SSAR_DECLARE_LISTEX( attributes , "attribute" );
	}
};


/// ラベル。ループ先などに指定する
class SsLabel
{
public:
	SsString	name;	///< 名前 [変数名変更禁止]
	int			time;	///< 設置された時間(フレーム) [変数名変更禁止]

	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( name );
		SPRITESTUDIO6SDK_SSAR_DECLARE( time );
	}

private:

};


class SsAnimation
{
public:
	SsString	name;							/// アニメーションの名称
	bool		overrideSettings;				/// このインスタンスが持つ設定を使いanimePack の設定を参照しない。FPS, frameCount は常に自身の設定を使う。
	SsAnimationSettings			settings;		/// 設定情報 
	std::vector<SsPartAnime*>	partAnimes;		///	パーツ毎のアニメーションキーフレームが格納されるリスト
	std::vector<SsLabel*>		labels;			/// アニメーションが持つラベルのリストです。
	bool						isSetup;		///< セットアップアニメか？

public:
	SsAnimation(){}
	virtual ~SsAnimation()
	{
		for ( std::vector<SsPartAnime*>::iterator itr = partAnimes.begin() ; 
			itr != partAnimes.end() ; itr ++ ) delete (*itr);	
		for ( std::vector<SsLabel*>::iterator itr = labels.begin() ; 
			itr != labels.end() ; itr ++ ) delete (*itr);	
	}

	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( name );
		SPRITESTUDIO6SDK_SSAR_STRUCT_DECLARE( settings );
		SPRITESTUDIO6SDK_SSAR_DECLARE_LISTEX( labels , "value" );
		SPRITESTUDIO6SDK_SSAR_DECLARE_LISTEX( partAnimes , "partAnime" );
		SPRITESTUDIO6SDK_SSAR_DECLARE(isSetup);
	}
};

/**
*@class SsAnimePack
*@brief パーツを組み合わせた構造とその構造を使用するアニメーションを格納するデータです。
パーツの組み合わせ構造をSsModel、Modelを使用するアニメデータをSsAnimationで定義しています。
*/
class SsAnimePack
{
public:
	SsString					version;
	SsAnimationSettings			settings;		//!< 設定情報 
	SsString					name;			//!< アニメーションパック名称
	SsModel						Model;			//!< パーツ情報の格納先
	std::vector<SsString>		cellmapNames;	//!< 使用されているセルマップの名称		
	std::vector<SsAnimation*>	animeList;		//!< 格納されている子アニメーションのリスト

	SsAnimePack(){}
	virtual ~SsAnimePack()
	{
		for ( std::vector<SsAnimation*>::iterator itr = animeList.begin() ; 
			itr != animeList.end() ; itr ++ ) delete (*itr);		
	}

	///シリアライズのための宣言です。
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE_ATTRIBUTE(version);
		SPRITESTUDIO6SDK_SSAR_STRUCT_DECLARE( settings );
		SPRITESTUDIO6SDK_SSAR_DECLARE( name );
		SPRITESTUDIO6SDK_SSAR_STRUCT_DECLARE( Model );
		SPRITESTUDIO6SDK_SSAR_DECLARE( cellmapNames );
		SPRITESTUDIO6SDK_SSAR_DECLARE_LISTEX( animeList , "anime" );

		//モデルにセットアップアニメーションを設定する
		int i;
		int size = (int)animeList.size();
		for (i = 0; i < size; i++)
		{
			SsAnimation* anime = animeList[i];
			if (anime->isSetup != 0)
			{
				Model.setupAnimation = anime;
				break;
			}
		}


	}

	//アニメーション名からアニメーションを取得する
	SsAnimation*	findAnimation(SsString& name);

};

/*
* @class ssloader_ssae
* @brief ssaeファイルをロードするためのクラスです。
*/
class	ssloader_ssae
{
public:
	ssloader_ssae(){}
	virtual ~ssloader_ssae(){}

	///ssaeファイル名を指定しロードが成功したらそのSsAnimePackのポインタを返します。
	static SsAnimePack*	Load(const std::string& filename );

};




}	// namespace spritestudio6


#endif
