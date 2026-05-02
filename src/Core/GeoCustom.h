#pragma once
#include "Render/GeometryFactory.h"
#include "Render/Geometry.h"
#include "Maths/Quaternion.h"
#include <windows.h>
#include "define.h"
struct GeoCustom
{
public:
	String name;
	String texturepath;
	gce::Geometry* pGeometry = nullptr;
	gce::Vector3f32 position;
	gce::Vector3f32 scale;
	gce::Quaternion rotation;

	std::vector<float> lineVertices;
	std::vector<uint32> lineIndices;
};
