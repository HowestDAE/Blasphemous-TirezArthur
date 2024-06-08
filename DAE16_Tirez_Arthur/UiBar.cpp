#include "pch.h"
#include "UiBar.h"
#include "utils.h"

UiBar::UiBar(float xPos, float yPos, float width, float height, const Color4f& color, const float& max, const float& current) :
	UiBar{ Point2f{xPos, yPos}, width, height, color, max, current }
{
}

UiBar::UiBar(const Point2f& pos, float width, float height, const Color4f& color, const float& max, const float& current) :
	UiElement{ pos },
	m_Width{ width },
	m_Height{ height },
	m_Color{ color },
	m_MaxValue{ max },
	m_CurrentValue{ current }
{
}

void UiBar::Update(float elapsedSec, bool selected)
{
}

void UiBar::Draw(bool selected) const
{
	const float progress{ m_CurrentValue / m_MaxValue };
	utils::SetColor(m_Color);
	utils::FillRect(m_Position, m_Width * progress, m_Height);
}
