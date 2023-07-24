#include <stdio.h>
#include <cstdlib>

#include "../Loader/ssloader.h"
#include "ssplayer_animedecode.h"
#include "ssplayer_effect.h"
#include "ssplayer_macro.h"
#include "ssplayer_matrix.h"
#include "ssplayer_render.h"
#include "ssplayer_effectfunction.h"


namespace spritestudio6
{


//二つの値の範囲から値をランダムで得る
static u8 GetRandamNumberRange( SsEffectRenderEmitter* e , u8 a , u8 b )
{
	u8 min = a < b ? a : b;
	u8 max = a < b ? b : a;

	u8 diff = ( max - min );


    if ( diff == 0 ) return min;
	return min + ((e->MT.get())->genrand_uint32() % diff);
}

static void VarianceCalcColor( SsEffectRenderEmitter* e ,  SsU8Color& out , SsU8Color  color1 , SsU8Color color2 )
{

	out.r = GetRandamNumberRange( e , color1.r , color2.r );
	out.g = GetRandamNumberRange( e ,  color1.g , color2.g );
	out.b = GetRandamNumberRange( e ,  color1.b , color2.b );
	out.a = GetRandamNumberRange( e ,  color1.a , color2.a );

}



float frand(unsigned v) {
    unsigned res = (v>>9) | 0x3f800000;
    return (*(float*)&res) - 1.0f;
}

static float VarianceCalc( SsEffectRenderEmitter* e ,  float base , float variance )
{
	unsigned long r = (e->MT.get())->genrand_uint32();

	float len = variance - base;

    return base + len * frand( r );


}

static float VarianceCalcFin( SsEffectRenderEmitter* e ,  float base , float variance )
{
	unsigned long r = (e->MT.get())->genrand_uint32();

	return base + (-variance + variance* ( frand(r) * 2.0f ));

}


static u8 blendNumber( u8 a , u8 b , float rate )
{
	return ( ( a + (u8)((float)( b - a ) * rate )) );
}


static float blendFloat( float a,float b , float rate )
{
	return   ( a + ( b - a ) * rate );
}


class EffectFuncBase
{
public:
	EffectFuncBase(){}
	virtual ~EffectFuncBase(){}

	virtual void	initalizeEmmiter ( SsEffectElementBase* ele , SsEffectRenderEmitter* emmiter){ SPRITESTUDIO6SDK_NOUSE_ARGUMENT(ele); SPRITESTUDIO6SDK_NOUSE_ARGUMENT(emmiter); }
	virtual void	updateEmmiter( SsEffectElementBase* ele , SsEffectRenderEmitter* emmiter){ SPRITESTUDIO6SDK_NOUSE_ARGUMENT(ele); SPRITESTUDIO6SDK_NOUSE_ARGUMENT(emmiter); }
	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* particle ){ SPRITESTUDIO6SDK_NOUSE_ARGUMENT(ele); SPRITESTUDIO6SDK_NOUSE_ARGUMENT(e); SPRITESTUDIO6SDK_NOUSE_ARGUMENT(particle); }
	virtual void	updateParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* particle ){ SPRITESTUDIO6SDK_NOUSE_ARGUMENT(ele); SPRITESTUDIO6SDK_NOUSE_ARGUMENT(e); SPRITESTUDIO6SDK_NOUSE_ARGUMENT(particle); }


	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* emmiter){}
};




