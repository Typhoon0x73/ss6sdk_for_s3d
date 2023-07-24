#include <stdio.h>
#include <cstdlib>
#include <time.h>   //時間



#include "../Loader/ssloader.h"
#include "ssplayer_animedecode.h"
#include "ssplayer_matrix.h"
#include "ssplayer_render.h"
#include "ssplayer_effect.h"
#include "ssplayer_effect2.h"
#include "ssplayer_mesh.h"
#include "../Loader/ssInterpolation.h"



namespace spritestudio6
{

//stdでののforeach宣言　

//乱数シードに利用するユニークIDを作成します。
int seedMakeID = 123456;
//エフェクトに与えるシードを取得する関数
//こちらを移植してください。
unsigned int getRandomSeed()
{
	seedMakeID++;	//ユニークIDを更新します。
	//時間＋ユニークIDにする事で毎回シードが変わるようにします。
	unsigned int rc = (unsigned int)time(0) + ( seedMakeID );

	return(rc);
}


SsAnimeDecoder::SsAnimeDecoder() : 
	curAnimeFPS(0),
	curAnimeStartFrame(0), 
	curAnimeEndFrame(0),
	curAnimeTotalFrame(0),
	nowPlatTime(0) ,
	nowPlatTimeOld(0),
	curCellMapManager(),
	partState(),
	instancePartsHide(false),
	seedOffset(0),
	maskFuncFlag(true),
	maskParentSetting(true),
	meshAnimator()
	{
	}


void	SsAnimeDecoder::reset()
{
	SPRITESTUDIO6SDK_foreach( std::list<SsPartState*> , sortList , e )
	{
		SsPartState* state = (*e);
		if ( state->refEffect )
		{
			state->reset();
			state->refEffect->setSeed(getRandomSeed());
			state->refEffect->reload();
			state->refEffect->stop();
			//state->refEffect->reset();
		}
	}
}

void	SsAnimeDecoder::restart()
{
#if 0
	SPRITESTUDIO6SDK_foreach( std::list<SsPartState*> , sortList , e )
	{
		SsPartState* state = (*e);
		if ( state->refEffect )
		{
			state->refEffect->setSeed(getRandomSeed());
			state->refEffect->reload();
			state->refEffect->stop();
		}
	}
#endif

}

bool	SsAnimeDecoder::getFirstCell(SsPart* part , SsCellValue& out)
{
	bool	retFlag = false;

	SsPartAnime* setupAnime = setupPartAnimeDic[part->name];
	if (setupAnime && !setupAnime->attributes.empty())
	{
		SsAttributeList attList;
		attList = setupAnime->attributes;

		SPRITESTUDIO6SDK_foreach(SsAttributeList, attList, e)
		{
			SsAttribute* attr = (*e);
			switch (attr->tag)
			{
				case SsAttributeKind::cell:		///< 参照セル
				{
					SsGetKeyValue(part, 0, attr, out);
					retFlag = true;
				}
				break;
				default:
					break;
			}
		}

	}

	return retFlag;

}


//void	SsAnimeDecoder::setAnimation(SsModel*	model, SsAnimation* anime, SsAnimePack *animepack, SsCellMapList* cellmap, SsProject* sspj )
void	SsAnimeDecoder::setAnimation( SsModel*	model , SsAnimation* anime , SsCellMapList* cellmap , SsProject* sspj )
{
	//プロジェクト情報の保存
	project = sspj;

	//セルマップリストを取得
	//MEMO: curCellMapManagerは所有・デストラクト時解放しているものの、（同じ実体が何度も割り当ることがあるので）
	//      所有しているセルマップと新規セルマップが同じ時に（先にデストラクタが走って）データを破壊してしまうことから、
	//      指定セルマップが定義されていない時にだけ更新定義していることに注意。
	if(cellmap != curCellMapManager.get())
		curCellMapManager.reset(cellmap);
	curAnimation = anime;

	//partStateをパーツ分作成する
	partAnimeDic.clear();
	setupPartAnimeDic.clear();

	myModel = model;

	//パーツの数
	size_t panum = anime->partAnimes.size();
	for ( size_t i = 0 ; i < panum ; i++ )
	{
		SsPartAnime* panime = anime->partAnimes[i];
		partAnimeDic[panime->partName] = panime;
	}
	//セットアップデータの作成
	if (model->setupAnimation)
	{
		panum = model->setupAnimation->partAnimes.size();
		for (size_t i = 0; i < panum; i++)
		{
			SsPartAnime* panime = model->setupAnimation->partAnimes[i];
			setupPartAnimeDic[panime->partName] = panime;
		}
	}

	//パーツとパーツアニメを関連付ける
	size_t partNum = model->partList.size();

	partState.reset( new std::vector<SsPartState>(partNum) );
	sortList.clear();
	partAnime.clear();
	setupPartAnime.clear();
	partStatesMask_.clear();
	//マスクがあるアニメーションからないアニメーションに切り替えいた場合にdrawで無効なマスクのパーツステートを参照してしまうためクリアを追加
	maskIndexList.clear();	
	stateNum = partNum;

	for ( size_t i = 0 ; i < partNum ; i++ ) 
	{
		SsPart* p = model->partList[i];

		SsPartAndAnime _temp;
		_temp.first = p;
		_temp.second = partAnimeDic[p->name];
		partAnime.push_back( _temp );

		SsPartAndAnime _tempSetup;
		_tempSetup.first = p;
		_tempSetup.second = setupPartAnimeDic[p->name];
		setupPartAnime.push_back(_tempSetup);

		//親子関係の設定
		std::vector<SsPartState>& partStateRaw = *(partState.get());
		if ( p->parentIndex != -1 )
		{
			partStateRaw[i].parent = &partStateRaw[p->parentIndex];
		}else{
			partStateRaw[i].parent = nullptr;
		}
		partStateRaw[i].part = p;

		//継承率の設定
		partStateRaw[i].inheritRates = p->inheritRates;
		partStateRaw[i].index = (int)i;
		partStateRaw[i].partType = p->type;
		partStateRaw[i].maskInfluence = p->maskInfluence && getMaskParentSetting();

		if (sspj)
		{
			//インスタンスパーツの場合の初期設定
			if ( p->type == SsPartType::instance )
			{

				//参照アニメーションを取得
				SsAnimePack* refpack = sspj->findAnimationPack( p->refAnimePack );
				SsAnimation* refanime = refpack->findAnimation( p->refAnime );

				//インスタンスパーツの設定setAnimationでソースアニメになるパーツに適用するので先に設定を行う
				SsAnimeDecoder* animedecoder = new SsAnimeDecoder();
				animedecoder->setMaskFuncFlag( false );					//マスク機能を無効にする
				animedecoder->setMaskParentSetting( p->maskInfluence );	//親のマスク対象を設定する 

				//MEMO: __cellmapはsetAnimation関数内でスマートポインタ化されています
				SsCellMapList* __cellmap = new SsCellMapList();
				__cellmap->set( sspj , refpack );
				animedecoder->setAnimation( &refpack->Model , refanime, __cellmap , sspj );
				partStateRaw[i].refAnime.reset(animedecoder);

				//親子関係を付ける
				std::vector<SsPartState>& partStateRawInstance = *(animedecoder->partState.get());
				partStateRawInstance[0].parent = &partStateRaw[i];
			}

			//エフェクトデータの初期設定
			if ( p->type == SsPartType::effect )
			{
				SsEffectFile* f = sspj->findEffect( p->refEffectName );
				if ( f )
				{
					partStateRaw[i].refEffect.reset( new SsEffectRenderV2() );
					SsEffectRenderV2* er = partStateRaw[i].refEffect.get();

					er->setParentAnimeState( &partStateRaw[i] );
					er->setCellmapManager( this->curCellMapManager.get() );
					er->setEffectData( &f->effectData );
					er->setSeed(getRandomSeed());
					er->reload();
					er->stop();
					er->setLoop(false);
				}
			}

			//マスクパーツの追加
			if (p->type == SsPartType::mask )
			{
				partStatesMask_.push_back( &partStateRaw[i] );
			}

			//メッシュパーツの追加
			if (p->type == SsPartType::mesh)
			{
				partStateRaw[i].meshPart.reset( new SsMeshPart() );
				SsMeshPart* mesh = partStateRaw[i].meshPart.get();

				mesh->myPartState = &partStateRaw[i];
				//使用するセルを調査する
				bool ret;
				SsCellValue cellv;
				if (ret = getFirstCell(p, cellv))
				{
					mesh->targetCell = cellv.cell;
					mesh->targetTexture = cellv.texture;
					mesh->makeMesh();
				}
				else {
					//not found cell
				}
			}
		}

		sortList.push_back( &partStateRaw[i] );
	}

	//アニメの最大フレーム数を取得
	curAnimeStartFrame = anime->settings.startFrame;	//Ver6.0.0開始終了フレーム対応
	curAnimeEndFrame = anime->settings.endFrame;
	curAnimeTotalFrame = anime->settings.frameCount;
	curAnimeFPS = anime->settings.fps;

	//メッシュアニメーションを初期化
	meshAnimator.reset( new SsMeshAnimator() );
	SsMeshAnimator* meshAnimatorRaw = meshAnimator.get();
	meshAnimatorRaw->setAnimeDecoder(this);
	meshAnimatorRaw->makeMeshBoneList();
}

void	SsAnimeDecoder::setSequence( SsSequence* sequence , SsProject* sspj )
{
	//プロジェクト情報の保存
	project = sspj;	

	curSequence = sequence;
}



//頂点変形アニメデータの取得
void	SsAnimeDecoder::SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsVertexAnime& v )
{
	//☆Mapを使っての参照なので高速化必須 
	//todo ロード時に SsVertexAnimeを作成してしまうようにする
	SsVertexAnime	lv;
	SsVertexAnime	rv;

	if ( rightkey == 0 ) //右側が０出会った場合
	{
		GetSsVertexAnime( leftkey , v );
		return ;
	}

	GetSsVertexAnime(leftkey,lv);
	GetSsVertexAnime(rightkey,rv);

	int range = rightkey->time - leftkey->time;
	float now = (float)(time - leftkey->time) / range;

	SsCurve curve;
	curve = leftkey->curve;
	if (leftkey->ipType == SsInterpolationType::bezier)
	{
		// ベジェのみキーの開始・終了時間が必要
		curve.startKeyTime = (float)leftkey->time;
		curve.endKeyTime = (float)rightkey->time;
	}
	
	float rate = SsInterpolate( leftkey->ipType , now , 0.0f , 1.0f , &curve );	
	for ( int i = 0 ; i < 4 ; i++ )
	{
		
		v.offsets[i].x = SsInterpolate( SsInterpolationType::linear , rate , lv.offsets[i].x , rv.offsets[i].x , 0 );	
		v.offsets[i].y = SsInterpolate( SsInterpolationType::linear , rate , lv.offsets[i].y , rv.offsets[i].y , 0 );	
//		v.offsets[i].x = SsInterpolate( leftkey->ipType , now , lv.offsets[i].x , rv.offsets[i].x , &curve );	
//		v.offsets[i].y = SsInterpolate( leftkey->ipType , now , lv.offsets[i].y , rv.offsets[i].y , &curve );	
	}

}


