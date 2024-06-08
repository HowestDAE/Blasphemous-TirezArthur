#pragma once
#include "UiElement.h"

class TextureManager;

class UiTextureBar final : public UiElement
{
public:
	explicit UiTextureBar(float xPos, float yPos, const std::string& texture, TextureManager* textureManager, float spacingX, float spacingY, const int& max, const int& current);
	explicit UiTextureBar(const Point2f& pos, const std::string& texture, TextureManager* textureManager, float spacingX, float spacingY, const int& max, const int& current);

	virtual void Update(float elapsedSec, bool selected = false);
	virtual void Draw(bool selected) const;
private:
	TextureManager* m_TextureManager;
	const std::string m_Texture;
	const int& m_MaxValue;
	const int& m_CurrentValue;
	const float m_SpacingX;
	const float m_SpacingY;
};

