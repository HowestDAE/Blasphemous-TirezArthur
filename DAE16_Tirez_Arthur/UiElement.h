#pragma once
class UiElement
{
public:
	explicit UiElement(float xPos, float yPos);
	explicit UiElement(const Point2f& pos);
	explicit UiElement(const UiElement&) = delete;
	explicit UiElement(UiElement&&) = delete;
	virtual ~UiElement();

	UiElement& operator=(const UiElement&) = delete;
	UiElement& operator=(UiElement&&) = delete;

	virtual void Update(float elapsedSec, bool selected = true) = 0;
	virtual void Draw(bool selected = false) const = 0;
protected:
	Point2f m_Position;
};