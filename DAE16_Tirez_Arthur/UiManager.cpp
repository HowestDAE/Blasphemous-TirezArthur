#include "pch.h"
#include "UiManager.h"
#include "InputManager.h"
#include "SaveManager.h"
#include "UiElement.h"
#include "UiStaticGraphic.h"
#include "UiAnimatedGraphic.h"
#include "UiList.h"
#include "UiBar.h"
#include "UiDynamicText.h"
#include "UiTextureBar.h"
#include "UiCollection.h"
#include "UiVolumeSlider.h"
#include "UiStaticText.h"
#include "UiInventorySlot.h"
#include "UiButton.h"
#include "UIGrid.h"
#include "Player.h"
#include "json/json.h"
#include "UiRectf.h"
#include <iostream>
#include <fstream>

UiManager::UiManager( TextureManager* textureManager, Player* player, SoundManager* soundManager, InputManager* inputManager, SaveManager* saveManager):
	m_TextureManager{ textureManager },
	m_PlayerPtr{ player },
	m_SoundManagerPtr{soundManager},
	m_InputManagerPtr{inputManager},
	m_SaveManagerPtr{saveManager}
{
	LoadScreen("game");
}

UiManager::~UiManager()
{
	for ( const std::pair<std::string, Screen*>& screen : m_ScreenMap)
	{
		for (UiElement* UiElement : screen.second->elements) {
			if (UiElement != nullptr) delete UiElement;
		}
		delete screen.second;
	}
}

void UiManager::Update(float elapsedSec)
{
	if (m_ScreenMap.find(m_CurrentScreen) != m_ScreenMap.end() && m_ScreenMap.at(m_CurrentScreen) != nullptr) {
		for (UiElement* UiElement : m_ScreenMap.at(m_CurrentScreen)->elements) {
			if (UiElement != nullptr) UiElement->Update(elapsedSec);
		}
	}
	if (m_CurrentScreen == "game") {
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::escape)) LoadScreen("options");
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::inventory)) LoadScreen("inventory");
	}
	else if (m_CurrentScreen == "options") {
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::escape)) LoadScreen("game");
	}
	else if (m_CurrentScreen == "inventory") {
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::escape)) LoadScreen("game");
	}
	else if (m_CurrentScreen == "controls") {
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::escape)) LoadScreen("options");
	}
	else if (m_CurrentScreen == "sound") {
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::escape)) LoadScreen("options");
	}
}

void UiManager::Draw() const
{
	if (m_ScreenMap.find(m_CurrentScreen) != m_ScreenMap.end() && m_ScreenMap.at(m_CurrentScreen) != nullptr) {
		for (const UiElement* UiElement : m_ScreenMap.at(m_CurrentScreen)->elements) {
			if (UiElement != nullptr) UiElement->Draw();
		}
	}
}

bool UiManager::GamePaused() const
{
	if (m_ScreenMap.find(m_CurrentScreen) != m_ScreenMap.end() && m_ScreenMap.at(m_CurrentScreen) != nullptr) return m_ScreenMap.at(m_CurrentScreen)->gamePaused;
	else return true;
}

void UiManager::LoadScreen(const std::string& path)
{
	if (m_ScreenMap.find(path) == m_ScreenMap.end()) {
		std::ifstream screenFile("UiScreens/" + path + ".json", std::ifstream::binary);
		if (!screenFile)
		{
			std::cout << "Screen \"" + path + "\" does not exist.";
			m_ScreenMap.at(path) = nullptr;
			return;
		}

		Json::Value screenData;
		screenFile >> screenData;

		Screen* screen{ new Screen{} };
		screen->gamePaused = screenData.get("gamePaused", false).asBool();
		for (int elementIndex{}; elementIndex < static_cast<int>(screenData["elements"].size()); ++elementIndex) {
			screen->elements.push_back(GetElement(screenData["elements"][elementIndex]));
		}
		m_ScreenMap[path] = screen;
	}

	if (m_ScreenMap.at(path) != nullptr) m_CurrentScreen = path;
}

void UiManager::UiButtonFunction(int functionId, const std::string& data)
{
	switch (functionId)
	{
	case 0:
		LoadScreen(data);
		break;
	case 1:
		m_InputManagerPtr->ChangeKeybindEvent((InputManager::Keybind)std::stoi(data));
		break;
	default:
		break;
	}
}

UiElement* UiManager::GetElement(const Json::Value& data)
{
	const std::string type{ data.get("type", "").asString() };
	if (type == "static_graphic") return GetStaticGraphic(data);
	else if (type == "list") return GetList(data);
	else if (type == "collection") return GetCollection(data);
	else if (type == "static_text") return GetStaticText(data);
	else if (type == "bar") return GetBar(data);
	else if (type == "dynamic_text") return GetDynamicText(data);
	else if (type == "texture_bar") return GetTextureBar(data);
	else if (type == "rectf") return GetRectf(data);
	else if (type == "grid") return GetGrid(data);
	else if (type == "button") return GetButton(data);
	else if (type == "volume_slider") return GetVolumeSlider(data);
	else if (type == "inventory_slot") return GetInventorySlot(data);
	else return nullptr;
}

