#include "pch.h"
#include "SaveManager.h"

SaveManager::SaveManager()
{
}

void SaveManager::CollectItem(const Item& item)
{
	if (!IsItemCollected(item.category, item.id)) m_CollectedItems.push_back(item);
}

bool SaveManager::IsItemCollected(CategoryId category, ItemId id)
{
	bool collected{ false };
	for (const Item& collectedItem: m_CollectedItems) {
		if (collectedItem.category == category && collectedItem.id == id) collected = true;
	}
	return collected;
}