//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementBasic : public EffectFuncBase
{
public:
	FuncParticleElementBasic(){}
	virtual ~FuncParticleElementBasic(){}

	virtual void	initalizeEmmiter ( SsEffectElementBase* ele , SsEffectRenderEmitter* e)
	{
		ParticleElementBasic* source = static_cast<ParticleElementBasic*>(ele);

		e->maxParticle = source->maximumParticle;
		e->interval = (float)source->interval;
		e->_lifetime = (float)source->lifetime;
		e->_life = (float)source->lifetime;
		e->burst = source->attimeCreate;

		e->undead = false;
		e->drawPriority = source->priority;

		if ( e->_lifetime == 0 ) e->undead = true;
		
	
	}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementBasic* source = static_cast<ParticleElementBasic*>(ele);
		SsVector3 eVec = e->getPosition();
		float eAngle = 0;

		p->_baseEmiterPosition.x = eVec.x;
		p->_baseEmiterPosition.y = eVec.y;
		p->_position.x = p->_baseEmiterPosition.x;
		p->_position.y = p->_baseEmiterPosition.y;
		p->_size = SsVector2( 1.0f , 1.0f );


		p->_color = SsU8Color(255,255,255,255) ;
		p->_startcolor = SsU8Color(255,255,255,255) ;
		p->_endcolor = p->_startcolor;


		p->_backposition = p->_position;

		p->_lifetime = VarianceCalc( e , (float)(source->lifespan.getMinValue()) , (float)(source->lifespan.getMaxValue()) );
		p->_life = (float)source->lifetime;
		float temp_angle = VarianceCalcFin( e ,  source->angle+eAngle , source->angleVariance/2.0f);

		float angle_rad = (float)(DegreeToRadian( (temp_angle+90.0f) ));
		float lspeed = VarianceCalc(  e , source->speed.getMinValue() , source->speed.getMaxValue() );

		p->speed = lspeed;
		p->firstspeed = lspeed;
		p->vector.x =  cos( angle_rad );
		p->vector.y =  sin( angle_rad );

		p->_force = SsVector2(0,0);//p->vector * p->speed;
		p->direction = 0;
		p->isTurnDirection = false;

		p->_rotation = 0;
		p->_rotationAdd = 0;
		p->_rotationAddDst = 0;
		p->_rotationAddOrg = 0;

		p->_rotation = 0;
	}
	
	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementBasic* source = static_cast<ParticleElementBasic*>(ele);
		e->priority = source->priority;

		//エミッターパラメータ
		e->emitter.emitmax = source->maximumParticle;
		e->emitter.interval = source->interval;
		e->emitter.life = source->lifetime;
		e->emitter.emitnum = source->attimeCreate;
		e->emitter.particleLife = 10;//
		e->emitter.Infinite = false;
		e->emitter.loopGen = 0;


		//パーティクルパラメータ
		e->emitter.particleLife = source->lifespan.getMinValue();
		e->emitter.particleLife2 = source->lifespan.getMaxValue() - source->lifespan.getMinValue();

		e->particle.scale = SsVector2( 1.0f , 1.0f );
		e->particle.startcolor = SsU8Color(255,255,255,255) ;
		e->particle.endcolor = SsU8Color(255,255,255,255) ;

		e->particle.speed = source->speed.getMinValue();
		e->particle.speed2 = source->speed.getMaxValue() - source->speed.getMinValue();

		e->particle.angle = (float)(DegreeToRadian( (source->angle+90.0f) ));
		e->particle.angleVariance = (float)(DegreeToRadian( source->angleVariance ));

		e->particle.useTanAccel = false;

		//重力
		e->particle.useGravity = false;
		e->particle.gravity = SsVector2(0,0);

		//オフセット
		e->particle.useOffset = false;
		e->particle.offset = SsVector2(0,0);
		e->particle.offset2 = SsVector2(0,0);


		//回転
		e->particle.useRotation = false;
		e->particle.useRotationTrans = false;

		//カラー
		e->particle.useColor = false;
		e->particle.useTransColor = false;

		//スケール
		e->particle.useInitScale = false;
 		e->particle.useTransScale = false;

		e->particle.delay = 0;

		e->particle.usePGravity = false;

		e->particle.useTransColor = false;
		e->particle.useInitScale = false;
		e->particle.usePGravity = false;
		e->particle.useAlphaFade = false;
		e->particle.useTransSpeed = false;
		e->particle.useTurnDirec = false;
		e->particle.userOverrideRSeed = false;
	}

};
static FuncParticleElementBasic		funcBasic;

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementRndSeedChange : public EffectFuncBase
{
public:
	FuncParticleElementRndSeedChange(){}
	virtual ~FuncParticleElementRndSeedChange(){}

	virtual void	initalizeEmmiter ( SsEffectElementBase* ele , SsEffectRenderEmitter* emmiter)	
	{
		ParticleElementRndSeedChange* source = static_cast<ParticleElementRndSeedChange*>(ele);
		emmiter->setMySeed( source->Seed );
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementRndSeedChange* source = static_cast<ParticleElementRndSeedChange*>(ele);
		e->particle.userOverrideRSeed = true;

		e->particle.overrideRSeed  =  source->Seed + SEED_MAGIC;
		e->emitterSeed = source->Seed + SEED_MAGIC;
	}
	
};
static FuncParticleElementRndSeedChange		funcRndSeedChange;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementDelay : public EffectFuncBase
{
public:
	FuncParticleElementDelay(){}
	virtual ~FuncParticleElementDelay(){}

	virtual void	initalizeEmmiter ( SsEffectElementBase* ele , SsEffectRenderEmitter* emmiter)
	{
		ParticleElementDelay* source = static_cast<ParticleElementDelay*>(ele);
		emmiter->delay = source->DelayTime;
		emmiter->_lifetime = emmiter->_lifetime + source->DelayTime;
		emmiter->_life = emmiter->_lifetime;
		emmiter->generate_ok = false;	
	}

	virtual void	updateEmmiter( SsEffectElementBase* ele , SsEffectRenderEmitter* emmiter)
	{
		ParticleElementDelay* source = static_cast<ParticleElementDelay*>(ele);
		//既定の時間までストップ？
		if ( emmiter->_exsitTime >= source->DelayTime )
		{
			emmiter->generate_ok = true;
		}	
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementDelay* source = static_cast<ParticleElementDelay*>(ele);
		e->particle.delay = (float)source->DelayTime;

	}
	
};
static FuncParticleElementDelay		funcDelay;

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementGravity : public EffectFuncBase
{
public:
	FuncParticleElementGravity(){}
	virtual ~FuncParticleElementGravity(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementGravity* source = static_cast<ParticleElementGravity*>(ele);
		p->_gravity = source->Gravity;
	}
	virtual void	updateParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* particle )
	{
		ParticleElementGravity* source = static_cast<ParticleElementGravity*>(ele);
		particle->_gravity = source->Gravity * particle->_exsitTime;	
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementGravity* source = static_cast<ParticleElementGravity*>(ele);
		e->particle.useGravity = true;
		e->particle.gravity = source->Gravity;

	}
};
static FuncParticleElementGravity		funcGravity;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementPosition : public EffectFuncBase
{
public:
	FuncParticleElementPosition(){}
	virtual ~FuncParticleElementPosition(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementPosition* source = static_cast<ParticleElementPosition*>(ele);
		p->_position.x = p->_baseEmiterPosition.x + VarianceCalc( e , source->OffsetX.getMinValue() , source->OffsetX.getMaxValue() );
		p->_position.y = p->_baseEmiterPosition.y + VarianceCalc( e , source->OffsetY.getMinValue() , source->OffsetY.getMaxValue() );
	}

	virtual void	updateParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* particle ){}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementPosition* source = static_cast<ParticleElementPosition*>(ele);
		e->particle.useOffset = true;
		e->particle.offset = SsVector2(source->OffsetX.getMinValue(), source->OffsetY.getMinValue());
		e->particle.offset2 = SsVector2( source->OffsetX.getMaxValue() - source->OffsetX.getMinValue() , source->OffsetY.getMaxValue() - source->OffsetY.getMinValue() );
	}
};
static FuncParticleElementPosition		funcPosition;