UiElement* UiManager::GetStaticGraphic(const Json::Value& data)
{
	const Point2f pos{ data.get("x", 0.0f).asFloat(), data.get("y", 0.0f).asFloat() };
	const std::string texture{ data.get("texture", "").asString() };
	const bool flipped{ data.get("flipped", false).asBool() };
	return new UiStaticGraphic{ pos, texture, m_TextureManager, flipped };
}

UiElement* UiManager::GetList(const Json::Value& data)
{
	const InputManager::Keybind cycleLeft{ (InputManager::Keybind)data.get("cycleLeft", 0).asInt() };
	const InputManager::Keybind cycleRight{ (InputManager::Keybind)data.get("cycleRight", 0).asInt() };
	const bool selectable{ data.get("selectable", false).asBool() };
	UiList* list{ new UiList{cycleLeft,cycleRight,m_SoundManagerPtr,m_InputManagerPtr,selectable} };
	for (int elementIndex{}; elementIndex < static_cast<int>(data["elements"].size()); ++elementIndex) {
		list->AddElement(GetElement(data["elements"][elementIndex]));
	}
	return list;
}

UiElement* UiManager::GetCollection(const Json::Value& data)
{
	const bool alwaysDisplay{ data.get("alwaysDisplay", true).asBool() };
	UiCollection* collection{ new UiCollection{ alwaysDisplay} };
	for (int elementIndex{}; elementIndex < static_cast<int>(data["elements"].size()); ++elementIndex) {
		collection->AddElement(GetElement(data["elements"][elementIndex]));
	}
	return collection;
}

UiElement* UiManager::GetStaticText(const Json::Value& data)
{
	const Point2f pos{ data.get("x", 0.0f).asFloat(), data.get("y", 0.0f).asFloat() };
	const std::string textureId{ data.get("textureId", "").asString() };
	const std::string text{	data.get("text", "").asString() };
	const Color4f textColor{	data["color"].get("R", 1.0f).asFloat(),
								data["color"].get("G", 1.0f).asFloat(),
								data["color"].get("B", 1.0f).asFloat(),
								data["color"].get("A", 1.0f).asFloat() };
	const Color4f selectedColor{ data["selectedColor"].get("R", 1.0f).asFloat(),
								data["selectedColor"].get("G", 1.0f).asFloat(),
								data["selectedColor"].get("B", 1.0f).asFloat(),
								data["selectedColor"].get("A", 1.0f).asFloat() };
	const UiStaticText::Alignment alignment{ (UiStaticText::Alignment)data.get("alignment", 7).asInt() };
	return new UiStaticText{ pos, m_TextureManager, textColor, selectedColor, textureId, text, alignment };
}

UiElement* UiManager::GetBar(const Json::Value& data)
{
	const Point2f pos{ data.get("x", 0.0f).asFloat(), data.get("y", 0.0f).asFloat() };
	const Color4f color{data["color"].get("R", 1.0f).asFloat(),
						data["color"].get("G", 1.0f).asFloat(),
						data["color"].get("B", 1.0f).asFloat(),
						data["color"].get("A", 1.0f).asFloat() };
	const float width{ data.get("w", 0.0f).asFloat()};
	const float height{ data.get("h", 0.0f).asFloat() };
	const std::string dataSource{ data.get("data", "").asString() };
	if (dataSource == "health") return new UiBar{ pos,width,height,color,m_PlayerPtr->GetMaxHealth(),m_PlayerPtr->GetHealth() };
	else return nullptr;
}

UiElement* UiManager::GetDynamicText(const Json::Value& data)
{
	const Point2f pos{ data.get("x", 0.0f).asFloat(), data.get("y", 0.0f).asFloat() };
	const std::string textureId{ data.get("textureId", "").asString() };
	const UiDynamicText::Alignment alignment{ (UiDynamicText::Alignment)data.get("alignment", 7).asInt() };
	const Color4f textColor{ data["color"].get("R", 1.0f).asFloat(),
								data["color"].get("G", 1.0f).asFloat(),
								data["color"].get("B", 1.0f).asFloat(),
								data["color"].get("A", 1.0f).asFloat() };
	const std::string dataSource{ data["data"].get("source", "").asString()};
	const UiDynamicText::DataType dataType{ (UiDynamicText::DataType)data["data"].get("type", "").asInt() };
	if (dataSource == "tears") return new UiDynamicText{ pos,m_TextureManager,m_PlayerPtr->GetTears(),dataType,textColor,textureId,alignment };
	else if (dataSource == "keybind") {
		InputManager::Keybind keybind{ (InputManager::Keybind)data["data"].get("key", 0).asInt() };
		const int& intRef{ m_InputManagerPtr->GetKeybind(keybind) };
		return new UiDynamicText{ pos,m_TextureManager,m_InputManagerPtr->GetKeybind(keybind),dataType,textColor,textureId,alignment };
	}
	else return nullptr;
}