static	float clamp( float v , float min , float max )
{
	float ret = v;

	if ( v < min ) ret = min;
	if ( v > max ) ret = max;

	return ret;

}

void	SsAnimeDecoder::SsInterpolationValue(int time, const SsKeyframe* leftkey, const SsKeyframe* rightkey, SsPartsColorAnime& v)
{
	//☆Mapを使っての参照なので高速化必須
	if (rightkey == 0)
	{
		GetSsPartsColorValue(leftkey, v);
		return;
	}

	SsPartsColorAnime leftv;
	SsPartsColorAnime rightv;

	GetSsPartsColorValue(leftkey, leftv);
	GetSsPartsColorValue(rightkey, rightv);


	SsCurve curve;
	curve = leftkey->curve;
	if (leftkey->ipType == SsInterpolationType::bezier)
	{
		// ベジェのみキーの開始・終了時間が必要
		curve.startKeyTime = (float)leftkey->time;
		curve.endKeyTime = (float)rightkey->time;
	}

	int range = rightkey->time - leftkey->time;
	float now = (float)(time - leftkey->time) / range;

	//初期化しておく
	v.color.rgba.a = 0;
	v.color.rgba.r = 0;
	v.color.rgba.g = 0;
	v.color.rgba.b = 0;
	v.target = SsColorBlendTarget::vertex;
	v.blendType = leftv.blendType;

	now = SsInterpolate(leftkey->ipType, now, 0.0f, 1.0f, &curve);

	if (leftv.target == SsColorBlendTarget::vertex)
	{
		if (rightv.target == SsColorBlendTarget::vertex)
		{
			//両方とも４頂点カラー
			for (int i = 0; i < 4; i++)
			{
				v.colors[i].rate = clamp(SsInterpolate(SsInterpolationType::linear, now, leftv.colors[i].rate, rightv.colors[i].rate, &curve), 0.0f, 1.0f);
				v.colors[i].rgba.a = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.colors[i].rgba.a, (float)rightv.colors[i].rgba.a, &curve), 0.0f, 255.0f));
				v.colors[i].rgba.r = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.colors[i].rgba.r, (float)rightv.colors[i].rgba.r, &curve), 0.0f, 255.0f));
				v.colors[i].rgba.g = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.colors[i].rgba.g, (float)rightv.colors[i].rgba.g, &curve), 0.0f, 255.0f));
				v.colors[i].rgba.b = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.colors[i].rgba.b, (float)rightv.colors[i].rgba.b, &curve), 0.0f, 255.0f));
			}
		}
		else
		{
			//左は４頂点、右は単色
			for (int i = 0; i < 4; i++)
			{
				v.colors[i].rate = clamp(SsInterpolate(SsInterpolationType::linear, now, leftv.colors[i].rate, rightv.color.rate, &curve), 0.0f, 1.0f);
				v.colors[i].rgba.a = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.colors[i].rgba.a, (float)rightv.color.rgba.a, &curve), 0.0f, 255.0f));
				v.colors[i].rgba.r = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.colors[i].rgba.r, (float)rightv.color.rgba.r, &curve), 0.0f, 255.0f));
				v.colors[i].rgba.g = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.colors[i].rgba.g, (float)rightv.color.rgba.g, &curve), 0.0f, 255.0f));
				v.colors[i].rgba.b = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.colors[i].rgba.b, (float)rightv.color.rgba.b, &curve), 0.0f, 255.0f));
			}
		}
	}
	else
	{
		if (rightv.target == SsColorBlendTarget::vertex)
		{
			//左は単色、右は４頂点カラー
			for (int i = 0; i < 4; i++)
			{
				v.colors[i].rate = clamp(SsInterpolate(SsInterpolationType::linear, now, leftv.color.rate, rightv.colors[i].rate, &curve), 0.0f, 1.0f);
				v.colors[i].rgba.a = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.color.rgba.a, (float)rightv.colors[i].rgba.a, &curve), 0.0f, 255.0f));
				v.colors[i].rgba.r = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.color.rgba.r, (float)rightv.colors[i].rgba.r, &curve), 0.0f, 255.0f));
				v.colors[i].rgba.g = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.color.rgba.g, (float)rightv.colors[i].rgba.g, &curve), 0.0f, 255.0f));
				v.colors[i].rgba.b = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.color.rgba.b, (float)rightv.colors[i].rgba.b, &curve), 0.0f, 255.0f));
			}
		}
		else
		{
			//両方とも単色
			v.color.rate = clamp(SsInterpolate(SsInterpolationType::linear, now, leftv.color.rate, rightv.color.rate, &curve), 0.0f, 1.0f);
			v.color.rgba.a = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.color.rgba.a, (float)rightv.color.rgba.a, &curve), 0.0f, 255.0f));
			v.color.rgba.r = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.color.rgba.r, (float)rightv.color.rgba.r, &curve), 0.0f, 255.0f));
			v.color.rgba.g = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.color.rgba.g, (float)rightv.color.rgba.g, &curve), 0.0f, 255.0f));
			v.color.rgba.b = (u32)(clamp(SsInterpolate(SsInterpolationType::linear, now, (float)leftv.color.rgba.b, (float)rightv.color.rgba.b, &curve), 0.0f, 255.0f));
			v.target = SsColorBlendTarget::whole;
		}
	}

}