#if 0		//オミット
//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementTransPosition: public EffectFuncBase
{
public:
	FuncParticleElementTransPosition(){}
	virtual ~FuncParticleElementTransPosition(){}
	
};
static FuncParticleElementPosition		funcTransPosition;
#endif


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementRotation: public EffectFuncBase
{
public:
	FuncParticleElementRotation(){}
	virtual ~FuncParticleElementRotation(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementRotation* source = static_cast<ParticleElementRotation*>(ele);

		p->_rotation = VarianceCalc( e , source->Rotation.getMinValue() , source->Rotation.getMaxValue() ) ;
		p->_rotationAdd =  VarianceCalc( e , source->RotationAdd.getMinValue() , source->RotationAdd.getMaxValue() );
		p->_rotationAddDst = p->_rotationAdd;
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementRotation* source = static_cast<ParticleElementRotation*>(ele);
		e->particle.useRotation = true;
		e->particle.rotation = source->Rotation.getMinValue();
		e->particle.rotation2 = source->Rotation.getMaxValue() - source->Rotation.getMinValue();

		e->particle.rotationAdd = source->RotationAdd.getMinValue();
		e->particle.rotationAdd2 = source->RotationAdd.getMaxValue() - source->RotationAdd.getMinValue();

	}
};
static FuncParticleElementRotation		funcRotation ;