UiElement* UiManager::GetTextureBar(const Json::Value& data)
{
	const Point2f pos{ data.get("x", 0.0f).asFloat(), data.get("y", 0.0f).asFloat() };
	const std::string texture{ data.get("texture", "").asString() };
	const float spacingX{ data.get("spacingX", 0.0f).asFloat() };
	const float spacingY{ data.get("spacingY", 0.0f).asFloat() };
	const std::string dataSource{ data.get("data", "").asString() };
	if (dataSource == "flask") return new UiTextureBar{ pos,texture,m_TextureManager,spacingX,spacingY,m_PlayerPtr->GetMaxFlask(),m_PlayerPtr->GetFlask() };
	else return nullptr;
}

UiElement* UiManager::GetRectf(const Json::Value& data)
{
	const Point2f pos{ data.get("x", 0.0f).asFloat(), data.get("y", 0.0f).asFloat() };
	const float width{ data.get("w", 0.0f).asFloat() };
	const float height{ data.get("h", 0.0f).asFloat() };
	const Color4f color{	data["color"].get("R", 1.0f).asFloat(),
							data["color"].get("G", 1.0f).asFloat(),
							data["color"].get("B", 1.0f).asFloat(),
							data["color"].get("A", 1.0f).asFloat() };
	return new UiRectf{ pos, width, height, color };
}

UiElement* UiManager::GetGrid(const Json::Value& data)
{
	const InputManager::Keybind cycleLeft{ (InputManager::Keybind)data.get("cycleLeft", 0).asInt() };
	const InputManager::Keybind cycleRight{ (InputManager::Keybind)data.get("cycleRight", 0).asInt() };
	const InputManager::Keybind cycleUp{ (InputManager::Keybind)data.get("cycleUp", 0).asInt() };
	const InputManager::Keybind cycleDown{ (InputManager::Keybind)data.get("cycleDown", 0).asInt() };
	UIGrid* grid{ new UIGrid{cycleLeft,cycleRight,cycleUp,cycleDown,m_SoundManagerPtr,m_InputManagerPtr} };
	for (int listIndex{}; listIndex < static_cast<int>(data["elements"].size()); ++listIndex) {
		grid->AddRow();
		for (int elementIndex{}; elementIndex < static_cast<int>(data["elements"][listIndex].size()); ++elementIndex) {
			grid->AddElement(GetElement(data["elements"][listIndex][elementIndex]));
		}
	}
	return grid;
}

UiElement* UiManager::GetButton(const Json::Value& data)
{
	const std::string functionData{ data.get("functionData", "").asString() };
	const int functionId{ data.get("functionId", -1).asInt() };
	return new UiButton{ m_InputManagerPtr, this, m_SoundManagerPtr, functionId, functionData };
}

UiElement* UiManager::GetVolumeSlider(const Json::Value& data)
{
	const Point2f pos{ data.get("x", 0.0f).asFloat(), data.get("y", 0.0f).asFloat() };
	const std::string sliderTexture{ data.get("sliderTexture", "").asString() };
	const float width{ data.get("w", 0.0f).asFloat() };
	const float height{ data.get("h", 0.0f).asFloat() };
	const int steps{ data.get("steps", 0.0f).asInt() };
	const InputManager::Keybind cycleLeft{ (InputManager::Keybind)data.get("cycleLeft", 0).asInt() };
	const InputManager::Keybind cycleRight{ (InputManager::Keybind)data.get("cycleRight", 0).asInt() };
	const int soundType{ data.get("soundType", 0.0f).asInt() };
	const Color4f color{	data["color"].get("R", 1.0f).asFloat(),
							data["color"].get("G", 1.0f).asFloat(),
							data["color"].get("B", 1.0f).asFloat(),
							data["color"].get("A", 1.0f).asFloat() };
	const Color4f borderColor{	data["borderColor"].get("R", 1.0f).asFloat(),
								data["borderColor"].get("G", 1.0f).asFloat(),
								data["borderColor"].get("B", 1.0f).asFloat(),
								data["borderColor"].get("A", 1.0f).asFloat() };
	return new UiVolumeSlider{ pos, m_SoundManagerPtr, m_TextureManager, m_InputManagerPtr, cycleLeft, cycleRight, width, height, steps, soundType, color, borderColor, sliderTexture };
}

UiElement* UiManager::GetInventorySlot(const Json::Value& data)
{
	const Point2f pos{ data.get("x", 0.0f).asFloat(), data.get("y", 0.0f).asFloat() };
	const Point2f dispPos{ data.get("dispX", 0.0f).asFloat(), data.get("dispY", 0.0f).asFloat() };
	const Point2f namePos{ data.get("nameX", 0.0f).asFloat(), data.get("nameY", 0.0f).asFloat() };
	const Point2f descPos{ data.get("descX", 0.0f).asFloat(), data.get("descY", 0.0f).asFloat() };
	const CategoryId category{ (CategoryId)data.get("category", 0).asInt() };
	const int index{ data.get("index", 0).asInt() };
	return new UiInventorySlot{ pos, dispPos, namePos, descPos, m_SaveManagerPtr, m_TextureManager, category, index };
}
