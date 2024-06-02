#include "pch.h"
#include "UIGrid.h"
#include "UiList.h"
#include "SoundManager.h"

UIGrid::UIGrid(InputManager::Keybind cycleLeft, InputManager::Keybind cycleRight, InputManager::Keybind cylcleUp, InputManager::Keybind cycleDown, SoundManager* soundManager, InputManager* inputManager) :
	UiElement{0.0f, 0.0f},
	m_CycleDown{cycleDown},
	m_CycleLeft{cycleLeft},
	m_CycleUp{cylcleUp},
	m_CycleRight{cycleRight},
	m_SoundManagerPtr{soundManager},
	m_InputManagerPtr{inputManager}
{
}

UIGrid::~UIGrid()
{
}

void UIGrid::Update(float elapsedSec)
{
	const bool upHeld{ m_InputManagerPtr->GetKeyState(m_CycleUp) };
	const bool downHeld{ m_InputManagerPtr->GetKeyState(m_CycleDown) };

	if ((upHeld || downHeld)) {
		if (!m_KeyProcessed) {
			if (upHeld && !downHeld) m_SelectedIndex = (m_SelectedIndex - 1 + (int)m_Lists.size()) % (int)m_Lists.size();
			if (!upHeld && downHeld) m_SelectedIndex = (m_SelectedIndex + 1) % (int)m_Lists.size();
			m_SoundManagerPtr->Play("menu_select");
		}
		m_KeyProcessed = true;
	}
	else {
		m_KeyProcessed = false;
	}

	m_Lists.at(m_SelectedIndex)->Update(elapsedSec);

	for (UiList* list : m_Lists) {
		list->SetSelectedIndex(m_Lists.at(m_SelectedIndex)->GetSelectedIndex());
	}
}

void UIGrid::Draw(bool selected) const
{
	for (UiElement* element : m_Lists) {
		if (element != nullptr) element->Draw(element == m_Lists.at(m_SelectedIndex));
	}
}

void UIGrid::AddElement(UiElement* element, int row)
{
	if (row == -1) {
		m_Lists.at(m_Lists.size() - 1)->AddElement(element);
	}
	else if (row < m_Lists.size()) {
		m_Lists.at(row)->AddElement(element);
	}
}

void UIGrid::AddRow()
{
	m_Lists.emplace_back(new UiList{ m_CycleLeft, m_CycleRight, m_SoundManagerPtr, m_InputManagerPtr, true });
}
