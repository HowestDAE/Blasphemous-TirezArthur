#include "pch.h"
#include "SaveManager.h"
#include "json/json.h"
#include "Player.h"
#include <fstream>
#include <iostream>

SaveManager::SaveManager()
{

}

SaveManager::~SaveManager()
{
	UnLoadSave();
}

void SaveManager::SetPlayer(Player* player)
{
	m_PlayerPtr = player;
}

void SaveManager::DeleteSave(int saveIndex)
{
	m_SaveDelete = saveIndex;
}

void SaveManager::ConfirmDelete(bool confirm)
{
	if (confirm && m_SaveDelete != -1) {
		std::remove(("Saves/save" + std::to_string(m_SaveDelete) + ".json").c_str());
		m_SavePeeks.erase(m_SaveDelete);
	}
	m_SaveDelete = -1;
}

bool SaveManager::PeekSave(int saveIndex, std::string& region, int& tears)
{
	if (m_SavePeeks.find(saveIndex) == m_SavePeeks.end()) {
		std::ifstream saveFile{ "Saves/save" + std::to_string(saveIndex) + ".json", std::fstream::in };
		if (!saveFile.good()) return false;
		if (!saveFile) return false;
		Json::Value saveData;
		saveFile >> saveData;
		m_SavePeeks[saveIndex].second = saveData.get("tears", 0).asInt();
		m_SavePeeks[saveIndex].first = saveData.get("saveRegion", 0).asString();
	}
	region = m_SavePeeks.at(saveIndex).first;
	tears = m_SavePeeks.at(saveIndex).second;
	return true;
}

int SaveManager::SaveLoaded()
{
	return m_LoadedSave;
}

void SaveManager::AddGuilt(const Point2f& pos, const std::string& area)
{
	m_GuiltLocations.emplace_back(std::pair<Point2f, std::string>{pos, area});
}

std::string SaveManager::GetSaveArea()
{
	return m_SavedArea;
}

Point2f SaveManager::GetSavePosition()
{
	return m_SaveLocation;
}

int SaveManager::GetGuiltAmount() const
{
	return (int)m_GuiltLocations.size();
}

int& SaveManager::GetTears()
{
	return m_Tears;
}

std::vector<Point2f> SaveManager::GetGuiltPositions(const std::string& area) const
{
	std::vector<Point2f> points{};
	for (const std::pair<Point2f, std::string>& location : m_GuiltLocations) {
		if (location.second == area) points.push_back(location.first);
	}
	return points;
}

void SaveManager::SetSaveLocation(const std::string& area, const Point2f& pos)
{
	m_SavedArea = area;
	m_SaveLocation = pos;
}

void SaveManager::CollectItem(const Item& item)
{
	if (!IsItemCollected(item.category, item.id)) m_CollectedItems.push_back(item);
}

void SaveManager::SetSaveRegion(const std::string& region)
{
	m_SaveRegion = region;
}

bool SaveManager::IsItemCollected(CategoryId category, ItemId id) const
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

int SaveManager::GetCollectedAmount() const
{
	return (int)m_CollectedItems.size();
}

void SaveManager::LoadSave(int index)
{
	std::ifstream saveFile{ "Saves/save" + std::to_string(index)+".json", std::fstream::in};
	m_CollectedItems.clear();
	m_GuiltLocations.clear();
	if (!saveFile) {
		m_Tears = 0;
		m_SavedArea = "outside1";
		m_SaveLocation = Point2f{ 150.0f, 720.0f };
		m_LoadedSave = index;
		m_PlayerPtr->Respawn();
		return;
	}
	Json::Value saveData;
	saveFile >> saveData;
	m_Tears = saveData.get("tears", 0).asInt();
	m_SaveLocation.x = saveData["savePos"].get("x", 0).asFloat();
	m_SaveLocation.y = saveData["savePos"].get("y", 0).asFloat();
	m_SavedArea = saveData.get("saveArea", "").asString();
	m_SaveRegion = saveData.get("saveRegion", 0).asString();
	for (int itemIndex{}; itemIndex < static_cast<int>(saveData["items"].size()); ++itemIndex) {
		const CategoryId category{ (CategoryId)saveData["items"][itemIndex].get("category", 0).asInt()};
		const ItemId id{ (ItemId)saveData["items"][itemIndex].get("id", 0).asInt() };
		const std::string name{ saveData["items"][itemIndex].get("name", "").asString() };
		const std::string description{ saveData["items"][itemIndex].get("description", "").asString() };
		m_CollectedItems.emplace_back(Item{category, id, name, description});
	}
	for (int guiltIndex{}; guiltIndex < static_cast<int>(saveData["guilt"].size()); ++guiltIndex) {
		const Point2f	pos{ saveData["guilt"][guiltIndex].get("x", 0).asFloat(),
						saveData["guilt"][guiltIndex].get("y", 0).asFloat() };
		const std::string area{ saveData["guilt"][guiltIndex].get("area", "").asString() };
		AddGuilt(pos, area);
	}
	m_LoadedSave = index;
	m_PlayerPtr->Respawn();
}

void SaveManager::UnLoadSave()
{
	if (m_LoadedSave == -1) return;
	m_SavePeeks.erase(m_LoadedSave);
	std::ofstream saveFile{ "Saves/save" + std::to_string(m_LoadedSave) + ".json", std::fstream::binary };
	Json::Value saveData;
	saveData["tears"] = m_Tears;
	saveData["savePos"]["x"] = m_SaveLocation.x;
	saveData["savePos"]["y"] = m_SaveLocation.y;
	saveData["saveArea"] = m_SavedArea;
	saveData["saveRegion"] = m_SaveRegion;
	for (int itemIndex{}; itemIndex < m_CollectedItems.size(); ++itemIndex) {
		saveData["items"][itemIndex]["name"] = m_CollectedItems.at(itemIndex).name;
		saveData["items"][itemIndex]["description"] = m_CollectedItems.at(itemIndex).description;
		saveData["items"][itemIndex]["category"] = (int)m_CollectedItems.at(itemIndex).category;
		saveData["items"][itemIndex]["id"] = (int)m_CollectedItems.at(itemIndex).id;
	}
	for (int guiltIndex{}; guiltIndex < m_GuiltLocations.size(); ++guiltIndex) {
		saveData["guilt"][guiltIndex]["pos"]["x"] = m_GuiltLocations.at(guiltIndex).first.x;
		saveData["guilt"][guiltIndex]["pos"]["y"] = m_GuiltLocations.at(guiltIndex).first.y;
		saveData["guilt"][guiltIndex]["area"] = m_GuiltLocations.at(guiltIndex).second;
	}
	saveFile << saveData;
	m_LoadedSave = -1;
}

Item::Item(CategoryId category, ItemId id, std::string name, std::string description) :
	category{category},
	id{id},
	name{name},
	description{description}
{
}
