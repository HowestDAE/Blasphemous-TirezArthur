#pragma once
#include "UiElement.h"
#include <vector>

class UiCollection final : public UiElement
{
public:
	explicit UiCollection(bool alwaysDisplay = true);
	explicit UiCollection(const UiCollection&) = delete;
	explicit UiCollection(UiCollection&&) = delete;
	virtual ~UiCollection() override;

	UiCollection& operator=(const UiCollection& other) = delete;
	UiCollection& operator=(UiCollection&& other) = delete;

	virtual void Update(float elapsedSec, bool selected = false) override;
	virtual void Draw(bool selected = false) const override;

	void AddElement(UiElement* element);
private:
	std::vector<UiElement*> m_Elements;
	const bool m_AlwaysDisplay;
};

