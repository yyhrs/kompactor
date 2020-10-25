#ifndef _3DTYPES_H
#define _3DTYPES_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#ifndef NDEBUG
#define D3D_DEBUG_INFO
#endif
#include <windows.h>

/*
 * Basic 3D types for 3D math
 * - Vector3
 * - Vector4
 * - Matrix
 *
 * Some properties:
 * - Matrices and vectors behave like in Direct3D
 * - Matrices have translation in bottom row
 * - Vectors are column vectors
 * - Matrix-vector multiplication is post-multiplication
 */
namespace Alamo
{

	struct Matrix;
	struct Vector3;
	struct Vector4;

	static const float PI = 3.14 /* = D3DX_PI*/;

	struct Point
	{
		int x;
		int y;

		Point(int _x, int _y): x(_x), y(_y) {
		}
		Point() {
		}
	};

	struct Vector3
	{
		float x;
		float y;
		float z;
	};

	struct Vector4
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct Matrix
	{
		float _11;
		float _12;
		float _13;
		float _14;
		float _21;
		float _22;
		float _23;
		float _24;

		static const Matrix Identity;
	};

	struct Vector2
	{
		float x;
		float y;
	};

	struct Color // :public D3DXCOLOR
	{
		float r;
		float g;
		float b;
		float a;
	};

}

#endif
