#include "pch.h"
#include "Ray.h"
#include "ArenaManager.h"

gce::Vector4 Ray::Vector4Matrix(gce::Matrix _m, gce::Vector4 _v)
{
    float x = _m[0][0] * _v.GetX() + _m[0][1] * _v.GetY() + _m[0][2] * _v.GetZ() + _m[0][3] * _v.GetW();
    float y = _m[1][0] * _v.GetX() + _m[1][1] * _v.GetY() + _m[1][2] * _v.GetZ() + _m[1][3] * _v.GetW();
    float z = _m[2][0] * _v.GetX() + _m[2][1] * _v.GetY() + _m[2][2] * _v.GetZ() + _m[2][3] * _v.GetW();
    float w = _m[3][0] * _v.GetX() + _m[3][1] * _v.GetY() + _m[3][2] * _v.GetZ() + _m[3][3] * _v.GetW();

    return gce::Vector4(x, y, z, w);
}

//Ray::Ray(gce::Vector2i32 _screenPos, gce::Vector2i32 const& _screenSize, gce::Vector3f32 const& _cameraWorldPosition, float _length, bool _fullscreen)
//{
//    gce::Matrix viewProjectionInverse = ArenaManager::GetInstance()->GetCamera()->GetGCRCamera().GetVPMatrix().GetInverse();
//    gce::Vector2f32 ndc; // Normalized Device Coordinates (-1 to 1)
//    ndc.x = (2.0f * _screenPos.x) / _screenSize.x - 1.0f;
//    ndc.y = 1.0f - (2.0f * _screenPos.y) / _screenSize.y;
//
//    float32 nearPlane = ArenaManager::GetInstance()->GetScene()->mp_camera->GetNearPlane();
//    float32 aspectRatio = _screenSize.x / (float32) _screenSize.y;
//
//    gce::Vector4 clipPoint(ndc.x, ndc.y, 1.0f, 1.0f);
//
//    gce::Vector4 worldPoint4 = Vector4Matrix(viewProjectionInverse, clipPoint);
//
//    // Homogeneous divide
//    float w = worldPoint4.GetW();
//    if (std::abs(w) > 1e-6f)
//        worldPoint4 /= w;
//
//    gce::Vector3f32 worldPoint(
//        worldPoint4.GetX(),
//        worldPoint4.GetY(),
//        worldPoint4.GetZ()
//    );
//
//    // Direction of ray between GameObject and cursor
//    gce::Vector3f32 direction = (worldPoint - _cameraWorldPosition).Normalize();
//
//    m_origin = _cameraWorldPosition;
//    m_direction = direction;
//    m_length = _length;
//}
