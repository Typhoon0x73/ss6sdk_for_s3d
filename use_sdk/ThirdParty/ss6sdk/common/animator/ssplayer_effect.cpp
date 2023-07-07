#include <stdio.h>
#include <cstdlib>

#include "../loader/ssloader.h"

#include "ssplayer_animedecode.h"
#include "ssplayer_effect.h"
#include "ssplayer_macro.h"
#include "ssplayer_matrix.h"
#include "ssplayer_render.h"
#include "ssplayer_effectfunction.h"


namespace spritestudio6
{


class SsEffectRenderParticle;

//SSの乱数固定ボタンがビットに対応
static  int seed_table[] =
{
	485,   //0
	583,   //1
	814,   //2
	907,   //3
	1311,  //4
	1901,  //5
	2236,  //6
	3051,  //7
	3676,  //8
	4338,  //9
	4671,  //10
	4775,  //11
	4928,  //12
	4960,  //13
	5228,  //14
	5591,  //15
	5755,  //16
	5825,  //17
	5885,  //18
	5967,  //19
	6014,  //20
	6056,  //21
	6399,  //22
	6938,  //23
	7553,  //24
	8280,  //25
	8510,  //26
	8641,  //27
	8893,  //28
	9043,  //29
	31043, //30
	82562  //31
};
//--------


// #define ONEFRAME ( 1.0f / 60.0f )
constexpr auto ONEFRAME = ( 1.0f / 60.0f );


//------------------------------------------------------------------------------
//	ユーティリティ
//------------------------------------------------------------------------------
SsEffectDrawBatch*	SsEffectRenderer::findBatchListSub(SsEffectNode* n)
{
	SsEffectDrawBatch* bl = 0;
	SPRITESTUDIO6SDK_foreach( std::list<SsEffectDrawBatch*> , drawBatchList , e )
	{
		if ( (*e)->targetNode == n )
		{
			bl = (*e);
			return bl;
		}
	}

	return bl;
}


SsEffectDrawBatch*	SsEffectRenderer::findBatchList(SsEffectNode* n)
{

	SsEffectDrawBatch* bl = 0;//findBatchListSub(n);
	if ( bl ==0 )
	{
		if ( SSEFFECTRENDER_BACTH_MAX <= dpr_pool_count ){
				return 0;
		}

		bl = &drawPr_pool[dpr_pool_count];

		dpr_pool_count++;
		bl->targetNode = n;
	}

	return bl;

}

//------------------------------------------------------------------------------
//要素生成関数
//------------------------------------------------------------------------------
SsEffectRenderAtom* SsEffectRenderer::CreateAtom( unsigned int seed , SsEffectRenderAtom* parent , SsEffectNode* node )
{
	SsEffectRenderAtom* ret = 0;
	SsEffectNodeType::_enum type = node->GetType(); 


	if ( type == SsEffectNodeType::particle )
	{
#if SPRITESTUDIO6SDK_PFMEM_TEST
		if ( SSEFFECTRENDER_PARTICLE_MAX <= pa_pool_count )
		{
			 return 0;
		}
		SsEffectRenderParticle* p = &pa_pool[pa_pool_count];
		p->InitParameter();
		pa_pool_count++;

		p->data = node;
		p->parent = parent;

#else
		SsEffectRenderParticle* p = new SsEffectRenderParticle( node , parent );
#endif

		updatelist.push_back( p );
		createlist.push_back( p );
		SsEffectRenderEmitter*	em = (SsEffectRenderEmitter*)parent;
		em->myBatchList->drawlist.push_back( p );
	
		ret = p;
	}


	if ( type == SsEffectNodeType::emmiter )
	{

#if SPRITESTUDIO6SDK_PFMEM_TEST
		if ( SSEFFECTRENDER_EMMITER_MAX <= em_pool_count )
		{
			return 0;
		}
		if ( SSEFFECTRENDER_BACTH_MAX <= dpr_pool_count ){
				return 0;
		}

		SsEffectRenderEmitter* p = &em_pool[em_pool_count];



		p->InitParameter();
		em_pool_count++;

		p->data = node;
		p->parent = parent;

#else
		SsEffectRenderEmitter* p = new SsEffectRenderEmitter( node , parent);
#endif
		p->setMySeed( seed );
		p->TrushRandom( em_pool_count%9 );

		SsEffectFunctionExecuter::initalize( &p->data->behavior , p );

		//セルデータの検索とセット
		//オリジナルでは上記initializeでやっているがクラス階層の関係からこちらでやる
		SsCelMapLinker* link = this->curCellMapManager->getCellMapLink( p->data->behavior.CellMapName );
		if ( link )
		{
			SsCell * cell = link->findCell( p->data->behavior.CellName );
		
			getCellValue(	this->curCellMapManager , 
				p->data->behavior.CellMapName ,
				p->data->behavior.CellName , 
				p->dispCell ); 
		}else{
			Logger << U"cell not found : {} , {}"_fmt(
				Unicode::FromUTF8(p->data->behavior.CellMapName), 
				Unicode::FromUTF8(p->data->behavior.CellName)
			);
		}

		updatelist.push_back( p );
		createlist.push_back( p );


		//バッチリストを調べる
		SsEffectDrawBatch* bl = findBatchList(node);

		p->myBatchList = bl;
		drawBatchList.push_back( bl );

		ret = p;
	}
	return ret;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool particleDelete(SsEffectRenderAtom* d)
{

	if ( d->m_isInit )
	{
		if ( d->m_isLive == false )
		{
		   //	delete d;
			return true;
		}

		if ( d->_life <= 0.0f)
		{
			d->m_isLive = false;
			return true;
		}
	}

    return false;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool particleDeleteAll(SsEffectRenderAtom* d)
{
	delete d;
	return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderEmitter::setMySeed( unsigned int seed )
{
	if ( seed > 31 ){
		(this->MT.get())->init_genrand( seed );
	}else{
		(this->MT.get())->init_genrand( seed_table[seed] );
	}
	myseed = seed ;
}


//----------------------------------------------------------------------
//生成フェーズ           SsEffectRendererへ移動してもいいかも
//----------------------------------------------------------------------
void	SsEffectRenderEmitter::Initialize()
{


	SsEffectNode* n = static_cast<SsEffectNode*>(this->data->ctop);

	if ( !m_isInit )
	{                                                                                                                                                 		//子要素を解析(一度だけ）
		while ( n )
		{
			if ( n->GetType() ==  SsEffectNodeType::particle )
			{
				param_particle = n;
			}

			n = static_cast<SsEffectNode*>(n->next);
		}

		if (this->data->GetMyBehavior())
		{
			SsEffectFunctionExecuter::initalize( this->data->GetMyBehavior() , this );
		}
        intervalleft = this->interval;
	}


	m_isInit = true;
}

//----------------------------------------------------------------------
//パーティクルオブジェクトの生成
//----------------------------------------------------------------------
bool	SsEffectRenderEmitter::genarate( SsEffectRenderer* render )
{

	if ( !generate_ok )return true;
	if ( m_isLive == false ) return true;

	int create_count = this->burst;
	if ( create_count <= 0 ) create_count = 1;


	int pc = (int)particleCount;

	while(1)
	{
		if ( this->intervalleft >= this->interval )
		{
			for ( int i = 0 ; i < create_count; i++)//最大作成数
			{
				if ( pc < maxParticle )
				{
					if ( param_particle )
					{
						SsEffectRenderAtom* a = render->CreateAtom( 0 , this , param_particle );
						if ( a )
						{
							a->Initialize();
							a->update(render->frameDelta);
							bool ret = a->genarate(render);
							pc++;
							if ( ret == false ) return false;
						}else{
							return false;
						}
					}
				}
			}
			this->intervalleft-=this->interval;
			//if ( this->interval == 0 )return true;
		}else{
			return true;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderEmitter::update(float delta)
{

	_exsitTime+=delta;
	_life = _lifetime - _exsitTime;
	intervalleft+=delta;

	if ( this->parent )
	{
		//以下は仮
		this->position = this->parent->position;
		this->rotation = this->parent->rotation;
		this->scale = this->parent->scale;
		this->alpha = this->parent->alpha;
	}
	if (this->data->GetMyBehavior())
	{
		SsEffectFunctionExecuter::updateEmmiter( this->data->GetMyBehavior() , this );
	}

	if ( this->myBatchList )
	{
		this->myBatchList->priority = this->drawPriority;
		this->myBatchList->dispCell = &this->dispCell;
		this->myBatchList->blendType = this->data->GetMyBehavior()->BlendType;

	}

}


//----------------------------------------------------------------------
//パーティクルクラス
//----------------------------------------------------------------------
//生成フェーズ
void	SsEffectRenderParticle::Initialize()
{


	if ( !m_isInit )
	{
		SsEffectNode* n = static_cast<SsEffectNode*>(this->data->ctop);

		//子要素を解析  基本的にエミッターのみの生成のはず　（Ｐではエラーでいい）
		//処理を省いてエミッター生成のつもりで作成する
		//パーティクルに紐づいたエミッターが生成される
		parentEmitter = 0;

		parentEmitter = static_cast<SsEffectRenderEmitter*>(this->parent);

		dispCell = &parentEmitter->dispCell;
		if ( parentEmitter->data == 0 )
		{
			this->_life = 0.0f;
			m_isInit = false;
			return ;
		}

		this->refBehavior = parentEmitter->data->GetMyBehavior();
		if ( refBehavior )
		{
			 SsEffectFunctionExecuter::initializeParticle( refBehavior , parentEmitter , this );

		}
	}

	m_isInit = true;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool	SsEffectRenderParticle::genarate( SsEffectRenderer* render )
{
	SsEffectNode* n = static_cast<SsEffectNode*>(this->data->ctop);
	if ( m_isInit && !m_isCreateChild)
	{
		if ( parentEmitter )
		{
			while ( n )
			{
				if ( parentEmitter == NULL ) return true;
				SsEffectRenderAtom* r = render->CreateAtom( parentEmitter->myseed , this , n );
				if ( r )
				{
					n = static_cast<SsEffectNode*>(n->next);
					r->Initialize();
					r->update( render->frameDelta );
					r->genarate( render );
				}else{
					return false;
				}
			}
		}

		m_isCreateChild = true;
	}

	return true;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderParticle::update(float delta)
{


	 //_rotation = 0;
     if ( !this->isInit() )return ;
	 this->position.x = this->_position.x;
	 this->position.y = this->_position.y;
	 this->scale = this->parent->scale;
	 this->alpha = this->parent->alpha;

	 //初期値突っ込んでおく、パーティクルメソッドのアップデートに持ってく？
	 this->_color = this->_startcolor;

	//this->parent
	if ( parentEmitter )
	{
    	updateDelta( delta );

		if ( refBehavior )
		{
			 SsEffectFunctionExecuter::updateParticle( refBehavior , parentEmitter , this );
		}

		updateForce( delta );

		if (parent->_life <= 0.0f)
		{
		}else{
			//仮
			this->position.x = this->_position.x;
			this->position.y = this->_position.y;
		}

	}

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderParticle::updateDelta(float delta)
{
	_rotation+=( _rotationAdd*delta );

	_exsitTime+=delta;
	_life = _lifetime - _exsitTime;

	SsVector2	tangential = SsVector2( 0 , 0 );

	//接線加速度の計算
	SsVector2  radial = SsVector2(this->_position.x,this->_position.y);

    SsVector2::normalize( radial , &radial );
	tangential = radial;

    radial = radial * _radialAccel;

	float newY = tangential.x;
	tangential.x = -tangential.y;
	tangential.y = newY;

	tangential = tangential* _tangentialAccel;

	SsVector2 tmp = radial + tangential;

	this->_execforce = tmp;


}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void 	SsEffectRenderParticle::updateForce(float delta)
{

	this->_backposition = this->_position;

	this->_force = _gravity;
	SsVector2 ff = (this->vector * this->speed) + this->_execforce + this->_force;


	if ( isTurnDirection )
	{
		this->direction =  SsPoint2::get_angle_360( SsVector2( 1.0f , 0.0f ) , ff ) - (float)DegreeToRadian(90);
	}else{
        this->direction = 0;
	}

	//フォースを加算
	this->_position.x+= (ff.x * delta );
	this->_position.y+= (ff.y * delta );

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderParticle::draw(SsEffectRenderer* render)
{

	if ( this->parentEmitter == NULL  )return;
	if ( refBehavior == NULL ) return;
	if ( dispCell == NULL ) return ;
	if ( dispCell->texture.isEmpty() ) return;

	float		matrix[4 * 4];	///< 行列
	IdentityMatrix( matrix );


	if (render->parentState)
	{
		memcpy( matrix , render->parentState->matrix , sizeof( float ) * 16 );
		this->alpha = (render->render_root.get())->alpha;
	}

	TranslationMatrixM( matrix , _position.x, _position.y, 0.0f );

	RotationXYZMatrixM( matrix , 0.0f , 0.0f , (float)(DegreeToRadian(_rotation)+direction) );

    ScaleMatrixM(  matrix , _size.x, _size.y, 1.0f );

	SsFColor fcolor;
	fcolor.fromARGB( _color.toARGB() );
	fcolor.a = fcolor.a * this->alpha;

	if ( ( dispCell->cell ) && ( fcolor.a != 0.0f ) )
	{

		SsVector2 pivot = SsVector2( dispCell->cell->pivot.x ,dispCell->cell->pivot.y);

		pivot.x = pivot.x * dispCell->cell->size.x;
		pivot.y = pivot.y * dispCell->cell->size.y;

		SsVector2 dispscale = dispCell->cell->size;


		SsCurrentRenderer::getRender()->renderSpriteSimple(
			matrix,
			(int)dispscale.x , (int)dispscale.y ,  pivot,
					dispCell->uvs[0],
					dispCell->uvs[3], fcolor );
	}
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------

bool compare_priority( SsEffectDrawBatch* left,  SsEffectDrawBatch* right)
{
  //	return true;
  return left->priority < right->priority ;
}
//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
extern int g_particle_num;
void	SsEffectRenderer::update(float delta)
{

	if (m_isPause) return;
	if (!m_isPlay) return;
    if ( !(this->render_root) ) return ;

	frameDelta = delta;

	if ( parentState )
	{
		
		SsVector3 pos = SsVector3( parentState->matrix[3*4] ,
								   parentState->matrix[3*4+1] ,
								   parentState->matrix[3*4+2] );

		layoutPosition = pos;

		SsEffectRenderAtom* effectRendererAtomRaw = this->render_root.get();
		effectRendererAtomRaw->setPosistion( 0 , 0 , 0 );

		effectRendererAtomRaw->rotation = 0;
		effectRendererAtomRaw->scale = SsVector2(1.0f, 1.0f);
		effectRendererAtomRaw->alpha = parentState->alpha;
	}

	size_t loopnum = updatelist.size();
	for ( size_t i = 0 ; i < loopnum ; i++ )
	{
		SsEffectRenderAtom* re = updatelist[i];
		re->Initialize();
		re->count();
	}

	loopnum = updatelist.size();
	size_t updatecount = 0;
	for ( size_t i = 0 ; i < loopnum ; i++ )
	{
		SsEffectRenderAtom* re = updatelist[i];

		if ( re->m_isLive == false ) continue;

		if ( re->parent && re->parent->_life  <= 0.0f || re->_life <= 0.0f )
		{
			re->update(delta);
		}else{
			re->update(delta);
			re->genarate(this);
		}

		updatecount++;
	}
//	g_particle_num = updatecount;
	//後処理  寿命で削除
	//死亡検出、削除の2段階
	std::vector<SsEffectRenderAtom*>::iterator endi = remove_if( updatelist.begin(), updatelist.end(), particleDelete );
    updatelist.erase( endi, updatelist.end() );

	drawBatchList.sort(compare_priority);


	if ( m_isLoop )
	{
		if ( updatecount== 0)
		{
		  reload();
		}
	}
}

//extern int g_particle_draw_num;
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderer::draw()
{
	SsCurrentRenderer::getRender()->renderSetup();					

	int cnt = 0;

	SPRITESTUDIO6SDK_foreach( std::list<SsEffectDrawBatch*> , drawBatchList , e )
	{
		//セットアップ
		if ( (*e)->dispCell )
		{
			switch( (*e)->blendType )
			{
				case SsRenderBlendType::Mix:
					SsCurrentRenderer::getRender()->SetAlphaBlendMode(SsBlendType::mix);					
					break;
				case SsRenderBlendType::Add:
					SsCurrentRenderer::getRender()->SetAlphaBlendMode(SsBlendType::add);					
					break;
			}
			SsCurrentRenderer::getRender()->SetTexture( (*e)->dispCell );
		}

		SPRITESTUDIO6SDK_foreach( std::list<SsEffectRenderAtom*> , (*e)->drawlist , e2 )
		{
			if ( (*e2) )
			{
				if ( !(*e2)->m_isLive) continue;
				if( (*e2)->_life <= 0.0f )continue;

				(*e2)->draw(this);
				cnt++;
			}
		}

	}
	//g_particle_draw_num = cnt;


}



//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
SsEffectRenderer::~SsEffectRenderer()
{
	clearUpdateList();

	render_root.reset();
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void	SsEffectRenderer::clearUpdateList()
{


	size_t s = createlist.size();
	size_t s2 = updatelist.size();


#if SPRITESTUDIO6SDK_PFMEM_TEST
	em_pool_count = 0;
	pa_pool_count = 0;
	dpr_pool_count = 0;

#else
	for ( size_t i = 0 ; i <  createlist.size() ; i++ )
	{
		delete createlist[i];
	}
#endif

	updatelist.clear();
	createlist.clear();

	SPRITESTUDIO6SDK_foreach( std::list<SsEffectDrawBatch*> , drawBatchList , e )
	{
		(*e)->drawlist.clear();
	}

	drawBatchList.clear();

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

void    SsEffectRenderer::reload()
{
	clearUpdateList();

	//座標操作のためのルートノードを作成する
	if ( !render_root )
	{
		render_root.reset( new SsEffectRenderAtom() );
	}

	//ルートの子要素を調査して作成する
	SsEffectNode* root = this->effectData->GetRoot();

	//シード値の決定
	u32 seed = 0;

	if ( this->effectData->isLockRandSeed )
	{
		seed = this->effectData->lockRandSeed;
	}else{
		seed = mySeed;
	}

	SimpleTree* n = root->ctop;
	//子要素だけつくってこれを種にする
	while( n )
	{
		SsEffectNode* enode = static_cast<SsEffectNode*>(n);
		SsEffectRenderAtom* effectr = CreateAtom( seed , render_root.get() , enode );

		n = n->next;
	}

}

void    SsEffectRenderer::play()
{
	m_isPlay = true;
	m_isPause = false;




}
	
void	SsEffectRenderer::stop()
{
	m_isPlay = false;

}
	
void    SsEffectRenderer::pause()
{
	m_isPause = true;

}

void	SsEffectRenderer::setLoop(bool flag)
{
	m_isLoop = flag;
}


//再生ステータスを取得
bool	SsEffectRenderer::getPlayStatus(void)
{
	return(m_isPlay);
}



}	// namespace spritestudio6


