#ifndef __SSEFFECTELEMENT__
#define __SSEFFECTELEMENT__


#include "sstypes.h"
#include "ssarchiver.h"
#include "SsEffectElement.h"

namespace spritestudio6
{

enum EffectPartType
{
	EffectPartTypeEmiiter,
	EffectPartTypeParticle

};


//命令種別
namespace SsEffectFunctionType
{
	enum enum_
	{
		Base,
		Basic	,
		RndSeedChange , 
		Delay,
		Gravity,
		Position,
		//TransPosition,
		Rotation,
		TransRotation,
		TransSpeed,
		TangentialAcceleration,
		InitColor,
		TransColor,
		AlphaFade,
		Size,
		TransSize,
		PointGravity,
		TurnToDirectionEnabled,
		InfiniteEmitEnabled,
	};
}

class SsCell;





//範囲値クラス
template<class mytype>
class VarianceValue : public SsXmlRangeValueConverter
{
private:

	enum RangeType { None, MinMax, PlusMinus,};

	RangeType 	type;
	mytype 		value;
	mytype 		subvalue;


public:

	VarianceValue( mytype v ){
		value = v;
		subvalue = v;
        type = VarianceValue::None;
	}
	VarianceValue( mytype v , mytype v2){
		value = v;
		subvalue = v2;
        type = VarianceValue::MinMax;
	}

	void	setPlusMinus( mytype v1  )
	{
		value = -v1;
		subvalue = v1;
        type = VarianceValue::PlusMinus;
	}

	void	setMinMax(mytype min , mytype max)
	{
		value = min;
		subvalue = max;
        type = VarianceValue::MinMax;
	}

	mytype	getValue(){ return value;}
	mytype	getMinValue(){ return value;}
	mytype	getMaxValue(){ return subvalue;}

	bool	isTypeNone(){ return  type == VarianceValue::None; }
	bool	isTypeMinMax(){ return  type == VarianceValue::MinMax; }
	bool	isTypePlusMinus(){ return  type == VarianceValue::PlusMinus; }

	mytype* getlpValue(){ return &value;}
	mytype* getlpSubValue(){ return &subvalue;}

	VarianceValue& operator=(mytype v) {  value = v; return *this; }
	operator mytype() { return value; }

private:
	virtual bool	inputString( SsString value , SsString subvalue )
	{
	 return true;
	}
};


typedef VarianceValue<float>   			f32VValue;
typedef VarianceValue<int>    			i32VValue;
typedef VarianceValue<SsU8Color>    	SsU8cVValue;


template<> inline bool VarianceValue<float>::inputString( SsString _value , SsString _subvalue )
{ 
	value = (float)atof(_value.c_str());
	subvalue = (float)atof(_subvalue.c_str());

	return true;
}
template<> inline bool VarianceValue<int>::inputString( SsString _value , SsString _subvalue )
{ 
	value = atoi(_value.c_str());
	subvalue = atoi(_subvalue.c_str());

	return true;
}
template<> inline bool VarianceValue<SsU8Color>::inputString( SsString _value , SsString _subvalue )
{ 
	u32 a = strtoul( _value.c_str(), 0 , 16);
	u32 b = strtoul( _subvalue.c_str(), 0 , 16);
	value.fromARGB( a );
	subvalue.fromARGB( b );
	return true;
}




class SsEffectRenderEmitter;
class SsEffectRenderParticle;


class   SsEffectElementBase 
{
public:
	//SsString            myName;
	SsEffectFunctionType::enum_ myType;

public:
	SsEffectElementBase() : myType (SsEffectFunctionType::Base) {}
	virtual ~SsEffectElementBase(){}

	void	setType(SsEffectFunctionType::enum_ type){ myType = type; } 

#if 0
	//各部で実装する
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter ) {}
	virtual void UpdateEmmiter( SsEffectRenderEmitter* emmiter ){}
	virtual void UpdateEndEmmiter( SsEffectRenderEmitter* emmiter ){}
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle ){}
	virtual void UpdateParticle( SsEffectRenderParticle* particle ){}
