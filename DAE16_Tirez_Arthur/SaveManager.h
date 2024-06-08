#pragma once
#include <vector>

enum class CategoryId {
	null = 0,
	rosary_beads = 1,
	relics = 2,
	quest_items = 3,
	mae_culpa_hearts = 4,
	prayers = 5,
	collectibles = 6
};

enum class ItemId {
	null = 0,
	proclamation_uvula = 1,
	navicular = 2,
};

struct Item {
	explicit Item(CategoryId category, ItemId id, std::string name, std::string description);
	CategoryId category;
	ItemId id;
	std::string name;
	std::string description;
};

class SaveManager final
{
public:
	SaveManager();

	void CollectItem(const Item& item);
	bool IsItemCollected(CategoryId category, ItemId id);
	Item GetItem(CategoryId category, int index) const;
	int GetCollectedAmount();
private:
	std::vector<Item> m_CollectedItems;
};