//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementRotationTrans : public EffectFuncBase
{
public:
	FuncParticleElementRotationTrans(){}
	virtual ~FuncParticleElementRotationTrans(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementRotationTrans* source = static_cast<ParticleElementRotationTrans*>(ele);
		if ( p->_lifetime == 0 ) return ;
		if ( source->EndLifeTimePer == 0 )
		{
			p->_rotationAddDst = p->_rotationAdd * source->RotationFactor;
			p->_rotationAddOrg = p->_rotationAdd ;
			return;
		}
		p->_rotationAddDst = p->_rotationAdd * source->RotationFactor;
		p->_rotationAddOrg = p->_rotationAdd;
	}
	virtual void	updateParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementRotationTrans* source = static_cast<ParticleElementRotationTrans*>(ele);

		if ( (p->_lifetime*source->EndLifeTimePer) == 0 )
		{
			p->_rotationAdd = blendFloat( p->_rotationAddOrg  , p->_rotationAddDst , 1.0f );
			return;
		}
		float per = ( (float)p->_exsitTime / ((float)p->_lifetime*( source->EndLifeTimePer / 100.0f))  );// * 100.0f;

		if ( per > 1.0f ) per = 1.0f;

		p->_rotationAdd = blendFloat( p->_rotationAddOrg  , p->_rotationAddDst , per );
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementRotationTrans* source = static_cast<ParticleElementRotationTrans*>(ele);
		e->particle.useRotationTrans = true;
		e->particle.rotationFactor = source->RotationFactor;
		e->particle.endLifeTimePer = source->EndLifeTimePer / 100.0f;
	}
};
static FuncParticleElementRotationTrans		funcRotationTrans ;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementTransSpeed : public EffectFuncBase
{
public:
	FuncParticleElementTransSpeed(){}
	virtual ~FuncParticleElementTransSpeed(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementTransSpeed* source = static_cast<ParticleElementTransSpeed*>(ele);
		p->lastspeed = VarianceCalc( e, source->Speed.getMinValue() , source->Speed.getMaxValue() );
	}

	virtual void	updateParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		//ParticleElementTransSpeed* source = static_cast<ParticleElementTransSpeed*>(ele);
		float per = ( (float)p->_exsitTime / (float)p->_lifetime );
		p->speed = ( p->firstspeed + ( p->lastspeed - p->firstspeed ) * per );
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementTransSpeed* source = static_cast<ParticleElementTransSpeed*>(ele);
		e->particle.useTransSpeed = true;
		e->particle.transSpeed = source->Speed.getMinValue();
		e->particle.transSpeed2 = source->Speed.getMaxValue() - source->Speed.getMinValue();
	}

};
static FuncParticleElementTransSpeed		funcTransSpeed ;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementTangentialAcceleration : public EffectFuncBase
{
public:
	FuncParticleElementTangentialAcceleration(){}
	virtual ~FuncParticleElementTangentialAcceleration(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementTangentialAcceleration* source = static_cast<ParticleElementTangentialAcceleration*>(ele);
		p->_tangentialAccel = VarianceCalc( e , source->Acceleration.getMinValue() , source->Acceleration.getMaxValue() );
	}
	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementTangentialAcceleration* source = static_cast<ParticleElementTangentialAcceleration*>(ele);
		e->particle.useTanAccel = true;
		e->particle.tangentialAccel = source->Acceleration.getMinValue();
		e->particle.tangentialAccel2 = ( source->Acceleration.getMaxValue() - source->Acceleration.getMinValue());

	}
};
static FuncParticleElementTangentialAcceleration		funcTangentialAcceleration ;