void	SsAnimeDecoder::SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsColorAnime& v )
{
	//☆Mapを使っての参照なので高速化必須
	if ( rightkey == 0 )
	{
		GetSsColorValue( leftkey , v );
		return ;
	}
	
	SsColorAnime leftv;
	SsColorAnime rightv;

	GetSsColorValue( leftkey , leftv );
	GetSsColorValue( rightkey , rightv );


	SsCurve curve;
	curve = leftkey->curve;
	if (leftkey->ipType == SsInterpolationType::bezier)
	{
		// ベジェのみキーの開始・終了時間が必要
		curve.startKeyTime = (float)leftkey->time;
		curve.endKeyTime = (float)rightkey->time;
	}

	int range = rightkey->time - leftkey->time;
	float now = (float)(time - leftkey->time) / range;

	//初期化しておく
	v.color.rgba.a = 0;	
	v.color.rgba.r = 0;	
	v.color.rgba.g = 0;	
	v.color.rgba.b = 0;	
	v.target = SsColorBlendTarget::vertex;
	v.blendType = leftv.blendType;

	now = SsInterpolate( leftkey->ipType , now , 0.0f , 1.0f , &curve );	

	if ( leftv.target == SsColorBlendTarget::vertex )
	{
		if ( rightv.target == SsColorBlendTarget::vertex )
		{
			//両方とも４頂点カラー
			for ( int i = 0 ; i < 4 ; i++ )
			{
				v.colors[i].rate = clamp( SsInterpolate( SsInterpolationType::linear , now , leftv.colors[i].rate , rightv.colors[i].rate  , &curve ) , 0.0f , 1.0f );	
				v.colors[i].rgba.a = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.colors[i].rgba.a , (float)rightv.colors[i].rgba.a  , &curve ) , 0.0f , 255.0f ));
				v.colors[i].rgba.r = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.colors[i].rgba.r , (float)rightv.colors[i].rgba.r  , &curve ) , 0.0f , 255.0f ));
				v.colors[i].rgba.g = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.colors[i].rgba.g , (float)rightv.colors[i].rgba.g  , &curve ) , 0.0f , 255.0f ));
				v.colors[i].rgba.b = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.colors[i].rgba.b , (float)rightv.colors[i].rgba.b  , &curve ) , 0.0f , 255.0f ));
			}
		}
		else
		{
			//左は４頂点、右は単色
			for ( int i = 0 ; i < 4 ; i++ )
			{
				v.colors[i].rate = clamp( SsInterpolate( SsInterpolationType::linear , now , leftv.colors[i].rate , rightv.color.rate  , &curve ) , 0.0f , 1.0f );	
				v.colors[i].rgba.a = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.colors[i].rgba.a , (float)rightv.color.rgba.a  , &curve ) , 0.0f , 255.0f ));
				v.colors[i].rgba.r = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.colors[i].rgba.r , (float)rightv.color.rgba.r  , &curve ) , 0.0f , 255.0f ));
				v.colors[i].rgba.g = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.colors[i].rgba.g , (float)rightv.color.rgba.g  , &curve ) , 0.0f , 255.0f ));
				v.colors[i].rgba.b = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.colors[i].rgba.b , (float)rightv.color.rgba.b  , &curve ) , 0.0f , 255.0f ));
			}
		}
	}
	else
	{
		if ( rightv.target == SsColorBlendTarget::vertex )
		{
			//左は単色、右は４頂点カラー
			for ( int i = 0 ; i < 4 ; i++ )
			{
				v.colors[i].rate = clamp( SsInterpolate( SsInterpolationType::linear , now , leftv.color.rate , rightv.colors[i].rate  , &curve ) , 0.0f , 1.0f );	
				v.colors[i].rgba.a = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.color.rgba.a , (float)rightv.colors[i].rgba.a  , &curve ) , 0.0f , 255.0f ));
				v.colors[i].rgba.r = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.color.rgba.r , (float)rightv.colors[i].rgba.r  , &curve ) , 0.0f , 255.0f ));
				v.colors[i].rgba.g = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.color.rgba.g , (float)rightv.colors[i].rgba.g  , &curve ) , 0.0f , 255.0f ));
				v.colors[i].rgba.b = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.color.rgba.b , (float)rightv.colors[i].rgba.b  , &curve ) , 0.0f , 255.0f ));
			}
		}
		else
		{
			//両方とも単色
			v.color.rate = clamp( SsInterpolate( SsInterpolationType::linear , now , leftv.color.rate , rightv.color.rate  , &curve ) , 0.0f , 1.0f );	
			v.color.rgba.a = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.color.rgba.a , (float)rightv.color.rgba.a  , &curve ) , 0.0f , 255.0f ));
			v.color.rgba.r = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.color.rgba.r , (float)rightv.color.rgba.r  , &curve ) , 0.0f , 255.0f ));
			v.color.rgba.g = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.color.rgba.g , (float)rightv.color.rgba.g  , &curve ) , 0.0f , 255.0f ));
			v.color.rgba.b = (u32)(clamp( SsInterpolate( SsInterpolationType::linear , now , (float)leftv.color.rgba.b , (float)rightv.color.rgba.b  , &curve ) , 0.0f , 255.0f ));
			v.target = SsColorBlendTarget::whole;
		}
	}

}

void	SsAnimeDecoder::SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsShaderAnime& v )
{
	//☆Mapを使っての参照なので高速化必須
	if ( rightkey == 0 )
	{
		GetSsShaderValue( leftkey , v );
		return ;
	}
	
	SsShaderAnime leftv;
	SsShaderAnime rightv;

	GetSsShaderValue( leftkey , leftv );
	GetSsShaderValue( rightkey , rightv );


	SsCurve curve;
	curve = leftkey->curve;
	if (leftkey->ipType == SsInterpolationType::bezier)
	{
		// ベジェのみキーの開始・終了時間が必要
		curve.startKeyTime = (float)leftkey->time;
		curve.endKeyTime = (float)rightkey->time;
	}

	int range = rightkey->time - leftkey->time;
	float now = (float)(time - leftkey->time) / range;

	//初期化しておく
	v.id = leftv.id;
	v.param[0] = 0.0f;	
	v.param[1] = 0.0f;	
	v.param[2] = 0.0f;	
	v.param[3] = 0.0f;	

	now = SsInterpolate( leftkey->ipType , now , 0.0f , 1.0f , &curve );	

	v.param[0] = SsInterpolate( SsInterpolationType::linear , now , leftv.param[0] , rightv.param[0]  , &curve );
	v.param[1] = SsInterpolate( SsInterpolationType::linear , now , leftv.param[1] , rightv.param[1]  , &curve );
	v.param[2] = SsInterpolate( SsInterpolationType::linear , now , leftv.param[2] , rightv.param[2]  , &curve );
	v.param[3] = SsInterpolate( SsInterpolationType::linear , now , leftv.param[3] , rightv.param[3]  , &curve );

}


