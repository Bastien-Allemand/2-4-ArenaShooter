#include "Elevator.h"
#include "Maploader.h"
#include "ArenaManager.h"


void ElevatorScript::Update()
{
	gce::BoxCollider* box = m_pOwner->GetComponent<gce::BoxCollider>();

	for (auto id : box->m_wasCollidingID) 
	{
		if (id == ArenaManager::GetInstance()->GetPlayer()->GetID()) // Collision stay call doesn't work, this is a workaround
		{
			std::cout << "Collide with player";
			ItemManager* itemManager = ArenaManager::GetItemManager();
			
			if (gce::GetKeyDown(gce::Keyboard::E))
			{
				if (itemManager->HasItem(100))
				{
					ArenaManager::GetInstance()->LoadMap(1);
				}
			}
		}

	}
}

void ElevatorScript::CollisionStay(gce::GameObject& other)
{
	if(!locked){
		if (gce::GetKeyDown(gce::Keyboard::E))
		{
			m_mapLoader->loadMap(1);
		}
	}
}
