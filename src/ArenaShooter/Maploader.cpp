#include "Maploader.h"
#include <windows.h>
#include "File.h"
#include "Core/GeoCustom.h"
#include "Crate.h"
#include "Grid.h"
#include "ArenaManager.h"
#include "Elevator.h"
#include "DoorSystem.h"
#include "ResourceManager.h"
#include "Enemy.h"
#include "MeleeEnemy.h"
#include "LaserEnemy.h"
#include "HealEnemy.h"
#include "Boss.h"
#include "Zone.h"



Maploader::Maploader(){}

void Maploader::Init()
{
	m_levels.PushBack(gce::GeometryFactory::LoadMultipleGeometries(m_1LevelPath));
	m_levels.PushBack(gce::GeometryFactory::LoadMultipleGeometries(m_2LevelPath));
}

void Maploader::loadMap(int levelcount, gce::Vector3f32 offSet)
{
	if(m_currentLevel != levelcount)
	{
		for (auto object : m_drawnObjects)
		{
			object->gameObject->Destroy();
		}
		m_drawnObjects.Clear();
	}

	int i = 0;
	//SetDataGameObjects
	for (GeoCustom& level : m_levels[levelcount])
	{
		String data;
		bool take = false;

		for (int u = 0; u < level.name.size(); u++)
		{
			if (take)
			{
				data += level.name[u];
			}
			if (level.name[u] == '$')
			{
				take = true;
				continue;
			}
		}
		//code $Type(PhysicalObject,Door,VisualObject,Crate,Floor,Elevator,enemis,Zone),
		// IFDOORORFRAME::DOORTAGANDFRAME  IFCRATE::CONTENTID/2ndITEMID

		if (data[0] == 'N')
		{
			if (m_currentLevel != levelcount) 
			{
				GridBuild(&level);
				//m_grid->Init(m_level1[i].lineVertices, m_level1[i].lineIndices);
				mp_currentGraph = m_graphs[level.name];
			}
			continue;
		}
		if (m_currentLevel != levelcount)
		{
			gce::GameObject& gameObject = gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene());

			DataGameObject* newData = new DataGameObject;
			newData->gameObject = &gameObject;
			newData->name = level.name;
			newData->geo = &level;
			m_drawnObjects.PushBack(newData);
		}

		i++;
	}
	//Set Components
	for (DataGameObject* object : m_drawnObjects)
	{
		object->gameObject->SetActive(true);
		object->gameObject->transform.SetWorldPosition(object->geo->position);
		object->gameObject->transform.SetWorldScale(object->geo->scale);
		object->gameObject->transform.SetLocalRotation({0, 0, 0});
		object->gameObject->transform.WorldTranslate(offSet);
		String data;
		bool take = false;
		for (int u = 0; u < object->name.size(); u++)
		{
			if (take)
			{
				data += object->name[u];
			}
			if (object->name[u] == '$')
			{
				take = true;
			}
		}
		object->meshRenderer = object->gameObject->AddComponent<gce::MeshRenderer>();

		switch (data[0])
		{
		//PhysicalObject
		case '0':
		{
			if (m_currentLevel == levelcount)
			{
				break;
			}
			//PhysicalObject
			WallBuild(object);	
			break;
		}
		//Door
		case '1':
		{
			object->gameObject->SetName("Door");
			object->meshRenderer->SetGeometry(object->geo->pGeometry);
			DoorSystemScript* door = object->gameObject->AddScript<DoorSystemScript>();
			if (data[1] == 'L')
			{
				door->Locked = true;
			}
			SetCollider(object);
			if (object->collider != nullptr)
			{
				object->collider->SetIgnoreTag(1);
			}
			break;
		}
		//VisualObject
		case '2':
		{
			object->meshRenderer->SetGeometry(object->geo->pGeometry);
			if (m_currentLevel == levelcount)
			{
				break;
			}
			//VisualObject
			object->gameObject->SetName("DoorFrame");
			break;
		}
		//Crate
		case '3':
		{
			int contentID = (data[1] - 48) * 100 + (data[2] - 48) * 10 + (data[3] - 48);
			if (m_currentLevel == levelcount)
			{
				object->gameObject->RemoveScript<Crate>();
				object->gameObject->AddScript<Crate>()->setItemId(contentID);
				break;
			}

			object->meshRenderer->SetGeometry(object->geo->pGeometry);
			object->gameObject->SetName("Crate");
			object->gameObject->AddScript<Crate>()->setItemId(contentID);
			SetCollider(object);
			object->gameObject->AddComponent<gce::PhysicComponent>()->SetGravityScale(2);
			break;
		}
		//Floor
		case '4':
		{
			//Floor
			if (m_currentLevel == levelcount)
			{
				break;
			}
			object->meshRenderer->SetGeometry(object->geo->pGeometry);
			object->gameObject->SetName("Floor");
			SetCollider(object);
			if (object->collider != nullptr)
			{
				object->collider->SetIgnoreTag(1);
			}
			break;
		}
		//Elevator
		case '5':
		{
			ElevatorScript* elevator = nullptr;
			if (m_currentLevel == levelcount)
			{
				elevator = object->gameObject->GetScript<ElevatorScript>();
				elevator->locked = true;
				break;
			}

			//Elevator
			object->meshRenderer->SetGeometry(object->geo->pGeometry);
			object->gameObject->SetName("Elevator");
			elevator = object->gameObject->AddScript<ElevatorScript>();
			elevator->m_mapLoader = this;
			elevator->locked = true;

			SetCollider(object);
			if (object->collider != nullptr)
			{
				object->collider->SetIgnoreTag(1);
				object->collider->isTrigger = true;
			}
			break;
		}
		//enemy
		case '6':
		{
			object->gameObject->SetName("Enemy");
			gce::Geometry* enemyGeo = nullptr;
			switch (data[1])
			{
			case '0':
				if (m_currentLevel == levelcount)
				{
					object->gameObject->GetScript<MeleeEnemy>()->Reset();
					break;
				}
				object->gameObject->AddScript<MeleeEnemy>();
				object->gameObject->AddComponent<gce::PhysicComponent>()->SetGravityScale(0);
				enemyGeo = gce::GeometryFactory::LoadGeometry("res/Models/MobMelee.obj");
				object->meshRenderer->SetGeometry(enemyGeo);
				object->geo->texturepath = "Textures/MobMelee_Albedo.png";
				break;
			case '1':
				if (m_currentLevel == levelcount)
				{
					object->gameObject->GetScript<LaserEnemy>()->Reset();
					break;
				}
				object->gameObject->AddScript<LaserEnemy>();
				object->gameObject->AddComponent<gce::PhysicComponent>()->SetGravityScale(0);
				enemyGeo = gce::GeometryFactory::LoadGeometry("res/Models/MobLaser.obj");
				object->meshRenderer->SetGeometry(enemyGeo);
				object->geo->texturepath = "Textures/MobLaser_Albedo.png";

				break;
			case '2':
				if (m_currentLevel == levelcount)
				{
					object->gameObject->GetScript<HealEnemy>()->Reset();
					break;
				}
				object->gameObject->AddScript<HealEnemy>();
				object->gameObject->AddComponent<gce::PhysicComponent>()->SetGravityScale(0);
				enemyGeo = gce::GeometryFactory::LoadGeometry("res/Models/MobHeal.obj");
				object->meshRenderer->SetGeometry(enemyGeo);
				object->geo->texturepath = "Textures/MobHeal_Albedo.png";
				break;

			case '3':
				if (m_currentLevel == levelcount)
				{
					//object->gameObject->GetScript<Boss>()->Reset();
					break;
				}
				object->gameObject->AddScript<Boss>();
				enemyGeo = gce::GeometryFactory::LoadGeometry("res/Models/MobBoss.obj");
				object->meshRenderer->SetGeometry(enemyGeo);
				object->geo->texturepath = "Textures/MobBoss_Albedo.png";
				break;

			default:

				if (m_currentLevel == levelcount)
				{
					object->gameObject->RemoveScript<MeleeEnemy>();
					object->gameObject->AddScript<MeleeEnemy>();
					break;
				}
				object->gameObject->AddScript<MeleeEnemy>();
				object->gameObject->AddComponent<gce::PhysicComponent>()->SetGravityScale(0);
				enemyGeo = gce::GeometryFactory::LoadGeometry("res/Models/MobMelee.obj");
				object->meshRenderer->SetGeometry(enemyGeo);
				object->geo->texturepath = "Textures/MobMelee_Albedo.png";
				break;
			}

			SetCollider(object);
			ArenaManager::GetInstance()->AddEnemy(object->gameObject);

			break;
		}
		//Zone
		case '7':
		{
			if (m_currentLevel == levelcount)
			{
				object->gameObject->GetScript<ZoneScript>()->Reset();
				/*object->gameObject->RemoveComponent<gce::BoxCollider>();
				SetCollider(object);
				object->collider->isTrigger = true;
				if (object->collider != nullptr)
				{
					object->collider->SetIgnoreTag(1);
				}*/
				break;
			}

			object->gameObject->SetName("Zone");
			object->meshRenderer->SetGeometry(object->geo->pGeometry);
			object->gameObject->AddScript<ZoneScript>();
			SetCollider(object);
			object->collider->isTrigger = true;
			if (object->collider != nullptr)
			{
				object->collider->SetIgnoreTag(1);
			}

			break;
		}
		default:
			break;
		}
		SetTexture(object);


	}
	m_currentLevel = levelcount;
}

