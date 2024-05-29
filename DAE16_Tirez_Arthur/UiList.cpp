#include "pch.h"
#include "UiList.h"

UiList::UiList(SDL_Scancode cycleLeft, SDL_Scancode cycleRight) :
	UiElement{ 0.0f, 0.0f },
	m_CycleLeft{cycleLeft},
	m_CycleRight{cycleRight}
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
	const Uint8* keyBoardState{ SDL_GetKeyboardState(nullptr) };
	const bool leftHeld{ (bool)keyBoardState[m_CycleLeft] };
	const bool rightHeld{ (bool)keyBoardState[m_CycleRight] };

	if ((leftHeld || rightHeld)) {
		if (leftHeld && !rightHeld && !m_KeyProcessed) m_SelectedIndex = (m_SelectedIndex - 1 + (int)m_Elements.size()) % (int)m_Elements.size();
		if (!leftHeld && rightHeld && !m_KeyProcessed) m_SelectedIndex = (m_SelectedIndex + 1) % (int)m_Elements.size();
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
		if (element != nullptr) element->Draw(element == m_Elements.at(m_SelectedIndex));
	}
}

void UiList::AddElement(UiElement* element)
{
	if (element != nullptr) m_Elements.push_back(element);
}
