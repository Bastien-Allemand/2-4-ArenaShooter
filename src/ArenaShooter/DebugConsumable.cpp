#include "DebugConsumable.h"
#include "ArenaManager.h"

DebugConsumable::DebugConsumable()
{
	m_name = L"Debug Consumable";
	m_description = L"Debug Consumable Description";
}

void DebugConsumable::OnSecondary()
{
	std::cout << "Used consumable n°" << m_amount << std::endl;

	m_amount -= 1;
	if (m_amount <= 0)
	{
		ArenaManager::GetItemManager()->RemoveItem<DebugConsumable>();
	}
}