#include "pch.h"
#include "TextureManager.h"
#include "UiInventorySlot.h"
#include "UiStaticGraphic.h"
#include "UIStaticText.h"

UiInventorySlot::UiInventorySlot(const Point2f& pos, const Point2f& displayPos, const Point2f& namePos, const Point2f& descriptionPos, SaveManager* saveManager, TextureManager* textureManager, CategoryId category, int itemIndex) :
	UiElement{ pos },
	m_TextureManagerPtr{ textureManager },
	m_SaveManagerPtr{ saveManager },
	m_ItemIndex{ itemIndex },
	m_Category{ category },
	m_DisplayPos{displayPos},
	m_NamePos{namePos},
	m_DescriptionPos{descriptionPos}
{
	m_ItemSlot = new UiStaticGraphic{ m_Position, "inventory_slot_empty", m_TextureManagerPtr };
	m_Item = nullptr;
	m_Name = nullptr;
	m_Description = nullptr;
	CheckSaveManager();
}

UiInventorySlot::~UiInventorySlot()
{
	delete m_ItemSlot;
	if (m_Item != nullptr) delete m_Item;
	if (m_Name != nullptr) delete m_Name;
	if (m_Description != nullptr) delete m_Description;
}

void UiInventorySlot::Update(float elapsedSec, bool selected)
{
	if (m_Item == nullptr && m_SaveManagerPtr->GetCollectedAmount() != m_PrevSize) CheckSaveManager();
}

void UiInventorySlot::Draw(bool selected) const
{
	if (selected) {
		m_ItemSlot->Draw(m_DisplayPos);
		if (m_Item != nullptr) m_Item->Draw(m_DisplayPos);
		if (m_Name != nullptr) m_Name->Draw();
		if (m_Description != nullptr) m_Description->Draw();
	}
	m_ItemSlot->Draw(selected);
	if (m_Item != nullptr) m_Item->Draw();
}

void UiInventorySlot::CheckSaveManager()
{
	m_PrevSize = m_SaveManagerPtr->GetCollectedAmount();
	Item item{ m_SaveManagerPtr->GetItem(m_Category, m_ItemIndex) };
	if (item.id == ItemId::null) return;
	delete m_ItemSlot;
	m_ItemSlot = new UiStaticGraphic{ m_Position, "inventory_slot_filled", m_TextureManagerPtr };
	m_Item = new UiStaticGraphic{ m_Position, "item_" + std::to_string((int)item.id), m_TextureManagerPtr };
	m_Name = new UiStaticText{ m_NamePos, m_TextureManagerPtr, Color4f{0.671f, 0.604f,0.247f,1.0f}, Color4f{0.671f, 0.604f,0.247f,1.0f}, "item_name_" + std::to_string((int)item.id), item.name, UiStaticText::Alignment::bottomleft };
	m_Description = new UiStaticText{ m_DescriptionPos, m_TextureManagerPtr, Color4f{0.396f, 0.376f,0.267f,1.0f}, Color4f{0.396f, 0.376f,0.267f,1.0f}, "item_description_" + std::to_string((int)item.id), item.description, UiStaticText::Alignment::topleft };
}
