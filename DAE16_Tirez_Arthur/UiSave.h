#pragma once
#include "UiElement.h"

class SaveManager;
class TextureManager;
class InputManager;
class UiManager;
class UiStaticGraphic;
class UiStaticText;

class UiSave final : public UiElement
{
public:
	explicit UiSave(const Point2f& pos, int saveIndex, SaveManager* saveManager, TextureManager* textureManager, InputManager* inputManager, UiManager* uiManager);
	explicit UiSave(const UiSave& other) = delete;
	explicit UiSave(UiSave&& other) = delete;
	~UiSave();

	UiSave& operator=(const UiSave& other) = delete;
	UiSave& operator=(UiSave&& other) = delete;

	virtual void Update(float elapsedSec, bool selected = false) override;
	virtual void Draw(bool selected = false) const override;
private:
	void UpdateInfo();
	UiManager* m_UiManagerPtr;
	SaveManager* m_SaveManagerPtr;
	InputManager* m_InputManagerPtr;
	TextureManager* m_TextureManagerPtr;
	UiStaticGraphic* m_SaveBorder{};
	UiStaticText* m_TearsDisplay{};
	UiStaticText* m_RegionDisplay{};
	std::string m_SaveRegion{};
	bool m_SaveExists{};
	int m_Tears{-1};
	const int m_SaveIndex;
};