static void getRange( u8 a , u8 b , u8& min, u8& diff)
{
	min = a < b ? a : b;
	u8 max = a < b ? b : a;

	diff = ( max - min );
}

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementInitColor : public EffectFuncBase
{
public:
	FuncParticleElementInitColor(){}
	virtual ~FuncParticleElementInitColor(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementInitColor* source = static_cast<ParticleElementInitColor*>(ele);
		VarianceCalcColor( e , p->_startcolor , source->Color.getMinValue() , source->Color.getMaxValue() );
		p->_color = p->_startcolor;
	
	}


	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementInitColor* source = static_cast<ParticleElementInitColor*>(ele);
		e->particle.useColor = true;

		SsU8Color color1 = source->Color.getMinValue();
		SsU8Color color2 = source->Color.getMaxValue();

		getRange( color1.a , color2.a , e->particle.initColor.a , e->particle.initColor2.a );
		getRange( color1.r , color2.r , e->particle.initColor.r , e->particle.initColor2.r );
		getRange( color1.g , color2.g , e->particle.initColor.g , e->particle.initColor2.g );
		getRange( color1.b , color2.b , e->particle.initColor.b , e->particle.initColor2.b );

	}
};
static FuncParticleElementInitColor		funcInitColor;

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementTransColor : public EffectFuncBase
{
public:
	FuncParticleElementTransColor(){}
	virtual ~FuncParticleElementTransColor(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementTransColor* source = static_cast<ParticleElementTransColor*>(ele);
		VarianceCalcColor( e , p->_endcolor , source->Color.getMinValue() , source->Color.getMaxValue() );
	}

	virtual void	updateParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		float per = ( (float)p->_exsitTime / (float)p->_lifetime );

		if ( per > 1.0f )per = 1.0f;

		p->_color.a = blendNumber( p->_startcolor.a , p->_endcolor.a , per );
		p->_color.r = blendNumber( p->_startcolor.r , p->_endcolor.r , per  );
		p->_color.g = blendNumber( p->_startcolor.g , p->_endcolor.g , per  );
		p->_color.b = blendNumber( p->_startcolor.b , p->_endcolor.b , per  );
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementTransColor* source = static_cast<ParticleElementTransColor*>(ele);

		e->particle.useTransColor = true;

		SsU8Color color1 = source->Color.getMinValue();
		SsU8Color color2 = source->Color.getMaxValue();

		getRange( color1.a , color2.a , e->particle.transColor.a , e->particle.transColor2.a );
		getRange( color1.r , color2.r , e->particle.transColor.r , e->particle.transColor2.r );
		getRange( color1.g , color2.g , e->particle.transColor.g , e->particle.transColor2.g );
		getRange( color1.b , color2.b , e->particle.transColor.b , e->particle.transColor2.b );
	}
};
static FuncParticleElementTransColor		funcTransColor;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementAlphaFade : public EffectFuncBase
{
public:
	FuncParticleElementAlphaFade(){}
	virtual ~FuncParticleElementAlphaFade(){}

	virtual void	updateParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* particle )
	{
		ParticleElementAlphaFade* source = static_cast<ParticleElementAlphaFade*>(ele);
	
		if ( particle->_lifetime == 0 ) return ;

		float per = ( (float)particle->_exsitTime / (float)particle->_lifetime ) * 100.0f;

		float start = source->disprange.getMinValue();
		float end = source->disprange.getMaxValue();

		if ( per < start )
		{
			float alpha = (start - per) / start;
//			particle->_color.a*= 1.0f - alpha;
			particle->_color.a = (u8)((float)particle->_color.a * (1.0f - alpha));
			return;
		}

		if ( per > end )
		{

			if (end>=100.0f)
			{
				particle->_color.a = 0;
				return;
			}
			float alpha = (per-end) / (100.0f-end);
//			particle->_color.a*= 1.0f - alpha;
			particle->_color.a = (u8)((float)particle->_color.a * (1.0f - alpha));
			return;
		}

	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementAlphaFade* source = static_cast<ParticleElementAlphaFade*>(ele);
		e->particle.useAlphaFade = true;
		e->particle.alphaFade = source->disprange.getMinValue();
		e->particle.alphaFade2 = source->disprange.getMaxValue();

	}

};
static FuncParticleElementAlphaFade		funcAlphaFade;



