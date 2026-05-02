#include "pch.h"
#include "BoxCollider2D.h"
#include "GameObject.h"
#include "GameTransform.h"
#include "Maths/Physics.h"
#include "MeshRenderer.h"
#include "Geometry.h"

namespace gce {

#define BOX_DEFAULT_VALUE .5f

	void BoxCollider2D::Init()
	{
		m_pOwner->pCollision2DEvents = &collision2DEnterEvent;
		m_pOwner->transform.dirtyEvent.AddListener(&BoxCollider2D::UpdateColliderComponent, this);
		
		if (!GetOwner().HasComponent<gce::MeshRenderer>())
		{
			m_worldBox.min = { -BOX_DEFAULT_VALUE, -BOX_DEFAULT_VALUE };
			m_worldBox.max = { BOX_DEFAULT_VALUE, BOX_DEFAULT_VALUE };
			localOBBPoints = Physics::GetListFromMinMax(m_worldBox.min, m_worldBox.max);
			
			Physics::UpdateBoundingBox(m_worldBox, GetOwner().transform.m_matrix, localScale);
			return;
		}
		
		Geometry const* geo = GetOwner().GetComponent<gce::MeshRenderer>()->GetGeometry();
		GameTransform& transform = GetOwner().transform;
		m_worldBox.min = Vector2f32(geo->min.x, geo->min.y) * localScale * Vector2f32(transform.GetWorldScale().x, transform.GetWorldScale().y);
		m_worldBox.max = Vector2f32(geo->max.x, geo->max.y) * localScale * Vector2f32(transform.GetWorldScale().x, transform.GetWorldScale().y);
		localOBBPoints = Physics::GetListFromMinMax(m_worldBox.min, m_worldBox.max);
		
		Physics::UpdateBoundingBox(m_worldBox, GetOwner().transform.m_matrix, localScale);
	}

	void BoxCollider2D::UpdateColliderComponent(uint8 const dirty)
	{
		GameTransform& transform = GetOwner().transform;

		Geometry const* geo = GetOwner().GetComponent<gce::MeshRenderer>()->GetGeometry();
		m_worldBox.min = Vector2f32(geo->min.x, geo->min.y) * localScale * Vector2f32(transform.GetWorldScale().x, transform.GetWorldScale().y);
		m_worldBox.max = Vector2f32(geo->max.x, geo->max.y) * localScale * Vector2f32(transform.GetWorldScale().x, transform.GetWorldScale().y);
		
		m_worldBox.center = Vector2f32(transform.GetWorldPosition().x, transform.GetWorldPosition().y) + m_localOffSet * Vector2f32(transform.GetWorldScale().x, transform.GetWorldScale().y);
		m_worldBox.aabb.center = m_worldBox.center;
		
		//Apply RotMatrix To center Point
		if (m_localOffSet != Vector2f32{ 0,0 })
		{
			Physics::ScaleNRotateVect2(m_worldBox.center, transform.m_matrix);
		}
		
		m_worldBox.axisX = Physics::ScaleNRotateVect2(Vector2f32{ 1,0 }, transform.m_matrix).Normalize();
		m_worldBox.axisY = Physics::ScaleNRotateVect2(Vector2f32{ 0,1 }, transform.m_matrix).Normalize();
		
		Physics::UpdateBoundingBox(m_worldBox, transform.m_matrix, localScale);

		//Apply RotMatrix To center Point
		if (m_localOffSet != Vector2f32{ 0,0 })
		{
			Physics::ScaleNRotateVect2(m_worldBox.center, transform.m_matrix);
		}
	}

#undef BOX_DEFAULT_VALUE
}