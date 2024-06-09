#pragma once
#include <vector>
#include <map>

class Player;

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
	explicit SaveManager();
	explicit SaveManager(const SaveManager& other) = delete;
	explicit SaveManager(SaveManager&& other) = delete;
	~SaveManager();

	SaveManager& operator=(const SaveManager& other) = delete;
	SaveManager& operator=(SaveManager&& other) = delete;

	void SetPlayer(Player* player);
	void SetSaveLocation(const std::string& area, const Point2f& pos);
	void SetSaveRegion(const std::string& region);
	void AddGuilt(const Point2f& pos, const std::string& area);
	void RemoveGuilt(const Point2f& pos, const std::string& area);
	void CollectItem(const Item& item);

	void LoadSave(int index);
	void UnLoadSave();
	void ConfirmDelete(bool confirm);
	void DeleteSave(int saveIndex);
	bool PeekSave(int saveIndex, std::string& region, int& tears);

	int SaveLoaded();
	int GetGuiltAmount() const;
	int GetCollectedAmount() const;
	int& GetTears();
	bool IsItemCollected(CategoryId category, ItemId id) const;
	Point2f GetSavePosition();
	Item GetItem(CategoryId category, int index) const;
	std::vector<Point2f> GetGuiltPositions(const std::string& area) const;
	std::string GetSaveArea();
private:
	Player* m_PlayerPtr{};
	std::map<int, std::pair<std::string, int>> m_SavePeeks;
	std::vector<Item> m_CollectedItems;
	std::vector<std::pair<Point2f, std::string>> m_GuiltLocations;
	std::string m_SavedArea;
	std::string m_SaveRegion;
	Point2f m_SaveLocation;
	int m_Tears{};
	int m_LoadedSave{ -1 };
	int m_SaveDelete{ -1 };
};

