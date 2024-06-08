#pragma once
#include "UiElement.h"
#include "InputManager.h"
#include <vector>

class UiList;
class SoundManager;

class UIGrid final : public UiElement
{
public:
	explicit UIGrid(InputManager::Keybind cycleLeft, InputManager::Keybind cycleRight, InputManager::Keybind cylcleUp, InputManager::Keybind cycleDown, SoundManager* soundManager, InputManager* inputManager);
	explicit UIGrid(const UIGrid&) = delete;
	explicit UIGrid(UIGrid&&) = delete;
	virtual ~UIGrid() override;

	UIGrid& operator=(const UIGrid& other) = delete;
	UIGrid& operator=(UIGrid&& other) = delete;

	virtual void Update(float elapsedSec, bool selected = false) override;
	virtual void Draw(bool selected = false) const override;

	void AddElement(UiElement* element, int row = -1);
	void AddRow();
private:
	SoundManager* m_SoundManagerPtr;
	InputManager* m_InputManagerPtr;
	InputManager::Keybind m_CycleLeft;
	InputManager::Keybind m_CycleRight;
	InputManager::Keybind m_CycleUp;
	InputManager::Keybind m_CycleDown;
	bool m_KeyProcessed{ false };
	int m_SelectedIndex{ 0 };
	std::vector<UiList*> m_Lists;
};

