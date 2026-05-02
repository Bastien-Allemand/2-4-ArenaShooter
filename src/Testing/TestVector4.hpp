#ifndef TEST_VECTOR4_HPP_INCLUDED
#define TEST_VECTOR4_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Vector4 )

	Vector4 v1;
	v1[0] = 2;
	v1[1] = 1;
	v1[2] = 0;
	v1[3] = 4;

	Vector4 v1Copy;
	v1Copy.x() = v1.x();
	v1Copy.y() = v1.y();
	v1Copy.z() = v1.z();
	v1Copy.w() = v1.w();
	assert(v1 == v1Copy);

	Vector4 v1CopyCopy = v1Copy;
	assert(v1CopyCopy == v1Copy);

	Vector4 v1CopyCopyCopy(v1CopyCopy);
	assert(v1CopyCopy == v1CopyCopyCopy);

	Vector4 v2(1, -2, 3, -4);
	assert(v1 != v2);

	// (1 - 2, -2 - 1, 3 - 0, -4 - 4) = (-1, -3, 3, -8)
	Vector4 v3 = v2 - v1;

	assert(v3 == Vector4(-1, -3, 3, -8));
	assert(-v3 == Vector4(1, 3, -3, 8));
	assert(v3 + v1 == v2);

	v1 *= 3;
	assert(v1 == Vector4(6, 3, 0, 12));

	v1 += Vector4(-2, 1, 4, -8);
	assert(v1 == Vector4(4, 4, 4, 4));
	assert(v1/2 == Vector4(2, 2, 2, 2)); // probs pas fonctionnel float/float faut faire un nearly equal.

	v1 -= Vector4(1, 2, 3, 4);
	assert(v1 == Vector4(3, 2, 1, 0));
	assert(v1 * 2 == Vector4(6, 4, 2, 0));
	v1 = Vector4(4, 8, 12, 16);
	v1 /= 4;
	assert(v1 == Vector4(1, 2, 3, 4));

	Vector4 v4(4, 3, 0, 0); // Norm is 5

	assert(v4.GetNorm() == 5);
	assert(v4.GetNormSq() == 25);
	assert(v4.GetNormalized() == Vector4(4.f / 5.f, 3.f / 5.f, 0, 0));
	v4.Normalize();
	assert(v4 == Vector4(4.f / 5.f, 3.f / 5.f, 0, 0));

	v1 *= 2;
	v2 *= 2;
	// (1 + 1, 2 - 2, 3 + 3, 4 - 4) 
	Vector4 v5 = Vector4::Lerp(v2, v1, 0.5f);
	assert(v5 == Vector4(2, 0, 6, 0));

	v1 *= 2;
	v2 *= 2;
	// (3 + 1, 6 - 2, 9 + 3, 12 - 4) 
	v5 = Vector4::Lerp(v2, v1, 0.75f);
	assert(v5 == Vector4(4, 4, 12, 8));

	Vector4 v6(3, 3, 3, 4);
	Vector4 v7(2, 3, 3, 3);

	// 3 * 2 + 3 * 3 + 3 * 3 + 4 * 3 = 36 
	float32 dot = Vector4::Dot(v6, v7);
	assert(dot == 36);

END_TEST_MAIN_FUNCTION( Vector4 )
END_TEST( Vector4 )


#endif