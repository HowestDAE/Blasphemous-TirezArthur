#include "pch.h"
#include "UiManager.h"
#include "UiElement.h"
#include "UiStaticGraphic.h"
#include "UiAnimatedGraphic.h"
#include "UiList.h"
#include "UiBar.h"
#include "UiNumericDisplay.h"
#include "UiTextureBar.h"
#include "UiCollection.h"
#include "UiStaticText.h"
#include "Player.h"
#include "json/json.h"
#include <iostream>
#include <fstream>

UiManager::UiManager( TextureManager* textureManager, Player* player ):
	m_TextureManager{ textureManager },
	m_PlayerPtr{ player }
{
	LoadScreen("game");
}

UiManager::~UiManager()
{
	for (UiElement*& UiElement : m_ScreenVector) {
		if (UiElement != nullptr) delete UiElement;
	}
}

void UiManager::Update(float elapsedSec)
{
	for (UiElement* UiElement : m_ScreenVector) {
		if (UiElement != nullptr) UiElement->Update(elapsedSec);
	}
}

void UiManager::Draw() const
{
	for (const UiElement* UiElement : m_ScreenVector) {
		if (UiElement != nullptr) UiElement->Draw();
	}
}

bool UiManager::GamePaused() const
{
	return m_GamePaused;
}

void UiManager::LoadScreen(const std::string& path)
{
	std::ifstream screenFile("UiScreens/" + path + ".json", std::ifstream::binary);
	if (!screenFile)
	{
		std::cout << "Screen \"" + path + "\" does not exist.";
		return;
	}

	for (UiElement*& UiElement : m_ScreenVector) {
		if (UiElement != nullptr) delete UiElement;
	}
	m_ScreenVector.clear();

	Json::Value screenData;
	screenFile >> screenData;

	m_GamePaused = screenData.get("gamePaused", false).asBool();

	for (int elementIndex{}; elementIndex < screenData["elements"].size(); ++elementIndex) {
		m_ScreenVector.push_back(GetElement(screenData["elements"][elementIndex]));
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
	else if (type == "numeric_display") return GetNumericDisplay(data);
	else if (type == "texture_bar") return GetTextureBar(data);
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
	const SDL_Scancode cycleLeft{ (SDL_Scancode)data.get("cycleLeft", 0).asInt() };
	const SDL_Scancode cycleRight{ (SDL_Scancode)data.get("cycleRight", 0).asInt() };
	UiList* list{ new UiList{cycleLeft,cycleRight} };
	for (int elementIndex{}; elementIndex < data["elements"].size(); ++elementIndex) {
		list->AddElement(GetElement(data["elements"][elementIndex]));
	}
	return list;
}

UiElement* UiManager::GetCollection(const Json::Value& data)
{
	const bool alwaysDisplay{ data.get("alwaysDisplay", true).asBool() };
	UiCollection* collection{ new UiCollection{ alwaysDisplay} };
	for (int elementIndex{}; elementIndex < data["elements"].size(); ++elementIndex) {
		collection->AddElement(GetElement(data["elements"][elementIndex]));
	}
	return collection;
}

UiElement* UiManager::GetStaticText(const Json::Value& data)
{
	const Point2f pos{ data.get("x", 0.0f).asFloat(), data.get("y", 0.0f).asFloat() };
	const std::string textureId{ data.get("textureId", "").asString() };
	const std::string text{ data.get("text", "").asString() };
	const Color4f textColor{	data["color"].get("R", 1.0f).asFloat(),
								data["color"].get("G", 1.0f).asFloat(),
								data["color"].get("B", 1.0f).asFloat(),
								data["color"].get("A", 1.0f).asFloat() };
	const Color4f selectedColor{data["selectedColor"].get("R", 1.0f).asFloat(),
								data["selectedColor"].get("G", 1.0f).asFloat(),
								data["selectedColor"].get("B", 1.0f).asFloat(),
								data["selectedColor"].get("A", 1.0f).asFloat() };
	const bool leftAligned{ data.get("leftAligned", true).asBool() };
	return new UiStaticText{ pos, m_TextureManager, textColor, selectedColor, textureId, text, leftAligned };
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

UiElement* UiManager::GetNumericDisplay(const Json::Value& data)
{
	const Point2f pos{ data.get("x", 0.0f).asFloat(), data.get("y", 0.0f).asFloat() };
	const std::string textureId{ data.get("textureId", "").asString() };
	const bool leftAligned{ data.get("leftAligned", true).asBool() };
	const Color4f textColor{ data["color"].get("R", 1.0f).asFloat(),
								data["color"].get("G", 1.0f).asFloat(),
								data["color"].get("B", 1.0f).asFloat(),
								data["color"].get("A", 1.0f).asFloat() };
	const std::string dataSource{ data.get("data", "").asString() };
	if (dataSource == "tears") return new UiNumericDisplay{ pos,m_TextureManager,m_PlayerPtr->GetTears(),textColor,textureId,leftAligned };
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
