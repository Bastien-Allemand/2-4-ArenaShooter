#ifndef ENGINE_RAYCAST_H_INCLUDED
#define ENGINE_RAYCAST_H_INCLUDED

#include "define.h"
#include "Maths/Vector3.h"
#include "RenderCamera.h"
#include "Maths/Vector4.h"
#include "Maths/Quaternion.h"

// Informations avbout the Ray and the Raycast
namespace gce {

	// Origin and direction of the ray
    struct Ray {
        Vector3f32 origin;
        Vector3f32 direction;

		// Constructors
        Ray() = default;
        Ray(Vector3f32 const& o, Vector3f32 const& d) : origin(o), direction(d) {}
    };

	// Information about the raycast hit
    struct RaycastHit {
        GameObject* pGameObject;
        float32 distance;
        Vector3f32 normal;
        Vector3f32 point;
    };

    class Raycast {
    public:
		// Create a ray from a screen point
        static Ray FromScreenPoint(
            Vector2i32 const& screenPos,
            Vector2i32 const& screenSize,
            Matrix const& viewProjectionInverse,
            Vector3f32 const& cameraWorldPosition)
        {
            Vector2f32 ndc;
            ndc.x = (2.0f * screenPos.x) / screenSize.x - 1.0f;
            ndc.y = 1.0f - (2.0f * screenPos.y) / screenSize.y;

            Vector4 clipPoint(ndc.x, ndc.y, 1.0f, 1.0f);

            Vector4 worldPoint4 = clipPoint.Vector4Matrix(viewProjectionInverse, clipPoint);

            // Homogeneous divide
            float w = worldPoint4.GetW();
            if (std::abs(w) > 1e-6f)
                worldPoint4 /= w;

            Vector3f32 worldPoint(
                worldPoint4.GetX(),
                worldPoint4.GetY(),
                worldPoint4.GetZ()
            );

            // Direction of ray between GameObject and cursor
            Vector3f32 direction = (worldPoint - cameraWorldPosition).Normalize();

            return Ray(cameraWorldPosition, direction);
        }


    };

}

#endif
