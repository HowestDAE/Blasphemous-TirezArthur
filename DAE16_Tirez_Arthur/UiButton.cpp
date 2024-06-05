#include "pch.h"
#include "UiButton.h"
#include "InputManager.h"
#include "UiManager.h"
#include "SoundManager.h"

UiButton::UiButton(InputManager* inputManager, UiManager* uiManager, SoundManager* soundManager, int functionId, const std::string& functionData) :
	UiElement{0.0f, 0.0f},
	m_InputManagerPtr{inputManager},
	m_UiManagerPtr{uiManager},
	m_FunctionData{functionData},
	m_FunctionId{functionId},
	m_SoundManagerPtr{soundManager}
{
}

void UiButton::Update(float elapsedSec)
{
	if (m_InputManagerPtr->GetKeyState(InputManager::Keybind::enter)) {
		m_UiManagerPtr->UiButtonFunction(m_FunctionId, m_FunctionData);
		m_SoundManagerPtr->Play("button_click");
	}
}

void UiButton::Draw(bool selected) const
{
}
