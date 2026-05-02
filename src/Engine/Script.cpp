#include "pch.h"
#include "Script.h"
#include "GameManager.h"
#include "GameObject.h"

namespace gce
{
	bool Script::IsActive() const
	{
		return m_activated && m_pOwner->IsActive();
	}

	void Script::SetActive(bool active)
	{
		m_activated = active;
	}

	void Script::Init()
	{
		if (m_flags & ScriptFlag::Start) m_pListeners[0] = GameManager::GetScriptSystem().m_startEvent.AddListener(&Script::OnStart, this);
		if (m_flags & ScriptFlag::Update) m_pListeners[1] = GameManager::GetScriptSystem().m_updateEvent.AddListener(&Script::OnUpdate, this);
		if (m_flags & ScriptFlag::FixedUpdate) m_pListeners[2] = GameManager::GetScriptSystem().m_fixedUpdateEvent.AddListener(&Script::OnFixedUpdate, this);
		if (m_flags & ScriptFlag::Destroy) m_pListeners[3] = m_pOwner->m_destroyEvent.AddListener(&Script::OnDestroy, this);
		if (m_pOwner->HasComponentOfSameFamily<BoxCollider>())
		{
			if (m_flags & ScriptFlag::CollisionEnter) m_pListeners[4] = m_pOwner->pCollisionEvents[0].AddListener(&Script::OnCollisionEnter, this);
			if (m_flags & ScriptFlag::CollisionStay) m_pListeners[5] = m_pOwner->pCollisionEvents[1].AddListener(&Script::OnCollisionStay, this);
			if (m_flags & ScriptFlag::CollisionExit) m_pListeners[6] = m_pOwner->pCollisionEvents[2].AddListener(&Script::OnCollisionExit, this);
			if (m_flags & ScriptFlag::Collision2DEnter) m_pListeners[7] = m_pOwner->pCollision2DEvents[0].AddListener(&Script::OnCollision2DEnter, this);
			if (m_flags & ScriptFlag::Collision2DStay) m_pListeners[8] = m_pOwner->pCollision2DEvents[1].AddListener(&Script::OnCollision2DStay, this);
			if (m_flags & ScriptFlag::Collision2DExit) m_pListeners[9] = m_pOwner->pCollision2DEvents[2].AddListener(&Script::OnCollision2DExit, this);
			m_hadCollider = true;
		}
	}

	void Script::UnInit()
	{
		if (m_flags & ScriptFlag::Start) GameManager::GetScriptSystem().m_startEvent.RemoveListener(static_cast<Event<>::ListenerBase*>(m_pListeners[0]));
		if (m_flags & ScriptFlag::Update) GameManager::GetScriptSystem().m_updateEvent.RemoveListener(static_cast<Event<>::ListenerBase*>(m_pListeners[1]));
		if (m_flags & ScriptFlag::FixedUpdate) GameManager::GetScriptSystem().m_fixedUpdateEvent.RemoveListener(static_cast<Event<>::ListenerBase*>(m_pListeners[2]));
		if (m_flags & ScriptFlag::Destroy) m_pOwner->m_destroyEvent.RemoveListener(static_cast<Event<>::ListenerBase*>(m_pListeners[3]));
		if ( m_hadCollider )
		{
			if (m_flags & ScriptFlag::CollisionEnter) m_pOwner->pCollisionEvents[0].RemoveListener(static_cast<Event<GameObject*>::ListenerBase*>(m_pListeners[4]));
			if (m_flags & ScriptFlag::CollisionStay) m_pOwner->pCollisionEvents[1].RemoveListener(static_cast<Event<GameObject*>::ListenerBase*>(m_pListeners[5]));
			if (m_flags & ScriptFlag::CollisionExit) m_pOwner->pCollisionEvents[2].RemoveListener(static_cast<Event<GameObject*>::ListenerBase*>(m_pListeners[6]));
			if (m_flags & ScriptFlag::Collision2DEnter) m_pOwner->pCollision2DEvents[0].RemoveListener(static_cast<Event<GameObject*>::ListenerBase*>(m_pListeners[7]));
			if (m_flags & ScriptFlag::Collision2DStay) m_pOwner->pCollision2DEvents[1].RemoveListener(static_cast<Event<GameObject*>::ListenerBase*>(m_pListeners[8]));
			if (m_flags & ScriptFlag::Collision2DExit) m_pOwner->pCollision2DEvents[2].RemoveListener(static_cast<Event<GameObject*>::ListenerBase*>(m_pListeners[9]));
		}
	}


}

