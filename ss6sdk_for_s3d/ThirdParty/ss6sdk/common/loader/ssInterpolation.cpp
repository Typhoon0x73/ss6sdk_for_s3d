#include "ssloader.h"
#include "ssInterpolation.h"

namespace spritestudio6
{

//---------------------------------------------------------------------------
/**
	等速運動
*/
//---------------------------------------------------------------------------
static float	// nowにおける補間された値
linear_(
	float	start,	// 始点
	float	end,	// 終点
	float	now)	// 現在の時間 0.f~1.f
{
	return start + (now * (end - start));
}

//---------------------------------------------------------------------------
/**
	加速運動
*/
//---------------------------------------------------------------------------
static float	// nowにおける補間された値
accelerating_(
	float	start,	// 始点
	float	end,	// 終点
	float	now)	// 現在の時間 0.f~1.f
{
	float rate = now * now;
	return start + (rate * (end - start));
}

//---------------------------------------------------------------------------
/**
	減速運動
*/
//---------------------------------------------------------------------------
static float	// nowにおける補間された値
decelerating_(
	float	start,	// 始点
	float	end,	// 終点
	float	now)	// 現在の時間 0.f~1.f
{
	float time = 1 - now;
	float rate = 1 - time * time;
	return start + (rate * (end - start));
}


static float bezier_(float start, float end, float time, const SsCurve * c)
{
	//値が変化しない場合は左キーを補間値とする
	if ((start == end) && (c->startValue == 0.0f) && (c->endValue == 0.0f))
	{
		return start;
	}


	float fCurrentPos = (c->endKeyTime - c->startKeyTime) * time + c->startKeyTime;

	float fRet = end;
	float fCurrentCalc = 0.5f;
	float fCalcRange = 0.5f;

	float fTemp1;
	float fTemp2;
	float fTemp3;

	float fCurrentX;

	for(int iLoop = 0; iLoop < 8; iLoop++ )
	{// more count of loop, better precision increase
		fTemp1 = 1.0f - fCurrentCalc;
		fTemp2 = fTemp1 * fTemp1;
		fTemp3 = fTemp2 * fTemp1;
		fCurrentX = ( fTemp3 * c->startKeyTime ) +
					( 3 * fTemp2 * fCurrentCalc * (c->startTime + c->startKeyTime) ) +
					( 3 * fTemp1 * fCurrentCalc * fCurrentCalc * (c->endTime + c->endKeyTime) ) +
					( fCurrentCalc * fCurrentCalc * fCurrentCalc * c->endKeyTime);

		fCalcRange /= 2.0f;
		if( fCurrentX > fCurrentPos )
		{
			fCurrentCalc -= fCalcRange;
		}
		else
		{
			fCurrentCalc += fCalcRange;
		}
	}

	// finally calculate with current value
	fTemp1 = 1.0f - fCurrentCalc;
	fTemp2 = fTemp1 * fTemp1;
	fTemp3 = fTemp2 * fTemp1;
	fRet = ( fTemp3 * start ) +
				( 3 * fTemp2 * fCurrentCalc * (c->startValue + start) ) +
				( 3 * fTemp1 * fCurrentCalc * fCurrentCalc * (c->endValue + end) ) +
				( fCurrentCalc * fCurrentCalc * fCurrentCalc * end );

	return fRet;
}

/**
	エルミートでは c->startTime, c->endTime は必要ない

	スロープ値を事前計算しておけばカーブ計算用パラメータは１つになる
	が、ベジェと共用するためこのままの形にしておく。
*/
static float hermite_(float start, float end, float time, const SsCurve * c)
{
	float t2 = time * time;
	float t3 = t2 * time;
	float result =
		(2 * t3 - 3 * t2 + 1) * start +
		(-2 * t3 + 3 * t2) * end +
		(t3 - 2 * t2 + time) * (c->startValue - start) +
		(t3 - t2) * (c->endValue - end);
	return result;
}


/// SsVector2 のメンバ全体の補間
SsVector2	SsInterpolate(SsInterpolationType::_enum ipType, float time, SsVector2 start, SsVector2 end, const SsCurve * curve)
{
	SsVector2 out;
	out.x = SsInterpolate(ipType, time, start.x, end.x, curve);
	out.y = SsInterpolate(ipType, time, start.y, end.y, curve);

	return(out);
}

//----------------------------------------------------------------------------
/**
	タイプを指定して補間する
*/
//----------------------------------------------------------------------------
float	SsInterpolate(SsInterpolationType::_enum type, float time, float start, float end, const SsCurve * curve)
{
	float r = start;
	switch (type)
	{
	case SsInterpolationType::none:
//		r = start;
		break;
	case SsInterpolationType::linear:
		r = linear_(start, end, time);
		break;
	case SsInterpolationType::acceleration:
		r = accelerating_(start, end, time);
		break;
	case SsInterpolationType::deceleration:
		r = decelerating_(start, end, time);
		break;
	case SsInterpolationType::bezier:
		r = bezier_(start, end, time, curve);
		break;
	case SsInterpolationType::hermite:
		r = hermite_(start, end, time, curve);
		break;
	default:
		//SS_ASSERT_ID(type);
		break;
	}
	return r;
}

}	// namespace spritestudio6
