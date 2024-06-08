#pragma once
#include "UiElement.h"

class UiRectf final : public UiElement
{
public:
	UiRectf(float xPos, float yPos, float width, float height, const Color4f& color);
	UiRectf(const Point2f& pos, float width, float height, const Color4f& color);
	UiRectf(const Rectf& rect, const Color4f& color);

	virtual void Update(float elapsedSec, bool selected = false) override;
	virtual void Draw(bool selected = false) const override;
private:
	const float m_Width;
	const float m_Height;
	const Color4f m_Color;
};

