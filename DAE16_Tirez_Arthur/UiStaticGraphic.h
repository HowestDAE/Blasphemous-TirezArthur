#pragma once
#include "UiElement.h"

class TextureManager;

class UiStaticGraphic final : public UiElement
{
public:
	explicit UiStaticGraphic(float xPos, float yPos, std::string texture, TextureManager* textureManager, bool flipped = false);
	explicit UiStaticGraphic(const Point2f& pos, std::string texture, TextureManager* textureManager, bool flipped = false);

	virtual void Update(float elapsedSec) override;
	virtual void Draw(bool selected = false) const override;
private:
	std::string m_Texture;
	TextureManager* m_TextureManager;
	const bool m_Flipped;
};

