#include "pch.h"
#include "UiTextureBar.h"
#include "TextureManager.h"

UiTextureBar::UiTextureBar(float xPos, float yPos, const std::string& texture, TextureManager* textureManager, float spacingX, float spacingY, const int& max, const int& current) :
	UiTextureBar{ Point2f{xPos, yPos}, texture, textureManager, spacingX, spacingY, max, current}
{
}

UiTextureBar::UiTextureBar(const Point2f& pos, const std::string& texture, TextureManager* textureManager, float spacingX, float spacingY, const int& max, const int& current) :
	UiElement{pos},
	m_TextureManager{ textureManager},
	m_Texture{ texture },
	m_CurrentValue{current},
	m_MaxValue{max},
	m_SpacingX{spacingX},
	m_SpacingY{spacingY}
{
}

void UiTextureBar::Update(float elapsedSec, bool selected)
{
}

void UiTextureBar::Draw(bool selected) const
{
	for (int index{}; index < m_CurrentValue; ++index) {
		m_TextureManager->Draw(m_Texture + "_selected", m_Position.x + index * m_SpacingX, m_Position.y + m_SpacingY * index);
	}
	for (int index{m_CurrentValue}; index < m_MaxValue; ++index) {
		m_TextureManager->Draw(m_Texture, m_Position.x + index * m_SpacingX, m_Position.y + m_SpacingY * index);
	}
}
