#include "pch.h"
#include "UiElement.h"

UiElement::UiElement(float xPos, float yPos) :
	UiElement{ Point2f{xPos, yPos} }
{
}

UiElement::UiElement(const Point2f& pos) : 
	m_Position{pos}
{
}

UiElement::~UiElement()
{
}

void UiElement::Update(float elapsedSec, bool selected)
{
}

void UiElement::Draw(bool selected) const
{
}
