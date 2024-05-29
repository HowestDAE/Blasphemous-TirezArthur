#pragma once
#include "UiElement.h"

class TextureManager;

class UiStaticText final : public UiElement
{
public:
	explicit UiStaticText(float xPos, float yPos, TextureManager* texureManager, const Color4f& textColor, const Color4f& selectedColor, const std::string& textureId, const std::string& text, bool leftAligned = true);
	explicit UiStaticText(const Point2f& pos, TextureManager* texureManager, const Color4f& textColor, const Color4f& selectedColor, const std::string& textureId, const std::string& text, bool leftAligned = true);

	virtual void Update(float elapsedSec) override;
	virtual void Draw(bool selected = false) const override;
private:
	TextureManager* m_TextureManagerPtr;
	const bool m_LeftAligned;
	std::string m_TextureId;
};

