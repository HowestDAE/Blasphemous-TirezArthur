#include "pch.h"
#include "SaveManager.h"
#include "InputManager.h"
#include "UiManager.h"
#include "UiStaticGraphic.h"
#include "UiStaticText.h"
#include "UiSave.h"

UiSave::UiSave(const Point2f& pos, int saveIndex, SaveManager* saveManager, TextureManager* textureManager, InputManager* inputManager, UiManager* uiManager) :
	UiElement{ pos },
	m_SaveManagerPtr{ saveManager },
	m_SaveIndex{ saveIndex },
	m_TextureManagerPtr{ textureManager },
	m_InputManagerPtr{ inputManager },
	m_UiManagerPtr{ uiManager }
{
	UpdateInfo();
}

UiSave::~UiSave()
{
	if (m_SaveBorder != nullptr) delete m_SaveBorder;
	if (m_RegionDisplay != nullptr) delete m_RegionDisplay;
	if (m_TearsDisplay != nullptr) delete m_TearsDisplay;
}

void UiSave::Update(float elapsedSec, bool selected)
{
	if (selected) {
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::enter)) {
			m_SaveManagerPtr->LoadSave(m_SaveIndex);
			m_UiManagerPtr->LoadScreen("game");
		}
		if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::saveWipe)) {
			m_SaveManagerPtr->DeleteSave(m_SaveIndex);
			m_UiManagerPtr->LoadScreen("delete");
		}
	}
	UpdateInfo();
}

void UiSave::Draw(bool selected) const
{
	m_SaveBorder->Draw(selected);
	if (m_SaveExists) {
		m_RegionDisplay->Draw(selected);
		m_TearsDisplay->Draw(selected);
	}
}

void UiSave::UpdateInfo()
{
	const std::string PrevSaveRegion{ m_SaveRegion };
	const bool PrevSaveExists{ m_SaveExists };
	const int PrevTears{ m_Tears };
	m_SaveExists = m_SaveManagerPtr->PeekSave(m_SaveIndex, m_SaveRegion, m_Tears);
	if ((PrevSaveRegion != m_SaveRegion || m_RegionDisplay == nullptr) && m_SaveExists) {
		if (m_RegionDisplay != nullptr) delete m_RegionDisplay;
		m_RegionDisplay = new UiStaticText{ m_Position.x + 43.0f, m_Position.y + 26.0f, m_TextureManagerPtr, Color4f{0.525f, 0.463f, 0.208f, 1.0f}, Color4f{0.996f, 0.827f, 0.067f, 1.0f}, "save_display_region_" + std::to_string(m_SaveIndex), m_SaveRegion, UiStaticText::Alignment::bottomleft };
	}
	if ((PrevTears != m_Tears || m_TearsDisplay == nullptr) && m_SaveExists) {
		if (m_TearsDisplay != nullptr) delete m_TearsDisplay;
		m_TearsDisplay = new UiStaticText{ m_Position.x + 427.0f , m_Position.y + 33.0f , m_TextureManagerPtr, Color4f{0.525f, 0.463f, 0.208f, 1.0f}, Color4f{0.996f, 0.827f, 0.067f, 1.0f}, "save_display_tears_" + std::to_string(m_SaveIndex), std::to_string(m_Tears), UiStaticText::Alignment::right };
	}
	if (PrevSaveExists != m_SaveExists || m_SaveBorder == nullptr){
		if (m_SaveBorder != nullptr) delete m_SaveBorder;
		if (m_SaveExists) m_SaveBorder = new UiStaticGraphic{ m_Position, "save_display", m_TextureManagerPtr };
		else m_SaveBorder = new UiStaticGraphic{ m_Position, "save_display_empty", m_TextureManagerPtr };
	}
}
