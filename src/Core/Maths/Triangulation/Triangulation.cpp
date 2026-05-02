#include "Triangulation.h"
#include "../Vector3.h"
#include "../Vector2.hpp"
#include "../Matrix.h"
#include "delaunay.hpp"

namespace gce
{
    Vector<Vector2f32> ProjectToPlane(Vector<Vector3f32> const& points)
    {

        Vector3f32 centroid = { 0, 0, 0 };
        for (Vector3f32 p : points)
            centroid = centroid + p;
        centroid = centroid / float32(points.Size());

        Vector3f32 xAxis = (points[1] - points[0]).Normalize();
        Vector3f32 temp = (points[2] - points[0]).Normalize();

        Vector3f32 zAxis = xAxis.CrossProduct(temp).Normalize();
        Vector3f32 yAxis = zAxis.CrossProduct(xAxis).Normalize();

        Vector<Vector2f32> projected;
        for (Vector3f32 p : points)
        {
            Vector3f32 local = p - centroid;
            float32 x = local.DotProduct(xAxis);
            float32 y = local.DotProduct(yAxis);
            projected.PushBack({ x, y });
        }

        return projected;
    }

    Vector<uint32> Triangulate(Vector<Vector2f32> const& pos)
    {
        Vector<uint32> indices;
        Tessellator meshIndices(pos);

        for (Triangle tri : meshIndices.GetTriangles())
        {
            indices.PushBack(tri.a_);
            indices.PushBack(tri.b_);
            indices.PushBack(tri.c_);
        }

        return indices;
    }

}