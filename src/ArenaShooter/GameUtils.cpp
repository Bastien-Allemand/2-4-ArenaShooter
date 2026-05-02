#include "GameUtils.h"
#include "ArenaManager.h"

gce::GameObject& CreateImage(gce::BitMapBrush* btm, gce::Vector2f32 pos, gce::Vector2f32 scale, gce::Vector2f32 origin, float rotation)
{
	gce::GameObject& itemUIObject = gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene());
	gce::UiImage* mp_itemSideUI = itemUIObject.AddComponent<gce::UiImage>();
	gce::Vector2f32 originOffset = origin * btm->GetSize() * scale;
	mp_itemSideUI->InitializeImage(pos - originOffset, btm->GetSize() * scale, 1);
	mp_itemSideUI->btmBrush = btm;
	mp_itemSideUI->btmBrush->SetTransformMatrix({ pos.x - originOffset.x, pos.y - originOffset.y, 0 }, { scale.x, scale.y, 1 }, 0);

	return itemUIObject;
}

Node<Tile>* GetClosestNode(gce::Vector3f32 pos, Graph<Tile>* graph)
{
	Node<Tile>* closest = nullptr;
	float minimumDistance = INFINITE;
	for (auto node : graph->m_data)
	{
		float delta = (node->GetData()->GetPosition() - pos).Norm();
		if (minimumDistance > delta)
		{
			minimumDistance = delta;
			closest = node;
		}
	}

	return closest;
}