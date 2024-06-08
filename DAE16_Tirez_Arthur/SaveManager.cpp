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

Item SaveManager::GetItem(CategoryId category, int index) const
{
	int indexCounter{-1};
	if (category != CategoryId::null) {
		for (const Item& item : m_CollectedItems) {
			if (item.category == category) ++indexCounter;
			if (index == indexCounter) return item;
		}
	}
	else {
		for (const Item& item : m_CollectedItems) {
			++indexCounter;
			if (index == indexCounter) return item;
		}
	}
	return Item{ CategoryId::null, ItemId::null, "", "" };
}

int SaveManager::GetCollectedAmount()
{
	return (int)m_CollectedItems.size();
}

Item::Item(CategoryId category, ItemId id, std::string name, std::string description) :
	category{category},
	id{id},
	name{name},
	description{description}
{
}
