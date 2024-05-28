#include "pch.h"
#include "UiList.h"

UiList::UiList(float xPos, float yPos, bool vertical) :
	UiList{ Point2f{xPos, yPos}, vertical }
{
}

UiList::UiList(const Point2f& pos, bool vertical) :
	UiElement{ pos },
	m_Vertical{ vertical }
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
	const bool leftHeld{ (bool)keyBoardState[SDL_SCANCODE_LEFT] };
	const bool rightHeld{ (bool)keyBoardState[SDL_SCANCODE_RIGHT] };

	if ((leftHeld || rightHeld)) {
		if (leftHeld && !rightHeld && !m_Vertical) m_SelectedIndex = (m_SelectedIndex - 1 + m_Elements.size()) % m_Elements.size();
		if (!leftHeld && rightHeld && !m_Vertical) m_SelectedIndex = (m_SelectedIndex + 1) % m_Elements.size();
		m_Vertical = true;
	}
	else {
		m_Vertical = false;
	}

	for (UiElement* element : m_Elements) {
		if (element != nullptr) element->Update(elapsedSec);
	}
}

void UiList::Draw(bool selected) const
{
	for (UiElement* element : m_Elements) {
		if (element != nullptr) element->Draw(element == m_Elements.at(m_SelectedIndex));
	}
}

void UiList::AddElement(UiElement* element)
{
	m_Elements.push_back(element);
}
