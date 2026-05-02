#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "GameTransform.h"
#include "Maths/Physics.h"
#include "MeshRenderer.h"
#include "Geometry.h"

namespace gce {

#define BOX_DEFAULT_VALUE .5f

void BoxCollider::Init()
{
	m_pOwner->pCollisionEvents = &collisionEnterEvent;
	m_pOwner->transform.dirtyEvent.AddListener(&BoxCollider::UpdateColliderComponent, this);

	if (!GetOwner().HasComponent<gce::MeshRenderer>())
	{
		m_worldBox.min = { -BOX_DEFAULT_VALUE, -BOX_DEFAULT_VALUE, -BOX_DEFAULT_VALUE };
		m_worldBox.max = { BOX_DEFAULT_VALUE, BOX_DEFAULT_VALUE, BOX_DEFAULT_VALUE };
		localOBBPoints = Physics::GetListFromMinMax(m_worldBox.min, m_worldBox.max);

		Physics::UpdateBoundingBox(m_worldBox, GetOwner().transform.m_matrix, localScale );
		return;
	}

	Geometry const* geo = GetOwner().GetComponent<gce::MeshRenderer>()->GetGeometry();
	GameTransform& transform = GetOwner().transform;
	m_worldBox.min = geo->min * localScale * transform.GetWorldScale();
	m_worldBox.max = geo->max * localScale * transform.GetWorldScale();
	localOBBPoints = Physics::GetListFromMinMax(m_worldBox.min, m_worldBox.max);

	Physics::UpdateBoundingBox(m_worldBox, GetOwner().transform.m_matrix, localScale);
}

void BoxCollider::UpdateColliderComponent( uint8 const dirty )
{
	GameTransform& transform = GetOwner().transform;

	if ( GetOwner().HasComponent<gce::MeshRenderer>() )
	{
		Geometry const* pGeometry = GetOwner().GetComponent<gce::MeshRenderer>()->GetGeometry();
		m_worldBox.min = localScale * transform.GetWorldScale() * pGeometry->min;
		m_worldBox.max = localScale * transform.GetWorldScale() * pGeometry->max;
	}
	else
	{
		m_worldBox.min = localScale * transform.GetWorldScale() * -BOX_DEFAULT_VALUE;
		m_worldBox.max = localScale * transform.GetWorldScale() * BOX_DEFAULT_VALUE;
	}

	m_worldBox.center = transform.GetWorldPosition() + m_localOffSet * transform.GetWorldScale();
	m_worldBox.aabb.center = m_worldBox.center;

	//Apply RotMatrix To center Point
	if (m_localOffSet != Vector3f32{ 0,0,0 })
	{
		Physics::ScaleNRotateVect3(m_worldBox.center, transform.m_matrix);
	}

	m_worldBox.axisX = Physics::ScaleNRotateVect3(Vector3f32{1,0,0}, transform.m_matrix).Normalize();
	m_worldBox.axisY = Physics::ScaleNRotateVect3(Vector3f32{0,1,0}, transform.m_matrix).Normalize();
	m_worldBox.axisZ = Physics::ScaleNRotateVect3(Vector3f32{0,0,1}, transform.m_matrix).Normalize();

	Physics::UpdateBoundingBox(m_worldBox, transform.m_matrix, localScale);

	//Apply RotMatrix To center Point
	if (m_localOffSet != Vector3f32{ 0,0,0 })
	{
		Physics::ScaleNRotateVect3(m_worldBox.center, transform.m_matrix);
	}
}

/////////////////////////////////////
/// @brief Check if the ray intersects with BoxColliders
/// @param ray Ray to intersect.
/// @param hitInfo Structure to store the hit information.
/// @param maxDistance Maximum distance to check for intersection.
/// @return true if Intersect
/////////////////////////////////////
bool BoxCollider::RaycastCollider(Ray const& ray, RaycastHit& hitInfo, float32 maxDistance)
{
	// Info box  
	Vector3f32 const& boxCenter = m_worldBox.center;
	Vector3f32 const& axisX = m_worldBox.axisX;
	Vector3f32 const& axisY = m_worldBox.axisY;
	Vector3f32 const& axisZ = m_worldBox.axisZ;
	Vector3f32 halfSize = (m_worldBox.max - m_worldBox.min) * 0.5f;

	// Transform ray in local space of the box  
	Vector3f32 p = ray.origin - boxCenter;
	Vector3f32 d = ray.direction;

	// Project ray on box axes  
	float32 px = p.DotProduct(axisX);
	float32 py = p.DotProduct(axisY);
	float32 pz = p.DotProduct(axisZ);
	float32 dx = d.DotProduct(axisX);
	float32 dy = d.DotProduct(axisY);
	float32 dz = d.DotProduct(axisZ);

	float32 tMin = -FLT_MAX;
	float32 tMax = FLT_MAX;

	// Enter and exit of the box on each axis  
	auto testAxis = [&](float p, float d, float e) {
		if (fabs(d) < 1e-6f) {
			// Ray parallel  
			if (p < -e || p > e)
				return false;
		}
		else {
			float32 t1 = (-e - p) / d;
			float32 t2 = (e - p) / d;
			if (t1 > t2) std::swap(t1, t2);
			tMin = Max(tMin, t1);
			tMax = Min(tMax, t2);
			if (tMin > tMax)
				return false;
		}
		return true;
		};

	if (!testAxis(px, dx, halfSize.x)) return false;
	if (!testAxis(py, dy, halfSize.y)) return false;
	if (!testAxis(pz, dz, halfSize.z)) return false;

	if (tMin < 0.f || tMin > maxDistance) return false;

	// Fill Hitbox Info  
	hitInfo.distance = tMin;
	hitInfo.point = ray.origin + ray.direction * tMin;
	hitInfo.pGameObject = m_pOwner;

	// Calculate normal  
	Vector3f32 localHit = p + d * tMin;
	Vector3f32 normal(0, 0, 0);
	float32 bias = 1e-4f;
	if (fabs(localHit.x - halfSize.x) < bias) normal = axisX;
	else if (fabs(localHit.x + halfSize.x) < bias) normal = -axisX;
	else if (fabs(localHit.y - halfSize.y) < bias) normal = axisY;
	else if (fabs(localHit.y + halfSize.y) < bias) normal = -axisY;
	else if (fabs(localHit.z - halfSize.z) < bias) normal = axisZ;
	else if (fabs(localHit.z + halfSize.z) < bias) normal = -axisZ;
	hitInfo.normal = normal;

	return true;
}

#undef BOX_DEFAULT_VALUE
}