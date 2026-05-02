#include "DoorSystem.h"
#include "ArenaManager.h"

void DoorSystemScript::Start()
{
	pPlayer = ArenaManager::GetInstance()->GetPlayer();
	closedPosition = m_pOwner->transform.GetWorldPosition();
	openPosition = closedPosition;
	openPosition.y -= 10.f;
	offset = openPosition - closedPosition;

}

void DoorSystemScript::FixedUpdate()
{
	if (!Locked)
	{
		gce::Vector3f32 playerPos = pPlayer->transform.GetWorldPosition();
		float range = (m_pOwner->transform.GetWorldPosition() - playerPos).Norm();
		if (range < 16.f )
		{
			if (!open)
			{
				opening = true;
			}
		}
		else
		{
			if (!closed)
			{
				closing = true;
			}
		}
		if (opening && !open)
		{
			m_pOwner->transform.WorldTranslate(offset * gce::GameManager::FixedDeltaTime() / 2 );
			if ((m_pOwner->transform.GetWorldPosition() - openPosition).Norm() < 0.1f)
			{
				m_pOwner->transform.SetWorldPosition(openPosition);
				opening = false;
				open = true;
				closed = false;
			}
		}
		if (closing)
		{
			m_pOwner->transform.WorldTranslate(-offset * gce::GameManager::FixedDeltaTime() / 2);
			if ((m_pOwner->transform.GetWorldPosition() - closedPosition).Norm() < 0.1f)
			{
				m_pOwner->transform.SetWorldPosition(closedPosition);
				closing = false;
				closed = true;
				open = false;
			}
		}
	}

	else
	{
		Locked = ArenaManager::GetItemManager()->HasItem(100) ? false : true;
	}
}