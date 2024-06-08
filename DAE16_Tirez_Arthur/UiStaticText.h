#pragma once
#include "UiElement.h"

class TextureManager;

class UiStaticText final : public UiElement
{
public:
	enum class Alignment {
		left,
		topleft,
		top,
		topright,
		right,
		bottomright,
		bottom,
		bottomleft
	};
	explicit UiStaticText(float xPos, float yPos, TextureManager* texureManager, const Color4f& textColor, const Color4f& selectedColor, const std::string& textureId, const std::string& text, Alignment alignment = Alignment::bottomleft);
	explicit UiStaticText(const Point2f& pos, TextureManager* texureManager, const Color4f& textColor, const Color4f& selectedColor, const std::string& textureId, const std::string& text, Alignment alignment = Alignment::bottomleft);

	virtual void Update(float elapsedSec, bool selected = false) override;
	virtual void Draw(bool selected = false) const override;
private:
	TextureManager* m_TextureManagerPtr;
	const Alignment m_Alignment;
	std::string m_TextureId;
};

