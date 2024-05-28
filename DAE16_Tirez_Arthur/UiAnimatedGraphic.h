#pragma once
#include "UiElement.h"

class TextureManager;

class UiAnimatedGraphic final : public UiElement
{
public:
	explicit UiAnimatedGraphic(float xPos, float yPos, std::string texture, TextureManager* textureManager, float frameModifier = 1.0f, bool flipped = false);
	explicit UiAnimatedGraphic(const Point2f & pos, std::string texture, TextureManager * textureManager, float frameModifier = 1.0f, bool flipped = false);

	virtual void Update(float elapsedSec) override;
	virtual void Draw(bool selected = false) const override;
private:
	std::string m_Texture;
	TextureManager* m_TextureManager;
	float m_AnimationDuration{0.0f};
	const float m_FrameModifier;
	const bool m_Flipped;
};

