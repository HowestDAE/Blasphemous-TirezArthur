#pragma once
#include "UiElement.h"
#include "InputManager.h"
#include <vector>

class SoundManager;

class UiList final : public UiElement
{
public:
	explicit UiList(InputManager::Keybind cycleLeft, InputManager::Keybind cycleRight, SoundManager* soundManager, InputManager* inputManager, bool selectable = false);
	explicit UiList(const UiList&) = delete;
	explicit UiList(UiList&&) = delete;
	virtual ~UiList() override;

	UiList& operator=(const UiList& other) = delete;
	UiList& operator=(UiList&& other) = delete;

	virtual void Update(float elapsedSec, bool selected = true) override;
	virtual void Draw(bool selected = false) const override;

	void AddElement(UiElement* element);

	int GetSelectedIndex() const;
	void SetSelectedIndex(int index);
private:
	SoundManager* m_SoundManagerPtr;
	InputManager* m_InputManagerPtr;
	InputManager::Keybind m_CycleLeft;
	InputManager::Keybind m_CycleRight;
	bool m_KeyProcessed{ false };
	bool m_Selectable;
	int m_SelectedIndex{ 0 };
	std::vector<UiElement*> m_Elements;
};