void	SsAnimeDecoder::SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsCellValue& v )
{
	SsRefCell cell;
	GetSsRefCell( leftkey , cell );

	getCellValue( this->curCellMapManager.get(), cell.mapid, cell.name, v );
}

void	SsAnimeDecoder::SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsSignalAttr& v )
{
	//補間は行わないので、常に左のキーを出力する
	GetSsSignalAnime( leftkey , v );
}

//インスタンスアニメデータ
void	SsAnimeDecoder::SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsInstanceAttr& v )
{
	//補間は行わないので、常に左のキーを出力する
	GetSsInstparamAnime( leftkey , v );
}

void	SsAnimeDecoder::SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , SsEffectAttr& v )
{
	//補間は行わないので、常に左のキーを出力する
	GetSsEffectParamAnime( leftkey , v );
	
}


void	SsAnimeDecoder::SsInterpolationValue(int time, const SsKeyframe* leftkey, const SsKeyframe* rightkey, SsDeformAttr& v)
{
	v.verticeChgList.clear();

	if (rightkey == 0)
	{
		GetSsDeformAnime(leftkey, v);
		return;
	}

	SsDeformAttr startValue;
	SsDeformAttr endValue;

	GetSsDeformAnime(leftkey, startValue);
	GetSsDeformAnime(rightkey, endValue);

	int range = rightkey->time - leftkey->time;
	float now = (float)(time - leftkey->time) / range;

	SsCurve curve;
	curve = leftkey->curve;
	if (leftkey->ipType == SsInterpolationType::bezier)
	{
		// ベジェのみキーの開始・終了時間が必要
		curve.startKeyTime = (float)leftkey->time;
		curve.endKeyTime = (float)rightkey->time;
	}

	float rate = SsInterpolate(leftkey->ipType, now, 0.0f, 1.0f, &curve);

	//スタートとエンドの頂点数を比較し、多い方に合わせる(足りない部分は0とみなす)
	int numPoints = std::max(static_cast<int>(startValue.verticeChgList.size()), static_cast<int>(endValue.verticeChgList.size()));

	std::vector<SsVector2> start = startValue.verticeChgList;
	//start.resize(numPoints);
	for (int i = (int)(start.size()); i < numPoints; i++)
	{
		start.push_back(SsVector2(0, 0));
	}

	std::vector<SsVector2> end = endValue.verticeChgList;
	//end.resize(numPoints);
	for (int i = (int)(end.size()); i < numPoints; i++)
	{
		end.push_back(SsVector2(0, 0));
	}

	//SsDebugPrint("start : %d, end : %d", start.size(), end.size());

	for (int i = 0; i < numPoints; i++)
	{
		SsVector2 outVec;

		outVec = SsInterpolate(SsInterpolationType::linear, rate, start[i], end[i], 0);
		v.verticeChgList.push_back(outVec);

	}


}


//float , int , bool基本型はこれで値の補間を行う
template<typename mytype>
void	SsAnimeDecoder::SsInterpolationValue( int time , const SsKeyframe* leftkey , const SsKeyframe* rightkey , mytype& v )
{
	if ( rightkey == 0 )
	{
		v = leftkey->value.get<mytype>();
		return ;
	}
	
	float v1 = (float)leftkey->value.get<mytype>();
	float v2 = (float)rightkey->value.get<mytype>();

	int range = rightkey->time - leftkey->time;
	float now = (float)(time - leftkey->time) / range;

	if (leftkey->ipType == SsInterpolationType::bezier)
	{
		// ベジェのみキーの開始・終了時間が必要
		SsCurve curve;
		curve = leftkey->curve;
		curve.startKeyTime = (float)leftkey->time;
		curve.endKeyTime = (float)rightkey->time;
		v = (mytype)(SsInterpolate( leftkey->ipType , now , v1 , v2 , &curve ));
	}
	else{
		v = (mytype)(SsInterpolate( leftkey->ipType , now , v1 , v2 , &leftkey->curve ));
	}

}



template<typename mytype> int	SsAnimeDecoder::SsGetKeyValue(SsPart* part, int time , SsAttribute* attr , mytype&  value )
{
	int	useTime = 0;

	if ( attr->isEmpty() )
	{
		//デフォルト値を入れる まだ未実装
		return useTime;
	}

	const SsKeyframe* lkey = attr->findLeftKey( time );

	//無い場合は、最初のキーを採用する
	if ( lkey == 0 )
	{
		if (curAnimation->isSetup == false)
		{
			//セットアップアニメから先頭キーを取得する
			SsPartAnime* setupAnime = setupPartAnimeDic[part->name];
			if ((setupAnime) && (!setupAnime->attributes.empty()))
			{
				SsAttributeList attList;
				attList = setupAnime->attributes;
				SPRITESTUDIO6SDK_foreach(SsAttributeList, attList, e)
				{
					SsAttribute* setupattr = (*e);
					if (setupattr->tag == attr->tag)
					{
						lkey = setupattr->firstKey();
						break;
					}
				}
			}
		}
		if (lkey == 0 )	//セットアップデータにキーが無い
		{
			lkey = attr->firstKey();	//現在のアニメの先頭キーを設定する
		}

		SsInterpolationValue( time , lkey , 0 , value );

		useTime = lkey->time;
		return useTime;

	}else if ( lkey->time == time )
	{
		SsInterpolationValue( time , lkey , 0 , value );
		useTime = time;
		return useTime;
	}else{
		//補間計算をする
		const SsKeyframe* rkey = attr->findRightKey( time );
		if (rkey == NULL)
		{
			// 次のキーが無いので補間できない。よって始点キーの値をそのまま返す
			SsInterpolationValue( time , lkey , 0 , value );
			useTime = lkey->time;
			return useTime;
		}else
		if (lkey->ipType == SsInterpolationType::none)
		{
			// 補間なし指定なので始点キーの値…
			SsInterpolationValue( time , lkey , 0 , value );
			useTime = lkey->time;
			return useTime ;
		}else
		{
			SsInterpolationValue( time , lkey ,rkey , value );
			useTime = time;
		}
	}
		
	return useTime ;

}


//中間点を求める
static void	CoordinateGetDiagonalIntersection( SsVector2& out , const SsVector2& LU , const SsVector2& RU , const SsVector2& LD , const SsVector2& RD )
{
	out = SsVector2(0.f,0.f);

	/* <<< 係数を求める >>> */
	float c1 = (LD.y - RU.y) * (LD.x - LU.x) - (LD.x - RU.x) * (LD.y - LU.y);
	float c2 = (RD.x - LU.x) * (LD.y - LU.y) - (RD.y - LU.y) * (LD.x - LU.x);
	float c3 = (RD.x - LU.x) * (LD.y - RU.y) - (RD.y - LU.y) * (LD.x - RU.x);


	if ( c3 <= 0 && c3 >=0) return;

	float ca = c1 / c3;
	float cb = c2 / c3;

	/* <<< 交差判定 >>> */
	if(((0.0f <= ca) && (1.0f >= ca)) && ((0.0f <= cb) && (1.0f >= cb)))
	{	/* 交差している */
		out.x = LU.x + ca * (RD.x - LU.x);
		out.y = LU.y + ca * (RD.y - LU.y);
	}
}

