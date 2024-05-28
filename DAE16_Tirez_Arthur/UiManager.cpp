#include "pch.h"
#include "UiManager.h"
#include "UiElement.h"
#include "UiStaticGraphic.h"
#include "UiAnimatedGraphic.h"
#include "UiList.h"
#include "UiBar.h"
#include "UiNumericDisplay.h"
#include "UiTextureBar.h"
#include "Player.h"

UiManager::UiManager( TextureManager* textureManager, Player* player ):
	m_TextureManager{ textureManager },
	m_PlayerPtr{ player }
{
	m_ScreenVector.emplace_back(new UiStaticGraphic{ 66.0f, 318.0f, "penitent_portrait_background", m_TextureManager });
	m_ScreenVector.emplace_back(new UiBar{ 66.0f, 330.0f, 87.0f, 4.0f, Color4f{0.8117f, 0.1254f, 0.1529f, 1.0f}, m_PlayerPtr->GetMaxHealth(), m_PlayerPtr->GetHealth() });
	m_ScreenVector.emplace_back(new UiStaticGraphic{ 16.0f, 287.0f, "penitent_portrait", m_TextureManager });
	m_ScreenVector.emplace_back(new UiStaticGraphic{ 529.0f, 297.0f, "tears_display", m_TextureManager });
	//UiList* list{ new UiList{0.0f, 0.0f, false} };
	//list->AddElement(new UiStaticGraphic{ 0.0f, 0.0f, "inventory_cat_0", m_TextureManager });
	//list->AddElement(new UiStaticGraphic{ 30.0f, 0.0f, "inventory_cat_1", m_TextureManager });
	//list->AddElement(new UiStaticGraphic{ 60.0f, 0.0f, "inventory_cat_2", m_TextureManager });
	//list->AddElement(new UiStaticGraphic{ 90.0f, 0.0f, "inventory_cat_3", m_TextureManager });
	//list->AddElement(new UiStaticGraphic{ 120.0f, 0.0f, "inventory_cat_4", m_TextureManager });
	//list->AddElement(new UiStaticGraphic{ 150.0f, 0.0f, "inventory_cat_5", m_TextureManager });
	//list->AddElement(new UiStaticGraphic{ 180.0f, 0.0f, "inventory_cat_6", m_TextureManager });
	//m_ScreenVector.push_back(list);
	m_ScreenVector.emplace_back(new UiNumericDisplay{ 578.0f, 310.0f, m_TextureManager, m_PlayerPtr->GetTears(), Color4f{0.6705f, 0.6039f, 0.2470f, 1.0f}, "tears_value", false });
	m_ScreenVector.emplace_back(new UiTextureBar{77.0f, 291.0f, "flask", m_TextureManager, 17.0f, 0.0f, m_PlayerPtr->GetMaxFlask(), m_PlayerPtr->GetFlask()});
	//m_ScreenVector.emplace_back(new UiAnimatedGraphic{ 0.0f, 0.0f, "penitent_main_menu", m_TextureManager, 0.3f });
}

UiManager::~UiManager()
{
	for (UiElement*& UiElement : m_ScreenVector) {
		if (UiElement != nullptr) delete UiElement;
	}
}

void UiManager::Update(float elapsedSec)
{
	for (UiElement* UiElement : m_ScreenVector) {
		if (UiElement != nullptr) UiElement->Update(elapsedSec);
	}
}

void UiManager::Draw() const
{
	for (const UiElement* UiElement : m_ScreenVector) {
		if (UiElement != nullptr) UiElement->Draw();
	}
}