void Maploader::GridBuild(GeoCustom* level)
{
	m_graphs[level->name] = new Graph<Tile>;
	

	for (int i = 0; i < level->lineVertices.size(); i += 3)
	{
		m_graphs[level->name]->AddNode(new Node<Tile>(
			new Tile({ level->lineVertices[i + 0] + level->position.x,
						level->lineVertices[i + 1] + level->position.y,
						level->lineVertices[i + 2] + level->position.z })));
	}

	for (int i = 1; i < level->lineIndices.size(); i += 2)
	{
		m_graphs[level->name]->m_data[level->lineIndices[i - 1]]->AddNeighbor(Link(m_graphs[level->name]->m_data[level->lineIndices[i]]));
		m_graphs[level->name]->m_data[level->lineIndices[i]]->AddNeighbor(Link(m_graphs[level->name]->m_data[level->lineIndices[i - 1]]));
	}

}

void Maploader::WallBuild(DataGameObject* object)
{
	object->meshRenderer->SetGeometry(object->geo->pGeometry);
	object->gameObject->SetName("Wall");
	SetCollider(object);
	if (object->collider != nullptr)
	{
		object->collider->SetIgnoreTag(1);
	}
}

void Maploader::SetTexture(DataGameObject* object)
{
	String type;
	bool save = false;
	if (object->geo->texturepath.size() >= 1)
	{
		m_textures.PushBack(ResourceManager::GetTexture("res/Map/" + object->geo->texturepath));

		for (int y = 0; y < object->geo->texturepath.size(); y++)
		{
			if (save)
			{
				type += object->geo->texturepath[y];
			}
			if (object->geo->texturepath[y] == '_')
			{
				save = true;
			}
		}
		if (type[0] == 'A')
		{
			if (type[1] == 'l')
			{
				object->meshRenderer->SetAlbedoTexture(m_textures.Back());
			}
			if (type[1] == 'm')
			{
				object->meshRenderer->SetAmbientTexture(m_textures.Back());
			}
		}
		if (type[0] == 'M')
		{
			object->meshRenderer->SetMetalnessTexture(m_textures.Back());
		}
		if (type[0] == 'R')
		{
			object->meshRenderer->SetRoughnessTexture(m_textures.Back());
		}
		if (type[0] == 'N')
		{
			object->meshRenderer->SetNormalTexture(m_textures.Back());
		}
	}
	else
	{
		object->meshRenderer->SetActive(false);
	}
}

void Maploader::SetCollider(DataGameObject* object)
{
	object->collider = object->gameObject->AddComponent<gce::BoxCollider>();
}