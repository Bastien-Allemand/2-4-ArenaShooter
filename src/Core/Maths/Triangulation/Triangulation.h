#ifndef CORE_TRIANGULATION_H_INCLUDED
#define CORE_TRIANGULATION_H_INCLUDED

#include <Containers/Vector.hpp>
#include "define.h"

namespace gce
{
	Vector<Vector2f32> ProjectToPlane(Vector<Vector3f32> const& points);

	Vector<uint32> Triangulate(Vector<Vector2f32> const& pos);
}

#endif