#endif
	//シリアライザ

	virtual SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		ar = ar;
	}

};



//--------------------------------------------------------------------------------------
//パーティクルを構成する基本の値
class  ParticleElementBasic  : public SsEffectElementBase
{
public:
	int			maximumParticle;
	f32VValue	speed;
	i32VValue 	lifespan;
	float		angle;
	float		angleVariance;
	int			interval;
	int			lifetime;
	int			attimeCreate;
    int			priority;

public:
	ParticleElementBasic()
			: 	maximumParticle( 50 ),
				speed( 5.0f , 5.0f),
				lifespan( 30 ,30 ),
				angle(0.0f),
				angleVariance(45.0f),
				interval(1),
				lifetime(30),
				attimeCreate(1),
                priority(64)
	{
		setType( SsEffectFunctionType::Basic );
	}

	virtual ~ParticleElementBasic(){}
/*
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter );
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle ){}
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( maximumParticle );
		SPRITESTUDIO6SDK_SSAR_DECLARE( speed );
		SPRITESTUDIO6SDK_SSAR_DECLARE( lifespan );
		SPRITESTUDIO6SDK_SSAR_DECLARE( angle );
		SPRITESTUDIO6SDK_SSAR_DECLARE( angleVariance );
		SPRITESTUDIO6SDK_SSAR_DECLARE( interval );
		SPRITESTUDIO6SDK_SSAR_DECLARE( lifetime );
		SPRITESTUDIO6SDK_SSAR_DECLARE( attimeCreate );
		SPRITESTUDIO6SDK_SSAR_DECLARE( priority );
	}

};


//--------------------------------------------------------------------------------------
//遅れ
class  ParticleElementRndSeedChange : public SsEffectElementBase
{
public:
	int		Seed;

	ParticleElementRndSeedChange()

		:	Seed( 0 )
	{
		setType( SsEffectFunctionType::RndSeedChange );
	}
	virtual ~ParticleElementRndSeedChange(){}
/*
	//各部で実装する
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter );
*/

	//シリアライザ
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( Seed );
	}

};

//--------------------------------------------------------------------------------------
//遅れ
class  ParticleElementDelay : public SsEffectElementBase
{
public:
	int		DelayTime;

	ParticleElementDelay()
		:	DelayTime( 0 )
	{
		setType( SsEffectFunctionType::Delay );
	}
	virtual ~ParticleElementDelay(){}
/*
	//各部で実装する
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter );
	virtual void UpdateEmmiter( SsEffectRenderEmitter* emmiter );

	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle ){}
	virtual void UpdateParticle( SsEffectRenderParticle* particle ){}

	virtual void UpdateEndEmmiter( SsEffectRenderEmitter* emmiter );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( DelayTime );
	}


};


//--------------------------------------------------------------------------------------
//重力への影響
class  ParticleElementGravity : public SsEffectElementBase
{
public:

	SsVector2   Gravity;

public:
	ParticleElementGravity()
        : Gravity(	0	,	-3.0f	)
    {
		setType( SsEffectFunctionType::Gravity );

	}
	virtual ~ParticleElementGravity(){}
/*
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( Gravity );
	}

};

//--------------------------------------------------------------------------------------
//発生位置への影響
class  ParticleElementPosition : public SsEffectElementBase
{
public:
	f32VValue   OffsetX;
	f32VValue   OffsetY;


	ParticleElementPosition()
		: OffsetX(0,0),OffsetY(0,0)
	{
		setType( SsEffectFunctionType::Position );
	}
	virtual ~ParticleElementPosition(){}
/*
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementPosition(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( OffsetX );
		SPRITESTUDIO6SDK_SSAR_DECLARE( OffsetY );
	}


};

#if 0	//オミット
//--------------------------------------------------------------------------------------
//発生位置への影響
class  ParticleElementTransPosition : public SsEffectElementBase
{
public:
	f32VValue   OffsetX;
	f32VValue   OffsetY;


	ParticleElementTransPosition()
		: OffsetX(0,0),OffsetY(0,0)
	{
		setType( SsEffectFunctionType::TransPosition );

	}
	virtual ~ParticleElementTransPosition(){}
	/*
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementTransPosition(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle ){}
	*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( OffsetX );
		SPRITESTUDIO6SDK_SSAR_DECLARE( OffsetY );
	}

};
#endif


//--------------------------------------------------------------------------------------
//角度変化
class  ParticleElementRotation : public SsEffectElementBase
{
public:
	f32VValue   Rotation;
	f32VValue   RotationAdd;

	ParticleElementRotation()
		:  Rotation( 0 ,0 ),
		   RotationAdd(0,0)
	{
		setType( SsEffectFunctionType::Rotation );

	}
	virtual ~ParticleElementRotation(){}
/*
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementRotation(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
*/
	//シリアライザ
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( Rotation );
		SPRITESTUDIO6SDK_SSAR_DECLARE( RotationAdd );
	}


};

