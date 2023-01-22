#ifndef __SSPLAYER_MACRO__
#define __SSPLAYER_MACRO__


namespace spritestudio6
{

	constexpr auto __PI__ = (3.14159265358979323846f);
	// #define RadianToDegree(Radian) ((double)Radian * (180.0f / __PI__))
	template<typename T> inline double RadianToDegree(T Radian)
	{
		return((double)Radian * (180.0f / __PI__));
	}
	// #define DegreeToRadian(Degree) ((double)Degree * (__PI__ / 180.0f))
	template<typename T> inline double DegreeToRadian(T Degree)
	{
		return((double)Degree * (__PI__ / 180.0f));
	}

	#define SPRITESTUDIO6SDK_foreach(T, c, i) for(T::iterator i = c.begin(); i!=c.end(); ++i)

}	// namespace spritestudio6

#endif
