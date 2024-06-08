#pragma once
#include "UiElement.h"
#include "SaveManager.h"

class UiStaticGraphic;
class UiStaticText;
class TextureManager;

class UiInventorySlot final : public UiElement
{
public:
	explicit UiInventorySlot(const Point2f& pos, const Point2f& displayPos, const Point2f& namePos, const Point2f& descriptionPos, SaveManager* saveManager, TextureManager* textureManager, CategoryId category, int itemIndex);
	explicit UiInventorySlot(const UiInventorySlot& other) = delete;
	explicit UiInventorySlot(UiInventorySlot&& other) = delete;
	virtual ~UiInventorySlot();

	UiInventorySlot& operator=(const UiInventorySlot& other) = delete;
	UiInventorySlot& operator=(UiInventorySlot&& other) = delete;

	virtual void Update(float elapsedSec, bool selected = false);
	virtual void Draw(bool selected) const;
private:
	void CheckSaveManager();
	SaveManager* m_SaveManagerPtr;
	TextureManager* m_TextureManagerPtr;
	UiStaticGraphic* m_Item;
	UiStaticGraphic* m_ItemSlot;
	UiStaticText* m_Name;
	UiStaticText* m_Description;

	CategoryId m_Category;
	Point2f m_DisplayPos;
	Point2f m_NamePos;
	Point2f m_DescriptionPos;

	int m_ItemIndex;
	int m_PrevSize{0};
};