static SsVector2 GetLocalScale( float matrix[16] )
{
	float sx = SsVector2::distance( SsVector2( matrix[0] , matrix[1] ) , SsVector2( 0 , 0 ) );
	float sy = SsVector2::distance( SsVector2( matrix[4 * 1 + 0] , matrix[4 * 1 + 1] ) , SsVector2( 0 , 0 ) );

	return SsVector2( sx , sy );
}


///現在の時間からパーツのアトリビュートの補間値を計算する
void	SsAnimeDecoder::updateState( int nowTime , SsPart* part , SsPartAnime* anime , SsPartState* state )
{

	//ステートの初期値を設定
	state->init();
	state->inheritRates = part->inheritRates;

	SsPartAnime* setupAnime = setupPartAnimeDic[part->name];	//セットアップアニメを取得する

	if ( ( anime == 0 ) && ( setupAnime == 0 ) ){
		state->hide = true;
		IdentityMatrix( state->matrix );
		state->hide = true;
		return;
	}

	// 親の継承設定を引用する設定の場合、ここで参照先を親のものに変えておく。
	if (part->inheritType == SsInheritType::parent)
	{
		if ( state->parent )
		{
			state->inheritRates = state->parent->inheritRates;
		}
	}


	bool	size_x_key_find = false;
	bool	size_y_key_find = false;

	state->is_vertex_transform = false;
	state->is_parts_color = false;
	state->is_color_blend = false;
	state->is_shader = false;
	state->alphaBlendType = part->alphaBlendType;

	bool hidekey_find = false;
	bool hideTriger = false;
	state->masklimen = 0;
	state->is_localAlpha = false;
	state->is_defrom = false;

	state->position.x = part->bonePosition.x;
	state->position.y = part->bonePosition.y;
	state->rotation.z = part->boneRotation;

	//セットアップデータをアニメーションデータ
	int idx = 0;
	for (idx = 0; idx < 2; idx++)
	{
		SsAttributeList attList;

		if (idx == 0)
		{
			//セットアップデータで初期化する
			if (!setupAnime)
			{
				continue;
			}
			if (setupAnime->attributes.empty())
			{
				continue;
			}
			attList = setupAnime->attributes;
		}
		else
		{
			//アニメーションデータを解析する
			if (!anime)
			{
				continue;
			}
			if (anime->attributes.empty())
			{
				continue;
			}
			attList = anime->attributes;
		}
		SPRITESTUDIO6SDK_foreach( SsAttributeList , attList , e )
		{
			SsAttribute* attr = (*e);
			switch( attr->tag )
			{
				case SsAttributeKind::invalid:	///< 無効値。旧データからの変換時など
					break;
				case SsAttributeKind::cell:		///< 参照セル
					{
						SsGetKeyValue( part, nowTime , attr , state->cellValue );
						state->noCells = false;
					}
					break;
				case SsAttributeKind::posx:		///< 位置.X
					SsGetKeyValue( part, nowTime , attr , state->position.x );
					break;
				case SsAttributeKind::posy:		///< 位置.Y
					SsGetKeyValue( part, nowTime , attr , state->position.y );
					break;
				case SsAttributeKind::posz:		///< 位置.Z
					SsGetKeyValue( part, nowTime , attr , state->position.z );
					break;
				case SsAttributeKind::rotx:		///< 回転.X
					SsGetKeyValue( part, nowTime , attr , state->rotation.x );
					break;
				case SsAttributeKind::roty:		///< 回転.Y
					SsGetKeyValue( part, nowTime , attr , state->rotation.y );
					break;
				case SsAttributeKind::rotz:		///< 回転.Z
					SsGetKeyValue( part, nowTime , attr , state->rotation.z );
					break;
				case SsAttributeKind::sclx:		///< スケール.X
					SsGetKeyValue( part, nowTime , attr , state->scale.x );
					break;
				case SsAttributeKind::scly:		///< スケール.Y
					SsGetKeyValue( part, nowTime , attr , state->scale.y );
					break;
				case SsAttributeKind::losclx:	///< ローカルスケール.X
					SsGetKeyValue( part, nowTime , attr , state->localscale.x);
					break;
				case SsAttributeKind::loscly:	///< ローカルスケール.X
					SsGetKeyValue( part, nowTime , attr , state->localscale.y);
					break;
				case SsAttributeKind::alpha:	///< 不透明度
					SsGetKeyValue( part, nowTime , attr , state->alpha);
					break;
				case SsAttributeKind::loalpha:	///< ローカル不透明度
					SsGetKeyValue( part, nowTime , attr , state->localalpha);
					state->is_localAlpha = true;
					break;
				case SsAttributeKind::prio:		///< 優先度
					SsGetKeyValue( part, nowTime , attr , state->prio );
					break;
//				case SsAttributeKind::fliph:	///< 左右反転(セルの原点を軸にする) Ver6非対応
//					SsGetKeyValue( part, nowTime , attr , state->hFlip );
//					break;
//				case SsAttributeKind::flipv:	///< 上下反転(セルの原点を軸にする) Ver6非対応
//					SsGetKeyValue( part, nowTime , attr , state->vFlip );
//					break;
				case SsAttributeKind::hide:		///< 非表示
					{
						int useTime = SsGetKeyValue( part, nowTime , attr , state->hide );
						// 非表示キーがないか、先頭の非表示キーより手前の場合は常に非表示にする。
						//セットアップによってhidekey_findがあった場合は強制非表示にしない
						if ( ( useTime > nowTime ) && ( hidekey_find == false ) )
						{
							state->hide = true;
						}
						else
						{
							//非表示キーがあり、かつ最初のキーフレームを取得した
							hidekey_find = true;
						}
					}
					break;
				case SsAttributeKind::partsColor:
					SsGetKeyValue( part, nowTime , attr , state->partsColorValue);
					state->is_parts_color = true;
					break;
//				case SsAttributeKind::color:	///< カラーブレンド  Ver6非対応
//					SsGetKeyValue( part, nowTime , attr , state->colorValue );
//					state->is_color_blend = true;
//					break;
				case SsAttributeKind::shader:	///< シェーダー
					SsGetKeyValue( part, nowTime , attr , state->shaderValue);
					state->is_shader = true;
					break;
				case SsAttributeKind::vertex:	///< 頂点変形
					SsGetKeyValue( part, nowTime , attr , state->vertexValue );
					state->is_vertex_transform = true;
					break;
				case SsAttributeKind::pivotx:	///< 原点オフセット.X
					SsGetKeyValue( part, nowTime , attr , state->pivotOffset.x );
					break;
				case SsAttributeKind::pivoty:	///< 原点オフセット.Y
					SsGetKeyValue( part, nowTime , attr , state->pivotOffset.y );
					break;
//				case SsAttributeKind::anchorx:	///< アンカーポイント.X Ver6非対応
//					SsGetKeyValue( part, nowTime , attr , state->anchor.x );
//					break;
//				case SsAttributeKind::anchory:	///< アンカーポイント.Y Ver6非対応
//					SsGetKeyValue( part, nowTime , attr , state->anchor.y );
//					break;
				case SsAttributeKind::sizex:	///< 表示サイズ.X
					SsGetKeyValue( part, nowTime , attr , state->size.x );
					size_x_key_find = true;
					break;
				case SsAttributeKind::sizey:	///< 表示サイズ.Y
					SsGetKeyValue( part, nowTime , attr , state->size.y );
					size_y_key_find = true;
					break;
				case SsAttributeKind::imgfliph:	///< イメージ左右反転(常にイメージの中央を原点とする)
					SsGetKeyValue( part, nowTime , attr , state->imageFlipH );
					break;
				case SsAttributeKind::imgflipv:	///< イメージ上下反転(常にイメージの中央を原点とする)
					SsGetKeyValue( part, nowTime , attr , state->imageFlipV );
					break;
				case SsAttributeKind::uvtx:		///< UVアニメ.移動.X
					SsGetKeyValue( part, nowTime , attr , state->uvTranslate.x );
					break;
				case SsAttributeKind::uvty:		///< UVアニメ.移動.Y
					SsGetKeyValue( part, nowTime , attr , state->uvTranslate.y );
					break;
				case SsAttributeKind::uvrz:		///< UVアニメ.回転
					SsGetKeyValue( part, nowTime , attr , state->uvRotation );
					break;
				case SsAttributeKind::uvsx:		///< UVアニメ.スケール.X
					SsGetKeyValue( part, nowTime , attr , state->uvScale.x );
					break;
				case SsAttributeKind::uvsy:		///< UVアニメ.スケール.Y
					SsGetKeyValue( part, nowTime , attr , state->uvScale.y );
					break;
				case SsAttributeKind::boundr:	///< 当たり判定用の半径
					SsGetKeyValue( part, nowTime , attr , state->boundingRadius );
					break;
				case SsAttributeKind::user:		///< Ver.4 互換ユーザーデータ
					break;
				case SsAttributeKind::signal:	///< シグナル
					SsGetKeyValue( part, nowTime , attr , state->signalValue );
					break;
				case SsAttributeKind::instance:	///インスタンスパラメータ
					{
						int t = SsGetKeyValue( part, nowTime , attr , state->instanceValue );
						//先頭にキーが無い場合
						if ( t  > nowTime )
						{
							SsInstanceAttr d;
							state->instanceValue = d;
						}
					}
					break;
				case SsAttributeKind::effect:
					{

						int t = SsGetKeyValue( part, nowTime , attr , state->effectValue );

						//先頭にキーが無い場合
						if ( t > nowTime )
						{
							SsEffectAttr d;
							state->effectValue = d;
						}else{
							state->effectTime = t;
							if ( !state->effectValue.attrInitialized )
							{
								state->effectValue.attrInitialized  = true;
								state->effectTimeTotal = (float)(state->effectValue.startTime);
								state->effectTime = t;//state->effectValue.startTime;
							}
						}
					}
					break;
				case SsAttributeKind::mask:
					SsGetKeyValue( part, nowTime, attr, state->masklimen);
					break;
				case SsAttributeKind::deform:
					state->is_defrom = true;
					SsGetKeyValue(part, nowTime, attr, state->deformValue);
					break;

			}
		}
	}

	// カラー値だけアニメが無いと設定されないので初期値を入れておく。
	// alpha はupdateで初期化されるのでOK
	// 当たり判定パーツ用のカラー。赤の半透明にする
	static const float sColorsForBoundsParts[] = {0.5f, 0.f, 0.f, 1.f};
	for (int i = 0; i < (4*4) ; ++i)
	{
		if (state->noCells)
			state->colors[i] = sColorsForBoundsParts[i & 3];
		else
			state->colors[i] = 1.f;
	}

	// 非表示キーがないか、先頭の非表示キーより手前の場合は常に非表示にする。
	// 継承する場合は継承を優先するため先に処理する
	if (!hidekey_find)
	{
		state->hide = true;
	}


	// 継承

    {
		// 継承 SS5
		if (state->parent)
		{
			// α
			if (state->inherits_(SsAttributeKind::alpha))
			{
				state->alpha *= state->parent->alpha;
			}

			//親がインスタンスパーツでかつ非表示フラグがある場合は非表示にする。
			if ( instancePartsHide == true )
			{
				state->hide = true;
			}
		}
	}

	// 頂点の設定
	if ( part->type == SsPartType::normal || part->type == SsPartType::mask )
	{
		SsCell * cell = state->cellValue.cell;
		if (cell && ( anime || setupAnime ) )
		{
			//サイズアトリビュートが指定されていない場合、セルのサイズを設定する
			if ( !size_x_key_find ) state->size.x = cell->size.x;
			if ( !size_y_key_find ) state->size.y = cell->size.y;
		}

		updateVertices(part , anime , state);
	}



}

