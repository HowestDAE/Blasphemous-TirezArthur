#include "pch.h"
#include "UiStaticText.h"
#include "TextureManager.h"

UiStaticText::UiStaticText(float xPos, float yPos, TextureManager* texureManager, const Color4f& textColor, const Color4f& selectedColor, const std::string& textureId, const std::string& text, Alignment alignment) :
	UiStaticText{ Point2f{ xPos, yPos }, texureManager, textColor, selectedColor, textureId, text, alignment }
{
}

UiStaticText::UiStaticText(const Point2f& pos, TextureManager* texureManager, const Color4f& textColor, const Color4f& selectedColor, const std::string& textureId, const std::string& text, Alignment alignment) :
	UiElement{pos},
	m_TextureManagerPtr{texureManager},
	m_Alignment{alignment},
	m_TextureId{textureId}
{
	m_TextureManagerPtr->MakeTextTexture(textureId, "blasphemous.ttf", text, textColor, 13);
	m_TextureManagerPtr->MakeTextTexture(textureId + "_selected", "blasphemous.ttf", text, selectedColor, 13);
}

void UiStaticText::Update(float elapsedSec, bool selected)
{
}

void UiStaticText::Draw(bool selected) const
{
	float xPos{ m_Position.x };
	float yPos{ m_Position.y };
	switch (m_Alignment)
	{
	case Alignment::left:
		yPos -= m_TextureManagerPtr->GetTextureHeight(m_TextureId) * 0.5f;
		break;
	case Alignment::topleft:
		yPos -= m_TextureManagerPtr->GetTextureHeight(m_TextureId);
		break;
	case Alignment::top:
		xPos -= m_TextureManagerPtr->GetTextureWidth(m_TextureId) * 0.5f;
		yPos -= m_TextureManagerPtr->GetTextureHeight(m_TextureId);
		break;
	case Alignment::topright:
		xPos -= m_TextureManagerPtr->GetTextureWidth(m_TextureId);
		yPos -= m_TextureManagerPtr->GetTextureHeight(m_TextureId);
		break;
	case Alignment::right:
		xPos -= m_TextureManagerPtr->GetTextureWidth(m_TextureId);
		yPos -= m_TextureManagerPtr->GetTextureHeight(m_TextureId) * 0.5f;
		break;
	case Alignment::bottomright:
		xPos -= m_TextureManagerPtr->GetTextureWidth(m_TextureId);
		break;
	case Alignment::bottom:
		xPos -= m_TextureManagerPtr->GetTextureWidth(m_TextureId) * 0.5f;
		break;
	case Alignment::bottomleft:
		break;
	}
	if (!selected) m_TextureManagerPtr->Draw(m_TextureId, xPos, yPos);
	else m_TextureManagerPtr->Draw(m_TextureId + "_selected", xPos, yPos);
}
