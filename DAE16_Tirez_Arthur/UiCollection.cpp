#include "pch.h"
#include "UiCollection.h"

UiCollection::UiCollection(bool selectable) :
	UiElement{ 0.0f, 0.0f },
	m_AlwaysDisplay{selectable}
{
}

UiCollection::~UiCollection()
{
	for (UiElement* element : m_Elements) {
		if (element != nullptr) delete element;
	}
}

void UiCollection::Update(float elapsedSec, bool selected)
{
	if (!m_AlwaysDisplay && selected) {
		for (UiElement* element : m_Elements) {
			if (element != nullptr) element->Update(elapsedSec);
		}
	}
	else if (m_AlwaysDisplay) {
		for (UiElement* element : m_Elements) {
			if (element != nullptr) element->Update(elapsedSec, selected);
		}
	}
}

void UiCollection::Draw(bool selected) const
{
	if (!m_AlwaysDisplay && selected) {
		for (UiElement* element : m_Elements) {
			if (element != nullptr) element->Draw();
		}
	}
	else if (m_AlwaysDisplay) {
		for (UiElement* element : m_Elements) {
			if (element != nullptr) element->Draw(selected);
		}
	}
}

void UiCollection::AddElement(UiElement* element)
{
	if (element != nullptr) m_Elements.push_back(element);
}