void	SsAnimeDecoder::updateMatrix(SsPart* part , SsPartAnime* anime , SsPartState* state)
{
	int num = 1;
	if ((state->localscale.x != 1.0f) || (state->localscale.y != 1.0f))
	{
		//ローカルスケール適用マトリクスを作成する
		num = 2;
	}
	int matcnt;
	for (matcnt = 0; matcnt < num; matcnt++)
	{
		float *pmat = state->matrix;	//子パーツに継承するマトリクス
		if (matcnt > 0)
		{
			pmat = state->matrixLocal;	//自分だけに適用するローカルマトリクス
		}

		IdentityMatrix( pmat );

		if (state->parent)	//親パーツがある場合は親のマトリクスを継承する
		{
			memcpy( pmat , state->parent->matrix , sizeof( float ) * 16 );
		}

		// アンカー
		if ( state->parent )
		{
			const SsVector2& parentSize = state->parent->size;
			state->position.x = state->position.x + state->parent->size.x * state->anchor.x;
			state->position.y = state->position.y + state->parent->size.y * state->anchor.y;
		}

		TranslationMatrixM( pmat , state->position.x, state->position.y, state->position.z );//
		RotationXYZMatrixM( pmat , (float)(DegreeToRadian(state->rotation.x)) , (float)(DegreeToRadian(state->rotation.y)) , (float)(DegreeToRadian( state->rotation.z)) );
		float sx = state->scale.x;
		float sy = state->scale.y;
		if (matcnt > 0)
		{
			//ローカルスケールを適用する
			sx *= state->localscale.x;
			sy *= state->localscale.y;
		}
		ScaleMatrixM(pmat, sx, sy, 1.0f);
	}
	if (num == 1)
	{
		//ローカルスケールが使用されていない場合は継承マトリクスをローカルマトリクスに適用
		memcpy(state->matrixLocal, state->matrix, sizeof(state->matrix));
	}


}


