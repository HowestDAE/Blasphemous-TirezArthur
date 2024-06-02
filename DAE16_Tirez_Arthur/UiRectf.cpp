#include "pch.h"
#include "UiRectf.h"
#include "Utils.h"

UiRectf::UiRectf(float xPos, float yPos, float width, float height, const Color4f& color) :
	UiRectf{Point2f{xPos, yPos}, width, height, color}
{
}

UiRectf::UiRectf(const Point2f& pos, float width, float height, const Color4f& color) :
	UiElement{pos},
	m_Width{width},
	m_Height{height},
	m_Color{color}
{
}

UiRectf::UiRectf(const Rectf& rect, const Color4f& color) :
	UiRectf{rect.left, rect.bottom, rect.width, rect.height, color}
{
}

void UiRectf::Update(float elapsedSec)
{
}

void UiRectf::Draw(bool selected) const
{
	utils::SetColor(m_Color);
	utils::FillRect(m_Position, m_Width, m_Height);
}
