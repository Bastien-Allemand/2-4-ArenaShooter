#pragma once
#include "Maths/Vector3.h"

class Entity;

struct Ray
{
	struct HitInfo
	{
		Entity* entity;
		float32 distance;
		gce::Vector3f32 normal;
		gce::Vector3f32 point;
	};

	gce::Vector3f32 m_origin;
	gce::Vector3f32 m_direction;
	float m_length;

	gce::Vector4 Vector4Matrix(gce::Matrix _m, gce::Vector4 _v);

	Ray(gce::Vector2i32 screenPos, gce::Vector2i32 const& screenSize, gce::Vector3f32 const& cameraWorldPosition, float _length, bool _fullscreen = false);
};