void	SsAnimeDecoder::updateVertices(SsPart* part , SsPartAnime* anime , SsPartState* state)
{

	SsCell * cell = state->cellValue.cell;

	SsVector2 pivot;

	if (cell)
	{
		// セルに設定された原点オフセットを適用する
		// ※セルの原点は中央が0,0で＋が右上方向になっている
		float cpx = cell->pivot.x + 0.5f;
		if (state->hFlip) cpx = 1 - cpx;	// 水平フリップによって原点を入れ替える
		pivot.x = cpx * state->size.x;
		// 上が＋で入っているのでここで反転する。
		float cpy = -cell->pivot.y + 0.5f;
		if (state->vFlip) cpy = 1 - cpy;	// 垂直フリップによって原点を入れ替える
		pivot.y = cpy * state->size.y;
	}
	else
	{
		// セルが無いパーツでも原点が中央に来るようにする。
		pivot.x = 0.5f * state->size.x;
		pivot.y = 0.5f * state->size.y;
	}

	// 次に原点オフセットアニメの値を足す
	pivot.x += state->pivotOffset.x * state->size.x;
	pivot.y += -state->pivotOffset.y * state->size.y;

	float sx = -pivot.x;
	float ex = sx + state->size.x;
	float sy = +pivot.y;
	float ey = sy - state->size.y;

	// Z順
	/*
		これは実は上下ひっくり返って裏面になっているためUV値も上下反転させている。
		左上が最初に来る並びの方が頂点カラー・頂点変形のデータと同じで判りやすいのでこれでいく。
	*/
	float vtxPosX[4] = {sx, ex, sx, ex};
	float vtxPosY[4] = {sy, sy, ey, ey};

	SsPoint2 * vtxOfs = state->vertexValue.offsets;

	//きれいな頂点変形に対応
#if SPRITESTUDIO6SDK_USE_TRIANGLE_FIN

	if ( state->is_parts_color || state->is_vertex_transform )
	{

		SsVector2	vertexCoordinateLU = SsVector2( sx + (float)vtxOfs[0].x , sy + (float)vtxOfs[0].y );// : 左上頂点座標（ピクセル座標系）
		SsVector2	vertexCoordinateRU = SsVector2( ex + (float)vtxOfs[1].x , sy + (float)vtxOfs[1].y );// : 右上頂点座標（ピクセル座標系）
		SsVector2	vertexCoordinateLD = SsVector2( sx + (float)vtxOfs[2].x , ey + (float)vtxOfs[2].y );// : 左下頂点座標（ピクセル座標系）
		SsVector2	vertexCoordinateRD = SsVector2( ex + (float)vtxOfs[3].x , ey + (float)vtxOfs[3].y );// : 右下頂点座標（ピクセル座標系）

		SsVector2 CoordinateLURU = (vertexCoordinateLU + vertexCoordinateRU) * 0.5f;
		SsVector2 CoordinateLULD = (vertexCoordinateLU + vertexCoordinateLD) * 0.5f;
		SsVector2 CoordinateLDRD = (vertexCoordinateLD + vertexCoordinateRD) * 0.5f;
		SsVector2 CoordinateRURD = (vertexCoordinateRU + vertexCoordinateRD) * 0.5f;

		SsVector2 center;
		CoordinateGetDiagonalIntersection( center , CoordinateLURU, CoordinateRURD, CoordinateLULD, CoordinateLDRD );

		SsVector2*	coodinatetable[] = { &vertexCoordinateLU , &vertexCoordinateRU , &vertexCoordinateLD , &vertexCoordinateRD , &center };


		for (int i = 0; i < 5; ++i)
		{
			state->vertices[ i * 3 ] = coodinatetable[i]->x;
			state->vertices[ i * 3 + 1 ] = coodinatetable[i]->y;
			state->vertices[ i * 3 + 2]	= 0;
		}
	}else{
		for (int i = 0; i < 4; ++i)
		{
			state->vertices[i * 3]		= vtxPosX[i];
			state->vertices[i * 3 + 1]	= vtxPosY[i];
			state->vertices[i * 3 + 2]	= 0;

			++vtxOfs;
		}

	}

#else
	//4点変形の場合
	// 頂点変形のデータはＺ字順に格納されている。
	//SsPoint2 * vtxOfs = vertexValue.offsets;
	for (int i = 0; i < 4; ++i)
	{
		const SsProjectSetting projsetting = project->getProjectSetting();
		if (projsetting.vertexAnimeFloat != 0 )	//頂点変形を少数で行う
		{
			state->vertices[i * 3] = vtxPosX[i] + (float)vtxOfs->x;
			state->vertices[i * 3 + 1] = vtxPosY[i] + (float)vtxOfs->y;
		}
		else
		{
			state->vertices[i * 3] = vtxPosX[i] + (int)vtxOfs->x;
			state->vertices[i * 3 + 1] = vtxPosY[i] + (int)vtxOfs->y;
		}
		state->vertices[i * 3 + 2]	= 0;

		++vtxOfs;
	}
#endif



}





void	SsAnimeDecoder::updateInstance( int nowTime , SsPart* part , SsPartAnime* partanime , SsPartState* state )
{
	if ( state->refAnime == 0 ) return ;
	//state->refAnime->setPlayFrame( nowTime );
	//state->refAnime->update();

	SsAnimation* anime = state->refAnime->curAnimation;
	const SsInstanceAttr& instanceValue = state->instanceValue;

    //プレイヤー等では再生開始時にいったん計算してしまって値にしてしまった方がいい。
    //エディター側のロジックなのでそのまま検索する
    //インスタンスアニメ内のスタート位置
    int	startframe = CalcAnimeLabel2Frame( instanceValue.startLabel , instanceValue.startOffset, anime);
    int	endframe = CalcAnimeLabel2Frame( instanceValue.endLabel , instanceValue.endOffset, anime);

    state->instanceValue.startFrame = startframe;		//ラベル位置とオフセット位置を加えた実際のフレーム数
    state->instanceValue.endFrame = endframe;			//ラベル位置とオフセット位置を加えた実際のフレーム数


    //タイムライン上の時間 （絶対時間）
	int time = nowTime;

	//独立動作の場合
	if ( instanceValue.independent )
	{
		//float delta = animeState->frame - parentBackTime;
		float delta = this->frameDelta;

		state->instanceValue.liveFrame+= ( delta * instanceValue.speed );
		//parentBackTime = animeState->frame;
		time = (int)instanceValue.liveFrame;

	}

    //このインスタンスが配置されたキーフレーム（絶対時間）
    int	selfTopKeyframe = instanceValue.curKeyframe;


    int reftime = (int)(( time - selfTopKeyframe) * instanceValue.speed);
    //int	reftime = (time*instanceValue.speed) - selfTopKeyframe; //開始から現在の経過時間
	if ( reftime < 0 ) return ; //そもそも生存時間に存在していない
	if ( selfTopKeyframe > time ) return ;

    int inst_scale = (endframe - startframe) + 1; //インスタンスの尺


	//尺が０もしくはマイナス（あり得ない
	if ( inst_scale <= 0 ) return ;

	int	nowloop =  (reftime / inst_scale);	//現在までのループ数

    int checkloopnum = instanceValue.loopNum;

	//pingpongの場合では２倍にする
    if ( instanceValue.pingpong ) checkloopnum = checkloopnum * 2;

	//無限ループで無い時にループ数をチェック
    if ( !instanceValue.infinity )   //無限フラグが有効な場合はチェックせず
	{
        if ( nowloop >= checkloopnum )
		{
			reftime = inst_scale-1;
			nowloop = checkloopnum-1;
		}
	}

	int temp_frame = reftime % inst_scale;  //ループを加味しないインスタンスアニメ内のフレーム

    //参照位置を決める
    //現在の再生フレームの計算
    int _time = 0;
	bool	reverse = instanceValue.reverse;
	if ( instanceValue.pingpong && (nowloop % 2 == 1) )
	{
		if (reverse)
		{
			reverse = false;//反転
		}
		else
		{
			reverse = true;//反転
		}
	}

	if (reverse)
	{
		//リバースの時
		_time = endframe - temp_frame;
	}else{
		//通常時
		_time = temp_frame + startframe;
    }

	state->refAnime->setInstancePartsHide(state->hide);
	state->refAnime->setPlayFrame((float)_time);

	//Ver6 ローカルスケール対応
	//ローカルスケールを適用するために一時的に継承マトリクスを入れ替える
	float mattemp[16];
	memcpy(mattemp, state->matrix, sizeof(state->matrix));					//継承用マトリクスを退避する
	memcpy(state->matrix, state->matrixLocal, sizeof(state->matrixLocal));	//ローカルをmatrixに適用する

	float orgAlpha = state->alpha;
	if (state->is_localAlpha)
	{
		//ローカス不透明度が使用されている場合は不透明度をアップデート中だけ上書きする
		state->alpha = state->localalpha;									// ローカル不透明度対応
	}

	state->refAnime->update(this->frameDelta);								//インスタンスが参照するソースアニメのアップデート

	memcpy(state->matrix, mattemp, sizeof(mattemp));						//継承用マトリクスを戻す
	state->alpha = orgAlpha;

  
	//頂点の作成
    //update_vertices();

}

