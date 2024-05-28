#include "pch.h"
#include "UiNumericDisplay.h"
#include "TextureManager.h"

UiNumericDisplay::UiNumericDisplay(float xPos, float yPos, TextureManager* texureManager, const int& number, const Color4f& textColor, const std::string& textureId, bool leftAligned) :
	UiNumericDisplay{ Point2f{xPos, yPos}, texureManager, number, textColor, textureId, leftAligned }
{
}

UiNumericDisplay::UiNumericDisplay(const Point2f& pos, TextureManager* texureManager, const int& number, const Color4f& textColor, const std::string& textureId, bool leftAligned) :
	UiElement{pos},
	m_TextureManagerPtr{texureManager},
	m_Number{number},
	m_PrevValue{number},
	m_TextureId{ textureId },
	m_TextColor{ textColor },
	m_LeftAligned{ leftAligned }
{
	m_TextureManagerPtr->MakeTextTexture(textureId, "blasphemous.ttf", std::to_string(number), textColor, 13);
}

void UiNumericDisplay::Update(float elapsedSec)
{
	if (m_PrevValue != m_Number) m_TextureManagerPtr->MakeTextTexture(m_TextureId, "blasphemous.ttf", std::to_string(m_Number), m_TextColor, 13);
	m_PrevValue = m_Number;
}

void UiNumericDisplay::Draw(bool selected) const
{
	if (m_LeftAligned )m_TextureManagerPtr->Draw(m_TextureId, m_Position);
	else m_TextureManagerPtr->Draw(m_TextureId, m_Position.x - m_TextureManagerPtr->GetTextureWidth(m_TextureId), m_Position.y);
}