//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementSize : public EffectFuncBase
{
public:
	FuncParticleElementSize(){}
	virtual ~FuncParticleElementSize(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{

		ParticleElementSize* source = static_cast<ParticleElementSize*>(ele);

		p->_size.x = VarianceCalc( e, source->SizeX.getMinValue() , source->SizeX.getMaxValue() );
		p->_size.y = VarianceCalc( e, source->SizeY.getMinValue() , source->SizeY.getMaxValue() );
		float sf   = VarianceCalc( e, source->ScaleFactor.getMinValue() , source->ScaleFactor.getMaxValue() );

		p->_size = p->_size * sf;
		p->_startsize = p->_size;	
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementSize* source = static_cast<ParticleElementSize*>(ele);

		e->particle.useInitScale = true;

		e->particle.scale.x = source->SizeX.getMinValue();
		e->particle.scaleRange.x = source->SizeX.getMaxValue() - source->SizeX.getMinValue();

		e->particle.scale.y = source->SizeY.getMinValue();
		e->particle.scaleRange.y = source->SizeY.getMaxValue() - source->SizeY.getMinValue();

		e->particle.scaleFactor = source->ScaleFactor.getMinValue();
		e->particle.scaleFactor2 = source->ScaleFactor.getMaxValue() - source->ScaleFactor.getMinValue();
	}

};
static FuncParticleElementSize		funcSize;

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleElementTransSize : public EffectFuncBase
{
public:
	FuncParticleElementTransSize(){}
	virtual ~FuncParticleElementTransSize(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticleElementTransSize* source = static_cast<ParticleElementTransSize*>(ele);
		SsVector2 endsize;
		endsize.x = VarianceCalc( e, source->SizeX.getMinValue() , source->SizeX.getMaxValue() );
		endsize.y = VarianceCalc( e, source->SizeY.getMinValue() , source->SizeY.getMaxValue() );

		float sf = VarianceCalc( e, source->ScaleFactor.getMinValue() , source->ScaleFactor.getMaxValue() );

		endsize = endsize * sf;

		p->_divsize = ( endsize - p->_startsize ) / p->_lifetime;	
	}
	virtual void	updateParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
	
		p->_size = p->_startsize + ( p->_divsize * ( p->_exsitTime ) );
	
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleElementTransSize* source = static_cast<ParticleElementTransSize*>(ele);
		e->particle.useTransScale = true;

		e->particle.transscale.x = source->SizeX.getMinValue();
		e->particle.transscaleRange.x = source->SizeX.getMaxValue() - source->SizeX.getMinValue();

		e->particle.transscale.y = source->SizeY.getMinValue();
		e->particle.transscaleRange.y = source->SizeY.getMaxValue() - source->SizeY.getMinValue();

		e->particle.transscaleFactor = source->ScaleFactor.getMinValue();
		e->particle.transscaleFactor2 = source->ScaleFactor.getMaxValue() - source->ScaleFactor.getMinValue();

	}
};
static FuncParticleElementTransSize		funcTransSize;


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticlePointGravity : public EffectFuncBase
{
public:
	FuncParticlePointGravity(){}
	virtual ~FuncParticlePointGravity(){}

	virtual void	initalizeEmmiter ( SsEffectElementBase* ele , SsEffectRenderEmitter* emmiter){}
	virtual void	updateEmmiter( SsEffectElementBase* ele , SsEffectRenderEmitter* emmiter){}
	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		//p->_orggravity = p->_gravity;	
	}
	virtual void	updateParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* p )
	{
		ParticlePointGravity* source = static_cast<ParticlePointGravity*>(ele);

		SsVector2 Target;
		Target.x = source->Position.x + p->parentEmitter->position.x;
		Target.y = source->Position.y + p->parentEmitter->position.y;

		//現在地点から指定された点に対してのベクトル*パワーを与える
		SsVector2 v2 = Target - p->_position;
		SsVector2 v2_temp = v2;

		SsVector2::normalize( v2 , &v2 );
		v2 = v2 * source->Power;

		p->_gravity = p->_gravity + v2;

	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticlePointGravity* source = static_cast<ParticlePointGravity*>(ele);
	   e->particle.usePGravity = true;
	   e->particle.gravityPos = source->Position;
//	   e->particle.gravityPower = source->Power / 100.0f;
	   e->particle.gravityPower = source->Power;

	}
};
static FuncParticlePointGravity		funcPointGravity;

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleTurnToDirectionEnabled : public EffectFuncBase
{
public:
	FuncParticleTurnToDirectionEnabled(){}
	virtual ~FuncParticleTurnToDirectionEnabled(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* particle )
	{
		particle->isTurnDirection = true;
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		ParticleTurnToDirectionEnabled* source = static_cast<ParticleTurnToDirectionEnabled*>(ele);
		e->particle.useTurnDirec = true;
		e->particle.direcRotAdd = source->Rotation;
	}
};
static FuncParticleTurnToDirectionEnabled		funcTurnToDirectionEnabled;



