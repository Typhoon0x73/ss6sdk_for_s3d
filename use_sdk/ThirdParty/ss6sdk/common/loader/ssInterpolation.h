#ifndef __SSINTERPOLATION__
#define __SSINTERPOLATION__

namespace spritestudio6
{

class SsCurve;

///補間でカーブパラメータが必要なタイプか判定する
inline bool SsNeedsCurveParams(SsInterpolationType::_enum type)
{
	switch (type)
	{
	case SsInterpolationType::bezier:
	case SsInterpolationType::hermite:
		return true;
	}
	return false;
}

///カーブパラメータ、補完方法により保管された値を生成する
SsVector2	SsInterpolate(SsInterpolationType::_enum ipType, float time, SsVector2 start, SsVector2 end, const SsCurve * curve);
float	SsInterpolate(SsInterpolationType::_enum type, float time, float start, float end, const SsCurve * curve);


}	// namespace spritestudio6

#endif
