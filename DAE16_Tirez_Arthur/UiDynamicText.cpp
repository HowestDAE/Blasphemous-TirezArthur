#include "pch.h"
#include "UiDynamicText.h"
#include "TextureManager.h"

UiDynamicText::UiDynamicText(float xPos, float yPos, TextureManager* texureManager, const int& number, DataType dataType, const Color4f& textColor, const std::string& textureId, Alignment alignment) :
	UiDynamicText{ Point2f{xPos, yPos}, texureManager, number, dataType, textColor, textureId, alignment }
{
}

UiDynamicText::UiDynamicText(const Point2f& pos, TextureManager* texureManager, const int& number, DataType dataType, const Color4f& textColor, const std::string& textureId, Alignment alignment) :
	UiElement{ pos },
	m_TextureManagerPtr{ texureManager },
	m_Number{ number },
	m_PrevValue{ number },
	m_TextureId{ textureId },
	m_TextColor{ textColor },
	m_Alignment{alignment},
	m_DataType{ dataType }
{
	MakeNewTexture();
}

void UiDynamicText::Update(float elapsedSec, bool selected)
{
	if (m_PrevValue != m_Number) MakeNewTexture();
	m_PrevValue = m_Number;
}

void UiDynamicText::Draw(bool selected) const
{
	float xPos{m_Position.x};
	float yPos{m_Position.y};
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
	m_TextureManagerPtr->Draw(m_TextureId, xPos, yPos);
}

void UiDynamicText::MakeNewTexture()
{
	switch (m_DataType)
	{
	case UiDynamicText::DataType::value:
		m_TextureManagerPtr->MakeTextTexture(m_TextureId, "blasphemous.ttf", std::to_string(m_Number), m_TextColor, 13);
		break;
	case UiDynamicText::DataType::keybind:
		m_TextureManagerPtr->MakeTextTexture(m_TextureId, "blasphemous.ttf", SDL_GetKeyName(m_Number), m_TextColor, 13);
		break;
	default:
		break;
	}
}
