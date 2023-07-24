
#include <stdio.h>
#include <cstdlib>

#include "../Loader/ssloader.h"

#include "ssplayer_animedecode.h"
#include "ssplayer_effect2.h"
#include "ssplayer_macro.h"
#include "ssplayer_matrix.h"
#include "ssplayer_render.h"
#include "ssplayer_effectfunction.h"

// MEMO: cpp側でのファイルスコープなので大丈夫かとは思うが、念のためSPRITESTUDIO6SDK_を接頭しておく
//       ※ちなみにどちらも使用していない。
#define SPRITESTUDIO6SDK_DEBUG_DISP (0)
#define SPRITESTUDIO6SDK_BUILD_ERROR_0418 (0)

namespace spritestudio6
{


static u8 blendNumber( u8 a , u8 b , float rate )
{
	return (u8)((int)( a + ( b - a ) * rate ));
}

static float blendFloat( float a,float b , float rate )
{
	return   ( a + ( b - a ) * rate );
}


double OutQuad(double t,double totaltime,double max ,double min )
{
	if( totaltime == 0.0 ) return 0.0;

	if ( t > totaltime ) t = totaltime;
	max -= min;
	t /= totaltime;
	return -max*t*(t-2)+min;
}

//現在時間から産出される位置を求める
//time変数から求められる式とする
//パーティクル座標計算のコア
void	SsEffectEmitter::updateParticle(float time, particleDrawData* p, bool recalc )
{
	float _t = (float)(time - p->stime);
	float _tm = (float)(_t - 1.0f );
	float _t2 = _t * _t; //(経過時間の二乗)
	float _life = (float)( p->lifetime - p->stime);

	if ( _life == 0 ) return ;
	float _lifeper = (float)( _t / _life );


	//_t = 0時点の値を作る
	//シード値で固定化されることが前提
	std::vector<unsigned long>&	seedListRaw = *(seedList.get());
	unsigned long pseed = seedListRaw[p->id % seedTableLen];

	//自身のシード値、エミッターのシード値、親パーティクルのＩＤをシード値とする
	rand.init_genrand(( pseed + emitterSeed + p->pid + seedOffset ));


	float rad = particle.angle + (rand.genrand_float32() * (particle.angleVariance ) - particle.angleVariance/2.0f);
	//float speed = rand.genrand_float32() * particle.speed;
	float speed = particle.speed + ( particle.speed2 * rand.genrand_float32() );


	//接線加速度
	float addr = 0;
	if ( particle.useTanAccel )
	{
		float accel = particle.tangentialAccel + (rand.genrand_float32() * particle.tangentialAccel2);

		float _speed = speed;
		if ( _speed <= 0 )_speed = 0.1f;
		//平均角速度を求める
		float l = _life * _speed * 0.2f; //円の半径
		float c = 3.14f * l;

		//最円周 / 加速度(pixel)
		addr = ( accel / c ) * _t;
	}

	float x = cos(rad + addr) * speed * (float)_t;
	float y = sin(rad + addr) * speed * (float)_t;

	if ( particle.useTransSpeed )
	{
		float transspeed = particle.transSpeed + ( particle.transSpeed2 * rand.genrand_float32() );
        float speedadd = transspeed / _life;

		float addtx =  cos(rad + addr) * speed;
		float addtx_trans =  cos(rad + addr) * speedadd;

		float addx = (( addtx_trans * _t ) + addtx ) * (_t+1.0f) / 2.0f;


		float addty =  sin(rad + addr) * speed;
		float addty_trans =  sin(rad + addr) * speedadd;

		float addy = (( addty_trans * _t ) + addty ) * ( _t+1.0f) / 2.0f;

		x = addx;
		y = addy;

	}


	//重力加速度の計算
	if ( particle.useGravity )
	{
		x += (0.5f * particle.gravity.x * (_t2));
		y += (0.5f * particle.gravity.y * (_t2));
	}

	//初期位置オフセット
	float ox,oy;
	ox = oy = 0;
	if ( particle.useOffset )
	{
		ox = (particle.offset.x + (particle.offset2.x * rand.genrand_float32()));
		oy = (particle.offset.y + (particle.offset2.y * rand.genrand_float32()));
	}

	//角度初期値
	p->rot = 0;
	if ( particle.useRotation )
	{
		p->rot = particle.rotation + (rand.genrand_float32() * particle.rotation2);
		float add = particle.rotationAdd + (rand.genrand_float32() * particle.rotationAdd2);

		//角度変化
		if ( particle.useRotationTrans )
		{
			//到達までの絶対時間
			float lastt = _life * particle.endLifeTimePer;

			float addf = 0;
			if ( lastt == 0 )
			{
			  	float addrf =  (add * particle.rotationFactor) * _t;
				p->rot+=addrf;
			}else{
				//1フレームで加算される量
				addf = ( add * particle.rotationFactor - add ) / lastt;

				//あまり時間
				float mod_t = _t - lastt;
				if ( mod_t < 0 ) mod_t = 0;

				//現在時間（最終時間でリミット
				float nowt = _t;
				if ( nowt > lastt ) nowt = lastt;

				//最終項 + 初項 x F / 2
				float final_soul = add + addf * nowt;
				float addrf = ( final_soul + add ) * (nowt+1.0f) / 2.0f;
				addrf-=add;
				addrf+= ( mod_t * ( final_soul ) ); //あまりと終項の積を加算
				p->rot+=addrf;
			}
		}else{
			p->rot+= ( (add*_t) );
		}
	}

	//カラーの初期値、カラーの変化
	p->color.a = 0xff;
	p->color.r = 0xff;
	p->color.g = 0xff;
	p->color.b = 0xff;

	if ( particle.useColor)
	{
		p->color.a = particle.initColor.a + (u8)(rand.genrand_float32() * particle.initColor2.a );
		p->color.r = particle.initColor.r + (u8)(rand.genrand_float32() * particle.initColor2.r );
		p->color.g = particle.initColor.g + (u8)(rand.genrand_float32() * particle.initColor2.g );
		p->color.b = particle.initColor.b + (u8)(rand.genrand_float32() * particle.initColor2.b );
	}

	if ( particle.useTransColor )
	{
		SsU8Color ecolor;
		ecolor.a = particle.transColor.a + (u8)(rand.genrand_float32() * particle.transColor2.a );
		ecolor.r = particle.transColor.r + (u8)(rand.genrand_float32() * particle.transColor2.r );
		ecolor.g = particle.transColor.g + (u8)(rand.genrand_float32() * particle.transColor2.g );
		ecolor.b = particle.transColor.b + (u8)(rand.genrand_float32() * particle.transColor2.b );

		p->color.a = blendNumber( p->color.a , ecolor.a , _lifeper );
		p->color.r = blendNumber( p->color.r , ecolor.r , _lifeper );
		p->color.g = blendNumber( p->color.g , ecolor.g , _lifeper );
		p->color.b = blendNumber( p->color.b , ecolor.b , _lifeper );
	}

	if ( particle.useAlphaFade )
	{

		float start = particle.alphaFade;
		float end = particle.alphaFade2;
        float per = _lifeper * 100.0f;


		if ( ( per < start ) && ( start > 0.0f ) ) //Ver6.2　0除算発生する可能性対策
		{
			float alpha = (start - per) / start;
//			p->color.a*= 1.0f - alpha;
			p->color.a = (u8)((float)p->color.a * (1.0f - alpha));
		}else{

			if ( per > end )
			{

				if (end>=100.0f)
				{
					p->color.a = 0;
				}else{
					float alpha = (per-end) / (100.0f-end);
                    if ( alpha >=1.0f ) alpha = 1.0f;

//					p->color.a*= 1.0f - alpha;
					p->color.a = (u8)((float)p->color.a * (1.0f - alpha));
				}
			}
		}
	}


	//スケーリング
	p->scale.x = 1.0f;
	p->scale.y = 1.0f;
	float scalefactor = 1.0f;

	if ( particle.useInitScale )
	{
		p->scale.x = particle.scale.x + (rand.genrand_float32() * particle.scaleRange.x );
		p->scale.y = particle.scale.y + (rand.genrand_float32() * particle.scaleRange.y );

        scalefactor = particle.scaleFactor + (rand.genrand_float32() * particle.scaleFactor2 );


	}

	if ( particle.useTransScale )
	{
		SsVector2 s2;
		float sf2;
		s2.x = particle.transscale.x + (rand.genrand_float32() * particle.transscaleRange.x );
		s2.y = particle.transscale.y + (rand.genrand_float32() * particle.transscaleRange.y );

		sf2 = particle.transscaleFactor + (rand.genrand_float32() * particle.transscaleFactor2 );

		p->scale.x = blendFloat( p->scale.x , s2.x , _lifeper );
		p->scale.y = blendFloat( p->scale.y , s2.y , _lifeper );
        scalefactor = blendFloat( scalefactor , sf2 , _lifeper );

	}

	p->scale.x*=scalefactor;
	p->scale.y*=scalefactor;

	p->x = x + ox + position.x;//エミッタからのオフセットを加算
	p->y = y + oy + position.y;//エミッタからのオフセットを加算


  	//指定の点へよせる
	if ( particle.usePGravity )
	{

		//生成地点からの距離
		SsVector2 v = SsVector2(  particle.gravityPos.x - (ox + position.x) ,
                         particle.gravityPos.y - (oy + position.y) );


		SsVector2 nv;
		SsVector2::normalize( v , &nv );

		float gp = particle.gravityPower;
		if (gp > 0) {
			SsVector2 v2 = SsVector2(p->x, p->y);

			//6.2対応　収束点座標を(0, 0)にすると収束しない
			float len = v.length(); // 生成位置からの距離
			if (len == 0.0f) {
				len = 0.1f;
				nv.x = 1;
				nv.y = 0;
			}

			float et = (len / gp)*0.90f;;

			float _gt = _t;
			if ( _gt >= (int)et )
			{
				_gt = et*0.90f;// + (_t / _life *0.1f);
			}

			nv = nv * gp * _gt;
			p->x += nv.x;
			p->y += nv.y;


			float blend = (float)(OutQuad(_gt, et, 0.9f, 0.0f));
			blend = blend; // *gp;
			blend += (_t / _life *0.1f);

			p->x = blendFloat(p->x, particle.gravityPos.x, blend);
			p->y = blendFloat(p->y, particle.gravityPos.y, blend);

		}
		else {
			nv = nv * gp * _t;
			// パワーマイナスの場合は単純に反発させる
			// 距離による減衰はない
			p->x += nv.x;
			p->y += nv.y;
		}

#if 0
		float gx = OutQuad( _t *0.8f , _life ,  particle.gravityPos.x , ox + position.x );
		float gy = OutQuad( _t *0.8f , _life ,  particle.gravityPos.y , oy + position.y );

		float gp = particle.gravityPower;

		if ( gp < 0 )
		{
			p->x = blendFloat( p->x , -gx , -gp);
			p->y = blendFloat( p->y , -gy , -gp);

		}else{
			p->x = blendFloat( p->x , gx , gp);
			p->y = blendFloat( p->y , gy , gp);
		}
#endif
	}

    //前のフレームからの方向を取る
	p->direc = 0.0f;
	if ( particle.useTurnDirec && recalc==false )
	{
		particleDrawData dp;
        dp = *p;

//		if ( time > 0.0f )
		{
			updateParticle( time + 1.0f , &dp , true );
			p->direc =  SsVector2::get_angle_360(
								SsVector2( 1 , 0 ) ,
								SsVector2(p->x - dp.x, p->y - dp.y) ) + (float)(DegreeToRadian(90) + DegreeToRadian(particle.direcRotAdd));
		}
	}


}


bool compare_life( emitPattern& left,  emitPattern& right)
{
	if ( left.life == right.life )
	{
        if ( left.uid < right.uid ) return true;

	}

	return left.life < right.life ;
}

void	SsEffectEmitter::precalculate2()
{
	rand.init_genrand( emitterSeed );

	_emitpattern.clear();
	//_lifeExtend.clear();
	_offsetPattern.clear();

	if ( !particleExistList )
	{
		particleExistList.reset( new std::vector<particleExistSt>(emitter.emitmax) ); //存在しているパーティクルが入る計算用バッファ
	}

	std::vector<particleExistSt>& particleExistListRaw = *(particleExistList.get());
	for(int i=0; i<emitter.emitmax; i++)
	{
		particleExistListRaw[i].Cleanup();
	}

	if ( emitter.emitnum < 1 ) emitter.emitnum = 1;

	int cycle =  (int)(( (float)(emitter.emitmax * emitter.interval)  / (float)emitter.emitnum ) + 0.5f) ;
    int group =  emitter.emitmax / emitter.emitnum;

	int extendsize = emitter.emitmax*LIFE_EXTEND_SCALE;
    if ( extendsize < LIFE_EXTEND_MIN ) extendsize = LIFE_EXTEND_MIN;




	int shot = 0;
	int offset = (int)(particle.delay);
	for ( int i = 0 ; i < emitter.emitmax ; i++ )
	{
		if ( shot >= emitter.emitnum )
		{
			shot = 0;
			offset+= emitter.interval;
		}
		_offsetPattern.push_back(offset);
		shot++;
	}


	for ( int i = 0 ; i < extendsize ; i++ )
	{
		emitPattern e;
		e.uid = i;
		e.life = emitter.particleLife + (int)(emitter.particleLife2 * rand.genrand_float32());
		e.cycle = cycle;

		if ( e.life > cycle )
		{
			e.cycle = e.life;
		}

		_emitpattern.push_back( e );
	}


	seedList.reset();

    particleListBufferSize = emitter.emitmax;


	rand.init_genrand((emitterSeed));

	seedTableLen = particleListBufferSize * 3;
	seedList.reset( new std::vector<unsigned long>(seedTableLen) );
	//各パーティクルＩＤから参照するシード値をテーブルとして作成する
	std::vector<unsigned long>&	seedListRaw = *(seedList.get());
	for ( size_t i = 0 ; i < seedTableLen ; i++ )
	{
    	seedListRaw[i] = rand.genrand_uint32();
	}
}



//----------------------------------------------------------------------------------




void SsEffectEmitter::updateEmitter( double _time , int slide ) 
{
	int onum = (int)(_offsetPattern.size());
	int pnum = (int)(_emitpattern.size());
	slide = slide * SEED_MAGIC;

	std::vector<particleExistSt>& particleExistListRaw = *(particleExistList.get());
	for ( int i = 0 ; i < onum ; i ++ )
	{
		int slide_num = ( i + slide ) % pnum;

		emitPattern* targetEP = &_emitpattern[slide_num];

		int t = (int)(_time - _offsetPattern[i]);

		particleExistListRaw[i].exist = false;
		particleExistListRaw[i].born = false;

		if ( targetEP->cycle != 0 )
		{
			int loopnum = t / targetEP->cycle;
			int cycle_top = loopnum * targetEP->cycle;

			particleExistListRaw[i].cycle = loopnum;

			particleExistListRaw[i].stime = cycle_top + _offsetPattern[i];
			particleExistListRaw[i].endtime = particleExistListRaw[i].stime + targetEP->life;// + _lifeExtend[slide_num];

			if ( (double)particleExistListRaw[i].stime <= _time &&  (double)particleExistListRaw[i].endtime > _time )
			{
				particleExistListRaw[i].exist = true;
				particleExistListRaw[i].born = true;
			}

			if ( !this->emitter.Infinite )
			{
				if ( particleExistListRaw[i].stime >= this->emitter.life ) //エミッターが終了している
				{
					particleExistListRaw[i].exist = false;    //作られてない

					//最終的な値に計算し直し <-事前計算しておくといいかも・
					int t = this->emitter.life - _offsetPattern[i];
					int loopnum = t / targetEP->cycle;

					int cycle_top = loopnum * targetEP->cycle;

					particleExistListRaw[i].stime = cycle_top + _offsetPattern[i];

					particleExistListRaw[i].endtime = particleExistListRaw[i].stime + targetEP->life;// + _lifeExtend[slide_num];
					particleExistListRaw[i].born = false;
				}else{
					particleExistListRaw[i].born = true;
				}
			}

			if ( t < 0 ){
				 particleExistListRaw[i].exist = false;
				 particleExistListRaw[i].born = false;
			}
		}
	}

}


const particleExistSt*	SsEffectEmitter::getParticleDataFromID(int id)
{
	std::vector<particleExistSt>& particleExistListRaw = *(particleExistList.get());
	return &particleExistListRaw[id];
}


void	SsEffectRenderV2::drawSprite(
		SsCellValue*		dispCell,
		SsVector2 _position,
		SsVector2 _size,
		float     _rotation,
		float	  direction,
		SsFColor	_color,
		SsRenderBlendType::_enum blendType
	)
{

	//SsCellValue*			dispCell;

	SsCurrentRenderer::getRender()->renderSetup();	

	switch( blendType )
	{
		case SsRenderBlendType::Mix:
			SsCurrentRenderer::getRender()->SetAlphaBlendMode(SsBlendType::mix);					
			break;
		case SsRenderBlendType::Add:
			SsCurrentRenderer::getRender()->SetAlphaBlendMode(SsBlendType::add);					
			break;
	}

	SsCurrentRenderer::getRender()->SetTexture( dispCell );


	float		matrix[4 * 4];	///< 行列
	IdentityMatrix( matrix );

	float parentAlpha = 1.0f;

	if ( parentState )
	{
		memcpy( matrix , parentState->matrix , sizeof( float ) * 16 );
    	parentAlpha = parentState->alpha;
	}


	TranslationMatrixM( matrix , _position.x * layoutScale.x , _position.y * layoutScale.y , 0.0f );

	RotationXYZMatrixM( matrix , 0 , 0 , (float)(DegreeToRadian(_rotation))+direction );

    ScaleMatrixM(  matrix , _size.x, _size.y, 1.0f );

	SsFColor fcolor;
	fcolor.fromARGB( _color.toARGB() );
	fcolor.a = fcolor.a * parentAlpha;


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


void SsEffectRenderV2::particleDraw(SsEffectEmitter* e , double time , SsEffectEmitter* parent , particleDrawData* plp )
{
	double t = time;

	if (e == 0) return;

	int pnum = e->getParticleIDMax();

	int slide = (parent == 0) ? 0 : plp->id;

	e->updateEmitter( time, slide ); 


	for (auto id = 0; id < pnum; id++)
	{
		const particleExistSt* drawe = e->getParticleDataFromID(id);

        if ( !drawe->born )continue;

		float targettime = ((float)t + 0.0f);
		particleDrawData lp;
		particleDrawData pp;
		pp.x = 0; pp.y = 0;

		lp.id = id + drawe->cycle;
		lp.stime = drawe->stime;
		lp.lifetime = drawe->endtime;
		lp.pid = 0;

		if ( parent )lp.pid = plp->id;

		//if ( lp.stime == lp.lifetime ) continue;

		//if ( lp.stime <= targettime && lp.lifetime >= targettime)
		if ( drawe->exist )
		{

			if (parent)
			{
				//親から描画するパーティクルの初期位置を調べる
				pp.id = plp->id;
				pp.stime = plp->stime;
				pp.lifetime = plp->lifetime;
				pp.pid = plp->pid;
				//パーティクルが発生した時間の親の位置を取る

				int ptime = lp.stime + pp.stime;
				if ( ptime > lp.lifetime ) ptime = lp.lifetime;

				//逆算はデバッグしずらいかもしれない
				parent->updateParticle( (float)lp.stime + pp.stime , &pp);
				e->position.x = pp.x;
				e->position.y = pp.y;

			}


			e->updateParticle(targettime, &lp);


			SsFColor fcolor;
			fcolor.fromARGB(lp.color.toARGB());

			drawSprite( &e->dispCell ,
						SsVector2(lp.x,lp.y),
						lp.scale,
						lp.rot , lp.direc , fcolor , e->refData->BlendType );


		}

	}



}



//パラメータをコピーする
void	SsEffectRenderV2::initEmitter( SsEffectEmitter* e , SsEffectNode* node)
{

	e->refData = node->GetMyBehavior();
	e->refCell = e->refData->refCell;

	//セルの初期化
	SsCelMapLinker* link = this->curCellMapManager->getCellMapLink( e->refData->CellMapName );

	if ( link )
	{
		SsCell * cell = link->findCell( e->refData->CellName );
		
		getCellValue(	this->curCellMapManager , 
			e->refData->CellMapName ,
			e->refData->CellName , 
			e->dispCell ); 
	}else{
		Logger << U"cell not found : {} , {}"_fmt(
			Unicode::FromUTF8(e->refData->CellMapName), 
			Unicode::FromUTF8(e->refData->CellName)
		);
	}

	SsEffectFunctionExecuter::initializeEffect( e->refData , e );

	e->emitterSeed = this->mySeed;

	if ( e->particle.userOverrideRSeed )
	{
		e->emitterSeed = e->particle.overrideRSeed;

	}else{
		if ( this->effectData->isLockRandSeed )
		{
			e->emitterSeed = (this->effectData->lockRandSeed+1) * SEED_MAGIC;
		}
	}

	e->emitter.life += (int)e->particle.delay;//ディレイ分加算
}


void	SsEffectRenderV2::clearEmitterList()
{
	for ( size_t i = 0 ; i < this->emmiterList.size(); i++)
	{
		emmiterList[i].reset();
	}

	emmiterList.clear();
	updateList.clear();

}



void	SsEffectRenderV2::setEffectData(SsEffectModel* data)
{
	effectData = data;

    reload();

}


void	SsEffectRenderV2::update()
{

	if ( !m_isPlay ) return;

	targetFrame = nowFrame;

	if ( !this->Infinite )
	{
		if ( this->isloop() ) //自動ループの場合
		{
			if ( nowFrame > getEffectTimeLength() )
			{
				targetFrame = (float)((int)nowFrame % getEffectTimeLength());
				int l = (int)( nowFrame / (float)(getEffectTimeLength()) );
				setSeedOffset( l );
			}
		}
	}

}

void	SsEffectRenderV2::draw()
{
    if ( nowFrame < 0 ) return;

	for ( size_t i = 0 ; i < updateList.size() ; i++ )
	{
		SsEffectEmitter* e = updateList[i];
		if ( e )
		{
			e->setSeedOffset( seedOffset ); 
		}
	}

	for ( size_t i = 0 ; i < updateList.size() ; i++ )
	{
		SsEffectEmitter* e = updateList[i];

		if ( e->_parent )
		{
			//グローバルの時間で現在親がどれだけ生成されているのかをチェックする
			e->_parent->updateEmitter(targetFrame , 0);

			int loopnum =  e->_parent->getParticleIDMax();
			for ( int n = 0 ; n < loopnum ; n ++ )
			{
				const particleExistSt* drawe = e->_parent->getParticleDataFromID(n);

				if ( drawe->born )
				{
					particleDrawData lp;
					lp.stime = drawe->stime;
					lp.lifetime = drawe->endtime;
					lp.id = n;
					lp.pid = 0;

					float targettime = (targetFrame + 0.0f);
					float ptime = (targettime - lp.stime );

	  				particleDraw( e , ptime , e->_parent , &lp);
				}
			}

		}else{
			particleDraw( e , targetFrame );
		}
	}

}


bool compare_priority( SsEffectEmitter* left,  SsEffectEmitter* right)
{
	if ( left->priority == right->priority )
	{
		if ( left->uid < right->uid ) return true;
	}
  //	return true;
  return left->priority < right->priority ;
}


void    SsEffectRenderV2::reload()
{
	nowFrame = 0;

    //updateが必要か
	stop();
	clearEmitterList();

	SsEffectNode* root = this->effectData->GetRoot();

    //this->effectData->updateNodeList();//ツールじゃないので要らない
    const std::vector<SsEffectNode*>& list = this->effectData->getNodeList();

	layoutScale.x = (float)(this->effectData->layoutScaleX) / 100.0f;
	layoutScale.y = (float)(this->effectData->layoutScaleY) / 100.0f;

	//MEMO: cnumはスコープ内ワークなので、スマートポインタ化していません。
	int* cnum = new int[list.size()];
	for(size_t i=0; i<list.size(); i++)
	{
		cnum[i] = 0;
	}

	bool _Infinite = false;
	//パラメータを取得
	//以前のデータ形式から変換
	for ( size_t i = 0 ; i < list.size() ; i ++ )
	{
		SsEffectNode *node = list[i];

		if ( node->GetType() == SsEffectNodeType::emmiter )
		{
			// MEMO: 本処理の最後でスマートポインタ化しています（途中生成中断があり得るので）
			SsEffectEmitter* e = new SsEffectEmitter();
			//パラメータをコピー

			e->_parentIndex = node->parentIndex;
			//繋ぎ先は恐らくパーティクルなのでエミッタに変換
			if ( e->_parentIndex != 0 )
			{
				e->_parentIndex = list[e->_parentIndex]->parentIndex;

			}

			cnum[e->_parentIndex]++;
			if ( cnum[e->_parentIndex] > 10 )
			{
				delete e;
				e = 0;
				_isWarningData = true;
				continue; //子１０ノード表示制限
			}

			//孫抑制対策
			if ( e->_parentIndex != 0 )
			{
				int a = list[e->_parentIndex]->parentIndex;
				if ( a != 0 )
				{
				   if ( list[a]->parentIndex > 0 ) {
						delete e;
						e = 0;
						_isWarningData = true;
						continue;
				   }
				}
			}

			initEmitter( e , node );
			this->emmiterList.push_back( std::move( std::unique_ptr<SsEffectEmitter>(e) ) );
			if ( e->emitter.Infinite ) _Infinite = true;
		}else
		{
			//エミッター同士を繋ぎたいので
			this->emmiterList.push_back( std::move( std::unique_ptr<SsEffectEmitter>() ) );
		}
	}

	delete[] cnum;
	Infinite = _Infinite;


    //親子関係整理


	effectTimeLength = 0;
	//事前計算計算  updateListにルートの子を配置し親子関係を結ぶ
	SsEffectEmitter* emitter;
	for ( size_t i = 0 ; i < this->emmiterList.size(); i++)
	{
		emitter = emmiterList[i].get();
		if ( emitter != 0 )
		{

			emitter->uid = (int)i;
			//emitter->precalculate();
			emitter->precalculate2(); //ループ対応形式


			int  pi = emitter->_parentIndex;

			if ( emitter->_parentIndex == 0 )  //ルート直下
			{
				emitter->_parent = 0;
				emitter->globaltime = emitter->getTimeLength();
				updateList.push_back(emitter);
			}else{
				SsEffectEmitter* emitterPI = emmiterList[pi].get();

				emitter->_parent = emitterPI;

				emitter->globaltime = (size_t)(emitter->getTimeLength()) + (size_t)(emitterPI->getTimeLength());

				updateList.push_back(emitter);
			}

			if ( emitter->globaltime > effectTimeLength )
			{
				effectTimeLength = emitter->globaltime;
			}
		}
	}
	//プライオリティソート
	std::sort( updateList.begin() , updateList.end() , compare_priority );


}


size_t  SsEffectRenderV2::getEffectTimeLength()
{

	return effectTimeLength;
}


int	SsEffectRenderV2::getCurrentFPS(){
	if (effectData)
	{
		if ( effectData->fps == 0 ) return 30;

		return effectData->fps;
	}
	return 30;
}

}	// namespace spritestudio6

