#include "pch.h"
#include "UiList.h"
#include "SoundManager.h"

UiList::UiList(InputManager::Keybind cycleLeft, InputManager::Keybind cycleRight, SoundManager* soundManager, InputManager* inputManager, bool selectable) :
	UiElement{ 0.0f, 0.0f },
	m_CycleLeft{cycleLeft},
	m_CycleRight{cycleRight},
	m_SoundManagerPtr{soundManager},
	m_Selectable{selectable},
	m_InputManagerPtr{inputManager}
{
}

UiList::~UiList()
{
	for (UiElement* element : m_Elements) {
		if (element != nullptr) delete element;
	}
}

void UiList::Update(float elapsedSec)
{
	const bool leftHeld{ m_InputManagerPtr->GetKeyState(m_CycleLeft) };
	const bool rightHeld{ m_InputManagerPtr->GetKeyState(m_CycleRight) };

	if ((leftHeld || rightHeld)) {
		if (!m_KeyProcessed){
			if (leftHeld && !rightHeld) m_SelectedIndex = (m_SelectedIndex - 1 + (int)m_Elements.size()) % (int)m_Elements.size();
			if (!leftHeld && rightHeld) m_SelectedIndex = (m_SelectedIndex + 1) % (int)m_Elements.size();
			m_SoundManagerPtr->Play("menu_select");
		}
		m_KeyProcessed = true;
	}
	else {
		m_KeyProcessed = false;
	}

	m_Elements.at(m_SelectedIndex)->Update(elapsedSec);
}

void UiList::Draw(bool selected) const
{
	for (UiElement* element : m_Elements) {
		if (element != nullptr) element->Draw(element == m_Elements.at(m_SelectedIndex) && !(m_Selectable && !selected));
	}
}

void UiList::AddElement(UiElement* element)
{
	if (element != nullptr) m_Elements.push_back(element);
}

int UiList::GetSelectedIndex() const
{
	return m_SelectedIndex;
}

void UiList::SetSelectedIndex(int index)
{
	if (index < m_Elements.size() - 1) m_SelectedIndex = index;
	else m_SelectedIndex = m_Elements.size() - 1;
}
