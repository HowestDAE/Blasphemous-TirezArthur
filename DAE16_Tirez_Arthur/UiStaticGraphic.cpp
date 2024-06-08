#include "pch.h"
#include "UiStaticGraphic.h"
#include "TextureManager.h"

UiStaticGraphic::UiStaticGraphic(float xPos, float yPos, std::string texture, TextureManager* textureManager, bool flipped) :
	UiStaticGraphic{ Point2f{xPos, yPos}, texture, textureManager, flipped }
{
}

UiStaticGraphic::UiStaticGraphic(const Point2f& pos, std::string texture, TextureManager* textureManager, bool flipped) :
	UiElement{pos},
	m_Texture{texture},
	m_TextureManager{ textureManager },
	m_Flipped{ flipped }
{
}

void UiStaticGraphic::Update(float elapsedSec, bool selected)
{

}

void UiStaticGraphic::Draw(const Point2f& pos, bool selected) const
{
	if (!selected) m_TextureManager->Draw(m_Texture, pos, m_Flipped);
	else m_TextureManager->Draw(m_Texture + "_selected", pos, m_Flipped);
}

void UiStaticGraphic::Draw(bool selected) const
{
	Draw(m_Position, selected);
}
