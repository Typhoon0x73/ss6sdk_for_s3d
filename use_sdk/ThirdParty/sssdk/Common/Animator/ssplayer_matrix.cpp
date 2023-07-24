#include "ssplayer_matrix.h"
#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <memory.h>
#include <math.h>

namespace spritestudio6
{


void	IdentityMatrix( float* matrix )
{

	float ident[] = {
		1.0f , 0.0f , 0.0f , 0.0f ,
		0.0f , 1.0f , 0.0f , 0.0f ,
		0.0f , 0.0f , 1.0f , 0.0f ,
		0.0f , 0.0f , 0.0f , 1.0f };


	memcpy( matrix , ident , sizeof(float) * 16 );

}


void    ScaleMatrix( float* _matrix , const float x , const float y , const float z)
{
	memset( _matrix , 0 , sizeof(float) * 16 );

	 _matrix[ 4 * 0 + 0 ] = x;
	 _matrix[ 4 * 1 + 1 ] = y;
	 _matrix[ 4 * 2 + 2 ] = z;
	 _matrix[ 4 * 3 + 3 ] = 1.0f;


}

void    TranslationMatrix( float* _matrix , const float x , const float y , const float z )
{
	memset( _matrix , 0 , sizeof(float) * 16 );

	_matrix[ 0 ] = 1.0f;
	_matrix[ 5 ] = 1.0f;
	_matrix[ 10 ] = 1.0f;


	_matrix[ 12 ] = x;
	_matrix[ 13 ] = y;
	_matrix[ 14 ] = z;
	_matrix[ 15 ] = 1.0f;

}


void MultiplyMatrix(const float *m0, const float *m1, float *matrix)
{

	float _temp[16];

    for (int i = 0; i < 16; ++i) {
        int j = i & ~3, k = i & 3;

        _temp[i] = m0[j + 0] * m1[ 0 + k]
        + m0[j + 1] * m1[ 4 + k]
        + m0[j + 2] * m1[ 8 + k]
        + m0[j + 3] * m1[12 + k];
    }

	memcpy( matrix , _temp , sizeof(float) * 16 );


}



void    Matrix4RotationX( float* _matrix ,const float radians )
{

	_matrix[0] = 1.0f;
	_matrix[1] = 0.0f;
	_matrix[2] = 0.0f;
	_matrix[3] = 0.0f;

	_matrix[4] = 0.0f;
	_matrix[5] = cosf(radians);
	_matrix[6] = sinf(radians);
	_matrix[7] = 0.0f;

	_matrix[8] = 0.0f;
	_matrix[9] = -sinf(radians);
	_matrix[10] = cosf(radians);
	_matrix[11] = 0.0f;

	_matrix[12] = 0.0f;
	_matrix[13] = 0.0f;
	_matrix[14] = 0.0f;
	_matrix[15] = 1.0f;


}


void    Matrix4RotationY( float* _matrix ,const float radians )
{

	_matrix[0] = cosf(radians);
	_matrix[1] = 0.0f;
	_matrix[2] = -sinf(radians);
	_matrix[3] = 0.0f;

	_matrix[4] = 0.0f;
	_matrix[5] = 1.0f;
	_matrix[6] = 0.0f;
	_matrix[7] = 0.0f;

	_matrix[8] = sinf(radians);
	_matrix[9] = 0.0f;
	_matrix[10] = cosf(radians);
	_matrix[11] = 0.0f;

	_matrix[12] = 0.0f;
	_matrix[13] = 0.0f;
	_matrix[14] = 0.0f;
	_matrix[15] = 1.0f;


}



void    Matrix4RotationZ( float* _matrix ,const float radians )
{
	_matrix[0] = cosf(radians);
	_matrix[1] = sinf(radians);
	_matrix[2] = 0.0f;
	_matrix[3] = 0.0f;

	_matrix[4] = -sinf(radians);
	_matrix[5] = cosf(radians);
	_matrix[6] = 0.0f;
	_matrix[7] = 0.0f;

	_matrix[8] = 0.0f;
	_matrix[9] = 0.0f;
	_matrix[10] = 1.0f;
	_matrix[11] = 0.0f;

	_matrix[12] = 0.0f;
	_matrix[13] = 0.0f;
	_matrix[14] = 0.0f;
	_matrix[15] = 1.0f;

}



void  MatrixTransformVector3(float* _matrix  , SsVector3& src, SsVector3& dst)
{
	float vx, vy, vz;
	//SsVector3 vec;
	float *pF1, *pF2, *pF3, *pF4;

	vx = src.x;
	vy = src.y;
	vz = src.z;

	pF1 = &_matrix[0];
	pF2 = &_matrix[4];
	pF3 = &_matrix[8];
	pF4 = &_matrix[12];

	dst.x = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + (*pF4);
	pF1 += 1; pF2 += 1; pF3 += 1; pF4 += 1;

	dst.y = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + (*pF4);
	pF1 += 1; pF2 += 1; pF3 += 1; pF4 += 1;

	dst.z = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + (*pF4);
	pF1 += 1; pF2 += 1; pF3 += 1; pF4 += 1;

	//dst = vec;

}

void	SsOpenGLMatrix::identityMatrix()
{
	float ident[] = {
		1.0f , 0.0f , 0.0f , 0.0f ,
		0.0f , 1.0f , 0.0f , 0.0f ,
		0.0f , 0.0f , 1.0f , 0.0f ,
		0.0f , 0.0f , 0.0f , 1.0f };


	memcpy(_matrix, ident, sizeof(float) * 16);
}



void	SsOpenGLMatrix::popMatrix(float* dst)
{
	memcpy(dst, _matrix, sizeof(float) * 16);
}

void	SsOpenGLMatrix::pushMatrix(float* src)
{
	memcpy(_matrix, src, sizeof(float) * 16);
}


#if 1

void    SsOpenGLMatrix::Matrix4RotationX(const float radians)
{

	_matrix[0] = 1.0f;
	_matrix[1] = 0.0f;
	_matrix[2] = 0.0f;
	_matrix[3] = 0.0f;

	_matrix[4] = 0.0f;
	_matrix[5] = cosf(radians);
	_matrix[6] = sinf(radians);
	_matrix[7] = 0.0f;

	_matrix[8] = 0.0f;
	_matrix[9] = -sinf(radians);
	_matrix[10] = cosf(radians);
	_matrix[11] = 0.0f;

	_matrix[12] = 0.0f;
	_matrix[13] = 0.0f;
	_matrix[14] = 0.0f;
	_matrix[15] = 1.0f;


}


void    SsOpenGLMatrix::Matrix4RotationY(const float radians)
{
	/*
	|  cos(A)  0   sin(A)  0 |
	M = |  0       1   0       0 |
	| -sin(A)  0   cos(A)  0 |
	|  0       0   0       1 |
	*/

	_matrix[0] = cosf(radians);
	_matrix[1] = 0.0f;
	_matrix[2] = -sinf(radians);
	_matrix[3] = 0.0f;

	_matrix[4] = 0.0f;
	_matrix[5] = 1.0f;
	_matrix[6] = 0.0f;
	_matrix[7] = 0.0f;

	_matrix[8] = sinf(radians);
	_matrix[9] = 0.0f;
	_matrix[10] = cosf(radians);
	_matrix[11] = 0.0f;

	_matrix[12] = 0.0f;
	_matrix[13] = 0.0f;
	_matrix[14] = 0.0f;
	_matrix[15] = 1.0f;


}



void    SsOpenGLMatrix::Matrix4RotationZ(const float radians)
{
	_matrix[0] = cosf(radians);
	_matrix[1] = sinf(radians);
	_matrix[2] = 0.0f;
	_matrix[3] = 0.0f;

	_matrix[4] = -sinf(radians);
	_matrix[5] = cosf(radians);
	_matrix[6] = 0.0f;
	_matrix[7] = 0.0f;

	_matrix[8] = 0.0f;
	_matrix[9] = 0.0f;
	_matrix[10] = 1.0f;
	_matrix[11] = 0.0f;

	_matrix[12] = 0.0f;
	_matrix[13] = 0.0f;
	_matrix[14] = 0.0f;
	_matrix[15] = 1.0f;

}

#endif



void    SsOpenGLMatrix::setScaleMatrix(const float x, const float y, const float z)
{
	memset(_matrix, 0, sizeof(float) * 16);

	_matrix[4 * 0 + 0] = x;
	_matrix[4 * 1 + 1] = y;
	_matrix[4 * 2 + 2] = z;
	_matrix[4 * 3 + 3] = 1.0f;


}

void    SsOpenGLMatrix::setTranslationMatrix(const float x, const float y, const float z)
{
	memset(_matrix, 0, sizeof(float) * 16);

	_matrix[0] = 1.0f;
	_matrix[5] = 1.0f;
	_matrix[10] = 1.0f;


	_matrix[12] = x;
	_matrix[13] = y;
	_matrix[14] = z;
	_matrix[15] = 1.0f;


}



void multiplyMatrix(const float *m0, const float *m1, float *matrix)
{
	for (int i = 0; i < 16; ++i) {
		int j = i & ~3, k = i & 3;

		matrix[i] = m0[j + 0] * m1[0 + k]
			+ m0[j + 1] * m1[4 + k]
			+ m0[j + 2] * m1[8 + k]
			+ m0[j + 3] * m1[12 + k];
	}
}

void 	SsOpenGLMatrix::multiply(SsOpenGLMatrix& m1, SsOpenGLMatrix& m2)
{

	multiplyMatrix(m1._matrix, m2._matrix, this->_matrix);

}


// 4x4の逆行列の計算
void 	SsOpenGLMatrix::inverseMatrix()
{


	double a[4][4];
	double inv_a[4][4]; //ここに逆行列が入る
	double buf; //一時的なデータを蓄える
	int i, j, k; //カウンタ
	int n = 4;  //配列の次数


				//オーバーフロー対策
	for (i = 0; i < 16; i++)
	{
		if (_matrix[i] > 0)
		{
			if ((0.000000000001f) > _matrix[i] && _matrix[i] != 0)
			{
				_matrix[i] = 0.00001f;
			}
		}
		else {
			if ((-0.000000000001f) < _matrix[i] && _matrix[i] != 0)
			{
				_matrix[i] = -0.00001f;
			}
		}
	}

	int c = 0;
	for (i = 0; i< 4; i++)
	{
		for (j = 0; j< 4; j++)
		{
			a[i][j] = _matrix[c];
			c++;
		}
	}


	//単位行列を作る
	for (i = 0; i<n; i++) {
		for (j = 0; j<n; j++) {
			inv_a[i][j] = (i == j) ? 1.0 : 0.0;
		}
	}
	//掃き出し法
	for (i = 0; i<n; i++) {
		if (a[i][i] == 0)return;
		buf = 1 / a[i][i];
		for (j = 0; j<n; j++) {
			a[i][j] *= buf;
			inv_a[i][j] *= buf;
		}
		for (j = 0; j<n; j++) {
			if (i != j) {
				buf = a[j][i];
				for (k = 0; k<n; k++) {
					a[j][k] -= a[i][k] * buf;
					inv_a[j][k] -= inv_a[i][k] * buf;
				}
			}
		}
	}

	c = 0;
	for (i = 0; i< 4; i++)
	{
		for (j = 0; j< 4; j++)
		{
			_matrix[c] = (float)(inv_a[i][j]);
			c++;
		}
	}


}

void  SsOpenGLMatrix::TransformVector3(SsVector3& src, SsVector3& dst)
{
	float vx, vy, vz;
	SsVector3 vec;
	float *pF1, *pF2, *pF3, *pF4;

	vx = src.x;
	vy = src.y;
	vz = src.z;

	pF1 = &this->_matrix[0];
	pF2 = &this->_matrix[4];
	pF3 = &this->_matrix[8];
	pF4 = &this->_matrix[12];

	vec.x = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + (*pF4);
	pF1 += 1; pF2 += 1; pF3 += 1; pF4 += 1;

	vec.y = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + (*pF4);
	pF1 += 1; pF2 += 1; pF3 += 1; pF4 += 1;

	vec.z = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + (*pF4);
	pF1 += 1; pF2 += 1; pF3 += 1; pF4 += 1;

	//vec.w = vx * (*pF1) + vy * (*pF2) + vz * (*pF3) + (*pF4);

	dst = vec;

}


}	// namespace spritestudio6
