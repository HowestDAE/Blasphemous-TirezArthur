#pragma once
#include "UiElement.h"
#include <vector>

class UiList final : public UiElement
{
public:
	explicit UiList(SDL_Scancode cycleLeft, SDL_Scancode cycleRight);
	explicit UiList(const UiList&) = delete;
	explicit UiList(UiList&&) = delete;
	virtual ~UiList() override;

	UiList& operator=(const UiList& other) = delete;
	UiList& operator=(UiList&& other) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw(bool selected = false) const override;

	void AddElement(UiElement* element);
private:
	SDL_Scancode m_CycleLeft;
	SDL_Scancode m_CycleRight;
	bool m_KeyProcessed{ false };
	int m_SelectedIndex{ 0 };
	std::vector<UiElement*> m_Elements;
};

