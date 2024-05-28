#pragma once
#include "UiElement.h"

class TextureManager;

class UiNumericDisplay final : public UiElement
{
public:
	explicit UiNumericDisplay(float xPos, float yPos, TextureManager* texureManager, const int& number, const Color4f& textColor, const std::string& textureId, bool leftAligned = true);
	explicit UiNumericDisplay(const Point2f& pos, TextureManager* texureManager, const int& number, const Color4f& textColor, const std::string& textureId, bool leftAligned = true);

	virtual void Update(float elapsedSec) override;
	virtual void Draw(bool selected = false) const override;
private:
	TextureManager* m_TextureManagerPtr;
	Color4f m_TextColor;
	std::string m_TextureId;
	const int& m_Number;
	const bool m_LeftAligned;
	int m_PrevValue;
};