int		SsAnimeDecoder::findAnimetionLabel(const SsString& str, SsAnimation* Animation)
{
	for ( std::vector<SsLabel*>::iterator itr = Animation->labels.begin() ; 
		itr != Animation->labels.end() ; itr ++ )
	{
		if ( str == (*itr)->name )
		{
			return (*itr)->time;
		}
	}

	return 0;
}

int		SsAnimeDecoder::CalcAnimeLabel2Frame(const SsString& str, int offset, SsAnimation* Animation )
{

	//10フレームのアニメだと11が入ってるため計算がずれるため-1する
	int maxframe = Animation->settings.frameCount - 1;
	int startframe = Animation->settings.startFrame;	//Ver6.0対応
	int endframe = Animation->settings.endFrame;		//Ver6.0対応
	int ret2 = offset;

    if (  str == "_start" )
	{
		ret2 = startframe + offset;
	}else if ( str == "_end" )
	{
		ret2 = endframe + offset;
	}else if ( str == "none" )
	{
        return offset;
	}else{
		int ret = findAnimetionLabel(str, Animation);

        if ( ret != -1 )
        {
			int ret2 = ret + offset;
			if ( ret2 < startframe ) ret2 = startframe;
			if ( ret2 > endframe ) ret2 = endframe;

        	return ret2;
		}
		//警告など出すべき？
	}

    if ( ret2 < startframe ) ret2 = startframe;
	if ( ret2 > endframe ) ret2 = endframe;

	return ret2;



}

//マスク用ステンシルバッファの初期化を行うか？
//インスタンスパーツとして再生する場合のみ設定する
void	SsAnimeDecoder::setMaskFuncFlag(bool flg)
{
	maskFuncFlag = flg;
}

//親のマスク対象
//インスタンスパーツとして再生する場合のみ設定する
//各パーツのマスク対象とアンドを取って処理する
void	SsAnimeDecoder::setMaskParentSetting(bool flg)
{
	maskParentSetting = flg;
}

static SsPartStateLess _ssPartStateLess;

///SS5の場合  SsPartのarrayIndexは、親子順　（子は親より先にいない）と
///なっているためそのまま木構造を作らずUpdateを行う
void	SsAnimeDecoder::update(float frameDelta)
{
	int	time = (int)nowPlatTime;

	this->frameDelta = frameDelta;

	int cnt = 0;
	SPRITESTUDIO6SDK_foreach( std::vector<SsPartAndAnime> , partAnime , e )
	{
		SsPart* part = e->first;
		SsPartAnime* anime = e->second;

		std::vector<SsPartState>& partStateRaw = *(partState.get());
		updateState( time , part , anime , &partStateRaw[cnt] );

		updateMatrix( part , anime , &partStateRaw[cnt]);

		if ( part->type == SsPartType::instance )
		{
			updateInstance( time , part , anime , &partStateRaw[cnt] );
			updateVertices( part , anime , &partStateRaw[cnt] );
		}

		if ( part->type == SsPartType::effect)
		{
			updateMatrix( part , anime , &partStateRaw[cnt]);
			updateEffect(frameDelta, time, part, anime, &partStateRaw[cnt]);
		}

		cnt++;
	}

	if (meshAnimator)
		(meshAnimator.get())->update();

	sortList.sort(_ssPartStateLess);
	partStatesMask_.sort(_ssPartStateLess);

	maskIndexList.clear();
	for ( auto it = partStatesMask_.begin(); it != partStatesMask_.end(); ++it)
	{
		SsPartState * ps = (*it);
		maskIndexList.push_back(ps);
	}

	//今回再生した時間を保存しておく
	nowPlatTimeOld = nowPlatTime;

}


void	SsAnimeDecoder::updateEffect( float frameDelta , int nowTime , SsPart* part , SsPartAnime* part_anime , SsPartState* state )
{
	if ( state->hide ) return ;

	if ( state->effectValue.independent )
	{
		if (state && state->refEffect && state->effectValue.attrInitialized )
		{
			state->effectTimeTotal += frameDelta* state->effectValue.speed;
			state->refEffect->setLoop(true);
			state->refEffect->setFrame( state->effectTimeTotal );
			state->refEffect->play();
			state->refEffect->update();
		}
	}else{
		if (state && state->refEffect)
		{
			float _time = (float)(nowTime - state->effectTime);
			if ( _time < 0 )
			{
				return ;
			}

			_time*= state->effectValue.speed;
			_time += state->effectValue.startTime;

			state->refEffect->setSeedOffset( seedOffset );
			state->refEffect->setFrame( _time );
			state->refEffect->play();
			state->refEffect->update();
		}
	}



}
//描画
void	SsAnimeDecoder::draw()
{

	if (SsCurrentRenderer::getRender() == 0) return;

	SsCurrentRenderer::getRender()->renderSetup();


	if (maskFuncFlag == true) //マスク機能が有効（インスタンスのソースアニメではない）
	{
		//初期に適用されているマスクを精製
		for (size_t i = 0; i < maskIndexList.size(); i++)
		{
			SsPartState * ps = maskIndexList[i];

			if (!ps->hide)
			{
				//ステンシルバッファの作成
				//ps->partType = SsPartType::mask;
				SsCurrentRenderer::getRender()->renderPart(ps);
			}
		}
	}

	int mask_index = 0;

	SPRITESTUDIO6SDK_foreach( std::list<SsPartState*> , sortList , e )
	{
		SsPartState* state = (*e);

		if (state->partType == SsPartType::mask)
		{
			//マスクパーツ

			//6.2対応
			//非表示の場合でもマスクの場合は処理をしなくてはならない
			//マスクはパーツの描画より先に奥のマスクパーツから順にマスクを作成していく必要があるため
			//通常パーツの描画順と同じ箇所で非表示によるスキップを行うとマスクのバッファがクリアされずに、
			//マスクが手前の優先度に影響するようになってしまう。
			if (maskFuncFlag == true) //マスク機能が有効（インスタンスのソースアニメではない）
			{
				SsCurrentRenderer::getRender()->clearMask();
				mask_index++;	//0番は処理しないので先にインクメントする

				for (size_t i = mask_index; i < maskIndexList.size(); i++)
				{
					SsPartState * ps2 = maskIndexList[i];
					if (!ps2->hide)
					{
						SsCurrentRenderer::getRender()->renderPart(ps2);
					}
				}
			}
		}

		if ( state->hide )continue;

		if ( state->refAnime )
		{
			//インスタンスパーツ
			SsCurrentRenderer::getRender()->execMask(state);
			state->refAnime->draw();
		}
		else if ( state->refEffect )
		{
			//エフェクトパーツ
			SsCurrentRenderer::getRender()->execMask(state);

			//Ver6 ローカルスケール対応
			//ローカルスケールを適用するために一時的に継承マトリクスを入れ替える
			float mattemp[16];
			memcpy(mattemp, state->matrix, sizeof(state->matrix));					//継承用マトリクスを退避する
			memcpy(state->matrix, state->matrixLocal, sizeof(state->matrixLocal));	//ローカルをmatrixに適用する

			float orgAlpha = state->alpha;
			if (state->is_localAlpha)
			{
				//ローカス不透明度が使用されている場合は不透明度をアップデート中だけ上書きする
				state->alpha = state->localalpha;									// ローカル不透明度対応
			}

			state->refEffect->draw();

			memcpy(state->matrix, mattemp, sizeof(mattemp));						//継承用マトリクスを戻す
			state->alpha = orgAlpha;
		}
		else if (state->partType != SsPartType::mask)
		{
			//通常パーツ
			SsCurrentRenderer::getRender()->renderPart(state);
		}
	}

	SsCurrentRenderer::getRender()->enableMask(false);
}

}	// namespace spritestudio6

