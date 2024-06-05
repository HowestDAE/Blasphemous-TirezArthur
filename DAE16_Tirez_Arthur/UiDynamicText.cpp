#include "pch.h"
#include "UiDynamicText.h"
#include "TextureManager.h"

UiDynamicText::UiDynamicText(float xPos, float yPos, TextureManager* texureManager, const int& number, DataType dataType, const Color4f& textColor, const std::string& textureId, bool leftAligned, bool centerAligned) :
	UiDynamicText{ Point2f{xPos, yPos}, texureManager, number, dataType, textColor, textureId, leftAligned, centerAligned }
{
}

UiDynamicText::UiDynamicText(const Point2f& pos, TextureManager* texureManager, const int& number, DataType dataType, const Color4f& textColor, const std::string& textureId, bool leftAligned, bool centerAligned) :
	UiElement{ pos },
	m_TextureManagerPtr{ texureManager },
	m_Number{ number },
	m_PrevValue{ number },
	m_TextureId{ textureId },
	m_TextColor{ textColor },
	m_LeftAligned{ leftAligned },
	m_CenterAligned{ centerAligned },
	m_DataType{ dataType }
{
	MakeNewTexture();
}

void UiDynamicText::Update(float elapsedSec)
{
	if (m_PrevValue != m_Number) MakeNewTexture();
	m_PrevValue = m_Number;
}

void UiDynamicText::Draw(bool selected) const
{
	if (m_CenterAligned) m_TextureManagerPtr->Draw(m_TextureId, m_Position.x - m_TextureManagerPtr->GetTextureWidth(m_TextureId) * 0.5f, m_Position.y);
	else if (m_LeftAligned)m_TextureManagerPtr->Draw(m_TextureId, m_Position);
	else m_TextureManagerPtr->Draw(m_TextureId, m_Position.x - m_TextureManagerPtr->GetTextureWidth(m_TextureId), m_Position.y);
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
