#ifndef __SSPLAYER_MATRIX__
#define __SSPLAYER_MATRIX__

#include "../Loader/sstypes.h"
#include <memory>

namespace spritestudio6
{

void	IdentityMatrix( float* matrix );
void    ScaleMatrix( float* _matrix , const float x , const float y , const float z);
void    TranslationMatrix( float* _matrix , const float x , const float y , const float z );
void	MultiplyMatrix(const float *m0, const float *m1, float *matrix);
void    Matrix4RotationX( float* _matrix ,const float radians );
void    Matrix4RotationY( float* _matrix ,const float radians );
void    Matrix4RotationZ( float* _matrix ,const float radians );

void	MatrixTransformVector3(float* _matrix, SsVector3& src, SsVector3& dst);



inline	void	TranslationMatrixM(  float* _matrix , const float x , const float y , const float z )
{
	float	_m[16];
	IdentityMatrix( _m );
	TranslationMatrix( _m , x , y , z );

	MultiplyMatrix( _m , _matrix , _matrix );
}

inline	void	ScaleMatrixM(  float* _matrix , const float x , const float y , const float z )
{

	float	_m[16];
	IdentityMatrix( _m );
	ScaleMatrix( _m , x , y , z );
	MultiplyMatrix( _m , _matrix , _matrix );
}

inline	void	RotationXYZMatrixM(  float* _matrix , const float x , const float y , const float z )
{

	if ( x != 0.0f )
	{
		float	_m[16];
		IdentityMatrix( _m );
		Matrix4RotationX( _m , x );

		MultiplyMatrix( _m , _matrix , _matrix );
	}

	if ( y != 0.0f )
	{
		float	_m[16];
		IdentityMatrix( _m );
		Matrix4RotationY( _m , y );

		MultiplyMatrix( _m , _matrix , _matrix );
	}

	if ( z != 0.0f )
	{
		float	_m[16];
		IdentityMatrix( _m );
		Matrix4RotationZ( _m , z );

		MultiplyMatrix( _m , _matrix , _matrix );
	}
}


class SsOpenGLMatrix
{
public:
	float _matrix[16];


public:
	SsOpenGLMatrix()
	{
		identityMatrix();
	}
	SsOpenGLMatrix(float* src)
	{
		identityMatrix();
		pushMatrix(src);
	}
	virtual ~SsOpenGLMatrix() {}

	//単位行列の作成
	void	identityMatrix();


	void	popMatrix(float* dst);
	void	pushMatrix(float* src);


	inline void	Scaling(const float x, const float y, const float z)
	{
		SsOpenGLMatrix m;
		m.setScaleMatrix(x, y, z);
		this->multiply(m, *this);
	}

	inline	void	Translation(const float x, const float y, const float z)
	{
		SsOpenGLMatrix m;
		m.setTranslationMatrix(x, y, z);
		this->multiply(m, *this);
	}


	void	setScaleMatrix(const float x, const float y, const float z);
	void    setTranslationMatrix(const float x, const float y, const float z);
	void 	multiply(SsOpenGLMatrix& m1, SsOpenGLMatrix& m2);

	// 4x4の逆行列の計算
	void	inverseMatrix();


	float 	getPositionX() { return _matrix[12]; }
	float 	getPositionY() { return _matrix[12 + 1]; }

	void  	TransformVector3(SsVector3& in, SsVector3& out);
//	void  	TransformVector4(SsVector4& src, SsVector4& dst);



	inline void RotationXYZ(const float x, const float y, const float z)
	{
		if (x != 0.0f)
		{
			SsOpenGLMatrix mx;
			SsOpenGLMatrix temp(this->_matrix);
			mx.Matrix4RotationX(x);
			this->multiply(mx, temp);
		}

		if (y != 0.0f)
		{
			SsOpenGLMatrix my;
			SsOpenGLMatrix temp(this->_matrix);
			my.Matrix4RotationY(y);
			this->multiply(my, temp);
		}

		if (z != 0.0f)
		{
			SsOpenGLMatrix mz;
			SsOpenGLMatrix temp(this->_matrix);
			mz.Matrix4RotationZ(z);
			this->multiply(mz, temp);
		}
	}

	void    Matrix4RotationX(const float radians);
	void    Matrix4RotationY(const float radians);
	void    Matrix4RotationZ(const float radians);
private:


};


}	// namespace spritestudio6

#endif

