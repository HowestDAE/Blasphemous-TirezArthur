#include "pch.h"
#include "UiAnimatedGraphic.h"
#include "TextureManager.h"

UiAnimatedGraphic::UiAnimatedGraphic(float xPos, float yPos, std::string texture, TextureManager* textureManager, float frameModifier, bool flipped) :
	UiAnimatedGraphic{ Point2f{xPos, yPos}, texture, textureManager, frameModifier }
{
}

UiAnimatedGraphic::UiAnimatedGraphic(const Point2f& pos, std::string texture, TextureManager* textureManager, float frameModifier, bool flipped) :
	UiElement{ pos },
	m_Texture{ texture },
	m_TextureManager{ textureManager },
	m_FrameModifier{ frameModifier },
	m_Flipped{flipped}
{
}

void UiAnimatedGraphic::Update(float elapsedSec, bool selected)
{
	m_AnimationDuration += elapsedSec;
}

void UiAnimatedGraphic::Draw(bool selected) const
{
	if (!selected) m_TextureManager->Animate(m_Texture, m_Position, m_AnimationDuration, m_Flipped, true, m_FrameModifier);
	else m_TextureManager->Animate(m_Texture+"_selected", m_Position, m_AnimationDuration, m_Flipped, true, m_FrameModifier);
}