//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
class FuncParticleInfiniteEmitEnabled : public EffectFuncBase
{
public:
	FuncParticleInfiniteEmitEnabled(){}
	virtual ~FuncParticleInfiniteEmitEnabled(){}

	virtual void	initializeParticle( SsEffectElementBase* ele , SsEffectRenderEmitter* e , SsEffectRenderParticle* particle )
	{
	}

	virtual void	initalizeEffect ( SsEffectElementBase* ele , SsEffectEmitter* e)
	{
		e->emitter.Infinite = true;
	}
};
static FuncParticleInfiniteEmitEnabled		funcParticleInfiniteEmitEnabled;


//-------------------------------------------------------------------
//挙動反映クラスの呼び出しテーブル
//SsEffectFunctionTypeの順に並べること
//-------------------------------------------------------------------
static EffectFuncBase* callTable[] =
{
	0,
	&funcBasic,
	&funcRndSeedChange,
	&funcDelay,
	&funcGravity,
	&funcPosition,
	//&funcTransPosition,
	&funcRotation,
	&funcRotationTrans,
	&funcTransSpeed,
	&funcTangentialAcceleration,
	&funcInitColor,
	&funcTransColor,
	&funcAlphaFade,
	&funcSize,
	&funcTransSize,
	&funcPointGravity,
	&funcTurnToDirectionEnabled,
	&funcParticleInfiniteEmitEnabled,
};






///----------------------------------------------------------------------------------------------------
//
///----------------------------------------------------------------------------------------------------
void	SsEffectFunctionExecuter::initalize( SsEffectBehavior* beh ,  SsEffectRenderEmitter* emmiter)
{
	SPRITESTUDIO6SDK_foreach( std::vector<std::unique_ptr<SsEffectElementBase>> , beh->plist , e )
	{
		SsEffectElementBase* elementBase = e->get();
		EffectFuncBase* cf = callTable[elementBase->myType];
		cf->initalizeEmmiter( elementBase , emmiter );
	}
}

void	SsEffectFunctionExecuter::updateEmmiter( SsEffectBehavior* beh , SsEffectRenderEmitter* emmiter)
{
	SPRITESTUDIO6SDK_foreach( std::vector<std::unique_ptr<SsEffectElementBase>> , beh->plist , e )
	{
		SsEffectElementBase* elementBase = e->get();
		EffectFuncBase* cf = callTable[elementBase->myType];
		cf->updateEmmiter( elementBase , emmiter );
	}
}

void	SsEffectFunctionExecuter::initializeParticle( SsEffectBehavior* beh ,  SsEffectRenderEmitter* emmiter , SsEffectRenderParticle* particle )
{
	SPRITESTUDIO6SDK_foreach( std::vector<std::unique_ptr<SsEffectElementBase>> , beh->plist , e )
	{
		SsEffectElementBase* elementBase = e->get();
		EffectFuncBase* cf = callTable[elementBase->myType];
		cf->initializeParticle( elementBase , emmiter , particle );
	}
}

void	SsEffectFunctionExecuter::updateParticle(  SsEffectBehavior* beh , SsEffectRenderEmitter* emmiter , SsEffectRenderParticle* particle )
{
	SPRITESTUDIO6SDK_foreach( std::vector<std::unique_ptr<SsEffectElementBase>> , beh->plist , e )
	{
		SsEffectElementBase* elementBase = e->get();
		EffectFuncBase* cf = callTable[elementBase->myType];
		cf->updateParticle( elementBase , emmiter , particle );
	}
}


void	SsEffectFunctionExecuter::initializeEffect( SsEffectBehavior* beh ,  SsEffectEmitter* emmiter)
{
	SPRITESTUDIO6SDK_foreach( std::vector<std::unique_ptr<SsEffectElementBase>> , beh->plist , e )
	{
		SsEffectElementBase* elementBase = e->get();
		EffectFuncBase* cf = callTable[elementBase->myType];
		cf->initalizeEffect( elementBase , emmiter );
	}
}






}	// namespace spritestudio6
