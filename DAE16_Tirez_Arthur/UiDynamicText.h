#pragma once
#include "UiElement.h"

class TextureManager;

class UiDynamicText final : public UiElement
{
public:
	enum class DataType {
		value,
		keybind
	};
	explicit UiDynamicText(float xPos, float yPos, TextureManager* texureManager, const int& number, DataType dataType, const Color4f& textColor, const std::string& textureId, bool leftAligned = true, bool centerAligned = false);
	explicit UiDynamicText(const Point2f& pos, TextureManager* texureManager, const int& number, DataType dataType, const Color4f& textColor, const std::string& textureId, bool leftAligned = true, bool centerAligned = false);

	virtual void Update(float elapsedSec) override;
	virtual void Draw(bool selected = false) const override;
private:
	void MakeNewTexture();
	DataType m_DataType;
	TextureManager* m_TextureManagerPtr;
	Color4f m_TextColor;
	std::string m_TextureId;
	const int& m_Number;
	const bool m_LeftAligned;
	const bool m_CenterAligned;
	int m_PrevValue;
};

