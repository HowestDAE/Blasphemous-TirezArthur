#pragma once
#define JSON_IS_AMALGAMATION
#include "json/json-forwards.h"
#include <vector>
#include <map>

class SoundManager;
class TextureManager;
class UiElement;
class Player;
class SaveManager;
class InputManager;

class UiManager final
{
public:
	explicit UiManager(TextureManager* textureManager, Player* player, SoundManager* soundManager, InputManager* inputManager, SaveManager* saveManager);
	~UiManager();

	void Update(float elapsedSec);
	void Draw() const;
	bool GamePaused() const;

	void LoadScreen(const std::string& path);
private:
	void UiButtonFunction(int functionId, const std::string& data);

	struct Screen {
		bool gamePaused;
		std::vector<UiElement*> elements;
	};

	UiElement* GetElement(const Json::Value& data);
	UiElement* GetStaticGraphic(const Json::Value& data);
	UiElement* GetAnimatedGraphic(const Json::Value& data);
	UiElement* GetList(const Json::Value& data);
	UiElement* GetCollection(const Json::Value& data);
	UiElement* GetStaticText(const Json::Value& data);
	UiElement* GetBar(const Json::Value& data);
	UiElement* GetDynamicText(const Json::Value& data);
	UiElement* GetTextureBar(const Json::Value& data);
	UiElement* GetRectf(const Json::Value& data);
	UiElement* GetGrid(const Json::Value& data);
	UiElement* GetButton(const Json::Value& data);
	UiElement* GetVolumeSlider(const Json::Value& data);
	UiElement* GetInventorySlot(const Json::Value& data);
	UiElement* GetSaveDisplay(const Json::Value& data);

	SoundManager* m_SoundManagerPtr;
	InputManager* m_InputManagerPtr;
	SaveManager* m_SaveManagerPtr;
	std::map<std::string, Screen*> m_ScreenMap;
	std::vector<UiElement*> m_ScreenVector;
	std::string m_CurrentScreen;
	TextureManager* m_TextureManager;
	Player* m_PlayerPtr;
	SDL_Event* m_Quit{ new SDL_Event{SDL_QUIT} };
	bool m_GamePaused{ false };

	friend class UiButton;
};

