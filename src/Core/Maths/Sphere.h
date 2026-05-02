#ifndef CORE_MATHS_SPHERE_H_INCLUDED
#define CORE_MATHS_SPHERE_H_INCLUDED

#include "define.h"
#include "Vector2.hpp"
#include "Vector3.h"

namespace gce
{

struct Sphere
{
	float32 ray = 0.5;
	Vector3f32 center {0,0,0};
};

struct Circle
{
	float32 radius = 0.5f;
	Vector2f32 center{ 0,0 };
};

}

#endif