//--------------------------------------------------------------------------------------
//角度変化
class  ParticleElementRotationTrans : public SsEffectElementBase
{
public:
	float   RotationFactor;
	float	EndLifeTimePer;

	ParticleElementRotationTrans()
		:  	RotationFactor( 0 ) ,
			EndLifeTimePer( 75 )
	{
		setType( SsEffectFunctionType::TransRotation );

	}
	virtual ~ParticleElementRotationTrans(){}
/*
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementRotationTrans(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
 	virtual void UpdateParticle( SsEffectRenderParticle* particle );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( RotationFactor );
		SPRITESTUDIO6SDK_SSAR_DECLARE( EndLifeTimePer );
	}


};

//--------------------------------------------------------------------------------------
class  ParticleElementTransSpeed : public SsEffectElementBase
{
public:
	f32VValue	Speed;

	ParticleElementTransSpeed()
        :	Speed( 0,0 )
	{
		setType( SsEffectFunctionType::TransSpeed );
	}
	virtual ~ParticleElementTransSpeed(){}
/*
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementTransSpeed(); }

	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
 	virtual void UpdateParticle( SsEffectRenderParticle* particle );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( Speed );
	}
};


//--------------------------------------------------------------------------------------
//接戦加速度を与える
class  ParticleElementTangentialAcceleration : public SsEffectElementBase
{
public:
	f32VValue	Acceleration;

	ParticleElementTangentialAcceleration()
			: Acceleration( 0, 0 )
	{
		setType( SsEffectFunctionType::TangentialAcceleration );

	}
	virtual ~ParticleElementTangentialAcceleration(){}
/*
	virtual    SsEffectElementBase*  new_(){ return new ParticleElementTangentialAcceleration(); }

	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( Acceleration );
	}

};


//--------------------------------------------------------------------------------------
//頂点カラーを制御する
class  ParticleElementInitColor : public SsEffectElementBase
{
public:
	SsU8cVValue Color;

	ParticleElementInitColor()
		: Color( SsU8Color(255,255,255,255) , SsU8Color(255,255,255,255) )
	{
		setType( SsEffectFunctionType::InitColor );
	}
	virtual ~ParticleElementInitColor(){}
/*
	virtual	SsEffectElementBase*  new_(){ return new ParticleElementInitColor(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( Color );
	}
};

//--------------------------------------------------------------------------------------
//頂点カラーを制御する
class  ParticleElementTransColor : public SsEffectElementBase
{
public:
	SsU8cVValue Color;

	ParticleElementTransColor()
		: Color( SsU8Color(255,255,255,255) , SsU8Color(255,255,255,255) )
	{
		setType( SsEffectFunctionType::TransColor );
	}
	virtual ~ParticleElementTransColor(){}
/*
	virtual	SsEffectElementBase*  new_(){ return new ParticleElementTransColor(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( Color );
	}

};


//--------------------------------------------------------------------------------------
//著点カラーを制御する
class  ParticleElementAlphaFade : public SsEffectElementBase
{
public:
	f32VValue  disprange;


	ParticleElementAlphaFade()
		: disprange(25,75)
	{
		setType( SsEffectFunctionType::AlphaFade );
	}
	virtual ~ParticleElementAlphaFade(){}
/*
	virtual	SsEffectElementBase*  new_(){ return new ParticleElementAlphaFade(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( disprange );
	}


};

//--------------------------------------------------------------------------------------
//サイズ初期
class  ParticleElementSize : public SsEffectElementBase
{
public:
	f32VValue SizeX;
	f32VValue SizeY;
	f32VValue ScaleFactor;

	ParticleElementSize()
		: 	SizeX( 1.0f , 1.0f ),
			SizeY( 1.0f , 1.0f ),
			ScaleFactor( 1.0f , 1.0f )
	{
		setType( SsEffectFunctionType::Size );
	}
	virtual ~ParticleElementSize(){}
/*
	virtual SsEffectElementBase*  new_(){ return new ParticleElementSize(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( SizeX );
		SPRITESTUDIO6SDK_SSAR_DECLARE( SizeY );
		SPRITESTUDIO6SDK_SSAR_DECLARE( ScaleFactor );
	}


};


//--------------------------------------------------------------------------------------
//サイズ変更
class  ParticleElementTransSize : public SsEffectElementBase
{
public:
	f32VValue SizeX;
	f32VValue SizeY;
	f32VValue ScaleFactor;


	ParticleElementTransSize()
		: 	SizeX( 1.0f , 1.0f ),
			SizeY( 1.0f , 1.0f ),
			ScaleFactor( 1.0f , 1.0f )
	{
		setType( SsEffectFunctionType::TransSize );
	}
	virtual ~ParticleElementTransSize(){}
/*
	virtual SsEffectElementBase*  new_(){ return new ParticleElementTransSize(); }
	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( SizeX );
		SPRITESTUDIO6SDK_SSAR_DECLARE( SizeY );
		SPRITESTUDIO6SDK_SSAR_DECLARE( ScaleFactor );
	}

};

//--------------------------------------------------------------------------------------
//重力点
class  ParticlePointGravity : public SsEffectElementBase
{
public:

	SsVector2   Position;
    float		Power;



	ParticlePointGravity()
		: Position( 0 , 0 ) ,Power(0.0f)
	{
		setType( SsEffectFunctionType::PointGravity );
	}
	virtual ~ParticlePointGravity(){}

	/*
	virtual SsEffectElementBase*  new_(){ return new ParticlePointGravity(); }
	//各部で実装する
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter ){}
	virtual void UpdateEmmiter( SsEffectRenderEmitter* emmiter ){}

	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle );
*/

	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( Position );
		SPRITESTUDIO6SDK_SSAR_DECLARE( Power );
	}

};




