#ifndef CORE_MATHS_BOUNDING_BOX_H_INCLUDED
#define CORE_MATHS_BOUNDING_BOX_H_INCLUDED

#include "define.h"
#include "Vector2.hpp"
#include "Vector3.h"

namespace gce
{

struct BoundingBox
{
	Vector3f32 center{ 0,0,0 };
	Vector3f32 min{ -.5f,-.5f,-.5f }; 
	Vector3f32 max{ .5f,.5f,.5f };

};

struct Box
{
	Vector3f32 center{ 0,0,0 };
	Vector3f32 min{ -.5f,-.5f,-.5f }; 
	Vector3f32 max{ .5f,.5f,.5f };

	Vector3f32 axisX{ 1,0,0 };
	Vector3f32 axisY{ 0,1,0 };
	Vector3f32 axisZ{ 0,0,1 };

	BoundingBox aabb;
};

struct BoundingBox2D
{
	Vector2f32 center{ 0,0 };
	Vector2f32 min{ -.5f,-.5f };
	Vector2f32 max{ .5f,.5f };
};

struct Square
{
	Vector2f32 center{ 0,0 };
	Vector2f32 min{ -0.5f,-0.5f };
	Vector2f32 max{ 0.5f,0.5f };

	Vector2f32 axisX{ 1,0 };
	Vector2f32 axisY{ 0,1 };

	BoundingBox2D aabb;
};

}

#endif