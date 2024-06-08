#pragma once
#include "UiElement.h"

class TextureManager;

class UiDynamicText final : public UiElement
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
	enum class DataType {
		value,
		keybind
	};
	explicit UiDynamicText(float xPos, float yPos, TextureManager* texureManager, const int& number, DataType dataType, const Color4f& textColor, const std::string& textureId, Alignment alignment = Alignment::bottomleft);
	explicit UiDynamicText(const Point2f& pos, TextureManager* texureManager, const int& number, DataType dataType, const Color4f& textColor, const std::string& textureId, Alignment alignment = Alignment::bottomleft);

	virtual void Update(float elapsedSec, bool selected = false) override;
	virtual void Draw(bool selected = false) const override;
private:
	void MakeNewTexture();
	DataType m_DataType;
	TextureManager* m_TextureManagerPtr;
	Color4f m_TextColor;
	std::string m_TextureId;
	const int& m_Number;
	const Alignment m_Alignment;
	int m_PrevValue;
};

