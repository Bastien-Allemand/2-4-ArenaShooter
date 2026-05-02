#include "pch.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "GameTransform.h"
#include "MeshRenderer.h"
#include "Geometry.h"
#include "Maths/Matrix.h"
#include "Maths/Vector3.h"

namespace gce {

#define SPHERE_DEFAULT_VALUE .5f

void SphereCollider::Init()
{
	m_pOwner->pCollisionEvents = &collisionEnterEvent;
	m_pOwner->transform.dirtyEvent.AddListener( &SphereCollider::UpdateColliderComponent, this );
	
	if (!GetOwner().HasComponent<gce::MeshRenderer>())
	{
		localRay = SPHERE_DEFAULT_VALUE;
		return;
	}
	Geometry const* geo = GetOwner().GetComponent<gce::MeshRenderer>()->GetGeometry();

	localRay = (geo->max.x - geo->min.x) * .5f;
	if (float32 value = (geo->max.y - geo->min.y) * .5f; value > localRay) localRay = value;
	if (float32 value = (geo->max.z - geo->min.z) * .5f; value > localRay) localRay = value;
}

void SphereCollider::UpdateColliderComponent( uint8 const dirty )
{
	GameTransform& transform = m_pOwner->transform;

	m_worldSphere.center = transform.GetWorldPosition() + m_localOffSet * transform.GetWorldScale();

	//Apply RotMatrix To center Point
	if (m_localOffSet != Vector3f32{0,0,0})
	{
		Matrix scaleRot = transform.m_matrix;
		scaleRot.SetValue(3, 0, 0);
		scaleRot.SetValue(3, 1, 0);
		scaleRot.SetValue(3, 2, 0);

		Matrix localPos = Matrix::Translation(m_worldSphere.center);

		localPos = localPos * scaleRot;

		m_worldSphere.center.x = localPos.GetValue(3, 0);
		m_worldSphere.center.y = localPos.GetValue(3, 1);
		m_worldSphere.center.z = localPos.GetValue(3, 2);
	}

	const Vector3f32& scale = transform.GetWorldScale();

	float sphereRayScale = scale.x;
	if (scale.y > sphereRayScale) sphereRayScale = scale.y;
	if (scale.z > sphereRayScale) sphereRayScale = scale.z;

	m_worldSphere.ray = localRay * localScale * sphereRayScale;

	m_worldSphere.center = transform.GetWorldPosition() + m_localOffSet * transform.GetWorldScale(); // QUAND EVENT

	//Apply RotMatrix To center Point
	if (m_localOffSet != Vector3f32{ 0,0,0 })
	{
		Matrix scaleRot = transform.m_matrix;
		scaleRot.SetValue(3, 0, 0);
		scaleRot.SetValue(3, 1, 0);
		scaleRot.SetValue(3, 2, 0);

		Matrix localPos = Matrix::Translation(m_worldSphere.center);

		localPos = localPos * scaleRot;

		m_worldSphere.center.x = localPos.GetValue(3, 0);
		m_worldSphere.center.y = localPos.GetValue(3, 1);
		m_worldSphere.center.z = localPos.GetValue(3, 2);
	}
}

#undef SPHERE_DEFAULT_VALUE

}