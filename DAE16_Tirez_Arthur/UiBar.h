#pragma once
#include "UiElement.h"

class UiBar final : public UiElement
{
public:
	explicit UiBar(float xPos, float yPos, float width, float height, const Color4f& color, const float& max, const float& current);
	explicit UiBar(const Point2f& pos, float width, float height, const Color4f& color, const float& max, const float& current);

	virtual void Update(float elapsedSec);
	virtual void Draw(bool selected) const;
private:
	const float& m_MaxValue;
	const float& m_CurrentValue;
	Color4f m_Color;
	const float m_Width;
	const float m_Height;
};

