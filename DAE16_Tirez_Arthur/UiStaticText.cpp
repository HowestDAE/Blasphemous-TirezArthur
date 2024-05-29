#include "pch.h"
#include "UiStaticText.h"
#include "TextureManager.h"

UiStaticText::UiStaticText(float xPos, float yPos, TextureManager* texureManager, const Color4f& textColor, const Color4f& selectedColor, const std::string& textureId, const std::string& text, bool leftAligned) :
	UiStaticText{ Point2f{ xPos, yPos }, texureManager, textColor, selectedColor, textureId, text, leftAligned }
{
}

UiStaticText::UiStaticText(const Point2f& pos, TextureManager* texureManager, const Color4f& textColor, const Color4f& selectedColor, const std::string& textureId, const std::string& text, bool leftAligned) :
	UiElement{pos},
	m_TextureManagerPtr{texureManager},
	m_LeftAligned{leftAligned},
	m_TextureId{textureId}
{
	m_TextureManagerPtr->MakeTextTexture(textureId, "blasphemous.ttf", text, textColor, 13);
	m_TextureManagerPtr->MakeTextTexture(textureId + "_selected", "blasphemous.ttf", text, selectedColor, 13);
}

void UiStaticText::Update(float elapsedSec)
{
}

void UiStaticText::Draw(bool selected) const
{
	if (m_LeftAligned){
		if (!selected) m_TextureManagerPtr->Draw(m_TextureId, m_Position);
		else m_TextureManagerPtr->Draw(m_TextureId + "_selected", m_Position);
	}
	else {
		if (!selected) m_TextureManagerPtr->Draw(m_TextureId, m_Position.x - m_TextureManagerPtr->GetTextureWidth(m_TextureId), m_Position.y);
		else m_TextureManagerPtr->Draw(m_TextureId + "_selected", m_Position.x - m_TextureManagerPtr->GetTextureWidth(m_TextureId), m_Position.y);
	}
}
