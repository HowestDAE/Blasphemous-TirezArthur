#pragma once
#define JSON_IS_AMALGAMATION
#include "json/json-forwards.h"
#include <vector>

class TextureManager;
class UiElement;
class Player;

class UiManager final
{
public:
	explicit UiManager(TextureManager* textureManager, Player* player);
	~UiManager();

	void Update(float elapsedSec);
	void Draw() const;
	bool GamePaused() const;

	void LoadScreen(const std::string& path);
private:
	UiElement* GetElement(const Json::Value& data);
	UiElement* GetStaticGraphic(const Json::Value& data);
	UiElement* GetList(const Json::Value& data);
	UiElement* GetCollection(const Json::Value& data);
	UiElement* GetStaticText(const Json::Value& data);
	UiElement* GetBar(const Json::Value& data);
	UiElement* GetNumericDisplay(const Json::Value& data);
	UiElement* GetTextureBar(const Json::Value& data);


	std::vector<UiElement*> m_ScreenVector;
	TextureManager* m_TextureManager;
	Player* m_PlayerPtr;
	bool m_GamePaused{ false };
};