//--------------------------------------------------------------------------------------
class  ParticleTurnToDirectionEnabled : public SsEffectElementBase
{
public:
	float Rotation;


	ParticleTurnToDirectionEnabled()
		: Rotation(0.0f)
	{
		setType( SsEffectFunctionType::TurnToDirectionEnabled );
	}
	virtual ~ParticleTurnToDirectionEnabled(){}
/*
	virtual SsEffectElementBase*  new_(){ return new ParticleTurnToDirectionEnabled(); }

	//各部で実装する
	virtual void InitializeEmmiter( SsEffectRenderEmitter* emmiter ){}
	virtual void UpdateEmmiter( SsEffectRenderEmitter* emmiter ){}


	virtual void InitializeParticle( SsEffectRenderEmitter* e , SsEffectRenderParticle* particle );
	virtual void UpdateParticle( SsEffectRenderParticle* particle ){}
*/
	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		SPRITESTUDIO6SDK_SSAR_DECLARE( Rotation );
	}
};


//--------------------------------------------------------------------------------------
class  ParticleInfiniteEmitEnabled : public SsEffectElementBase
{
public:

	ParticleInfiniteEmitEnabled()
	{
		setType( SsEffectFunctionType::InfiniteEmitEnabled );
	}
	virtual ~ParticleInfiniteEmitEnabled(){}

	SPRITESTUDIO6SDK_SERIALIZE_BLOCK
	{
		// 警告を抑制するために追記
		SPRITESTUDIO6SDK_NOUSE_ARGUMENT(ar);
	}
};




}	// namespace spritestudio6

#endif
