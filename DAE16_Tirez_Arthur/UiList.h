#pragma once
#include "UiElement.h"
#include <vector>

class UiList final : public UiElement
{
public:
	explicit UiList(float xPos, float yPos, bool vertical);
	explicit UiList(const Point2f& pos, bool vertical);
	explicit UiList(const UiList&) = delete;
	explicit UiList(UiList&&) = delete;
	virtual ~UiList() override;

	virtual void Update(float elapsedSec) override;
	virtual void Draw(bool selected = false) const override;

	void AddElement(UiElement* element);
private:
	bool m_Vertical;
	int m_SelectedIndex{ 0 };
	std::vector<UiElement*> m_Elements;
};

