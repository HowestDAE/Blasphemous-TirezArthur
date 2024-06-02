#include "pch.h"
#include "InputManager.h"
#include <iostream>

InputManager::InputManager()
{
}

void InputManager::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	SDL_Keycode keyPressed{ e.keysym.sym };
	if (keyPressed == m_MoveLeft && m_MoveLeftHeld == keyState::off) m_MoveLeftHeld = keyState::pressed;
	if (keyPressed == m_MoveRight && m_MoveRightHeld == keyState::off) m_MoveRightHeld = keyState::pressed;
	if (keyPressed == m_MoveUp && m_MoveUpHeld == keyState::off) m_MoveUpHeld = keyState::pressed;
	if (keyPressed == m_MoveDown && m_MoveDownHeld == keyState::off) m_MoveDownHeld = keyState::pressed;
	if (keyPressed == m_UiLeft && m_UiLeftHeld == keyState::off) m_UiLeftHeld = keyState::pressed;
	if (keyPressed == m_UiRight && m_UiRightHeld == keyState::off) m_UiRightHeld = keyState::pressed;
	if (keyPressed == m_UiUp && m_UiUpHeld == keyState::off) m_UiUpHeld = keyState::pressed;
	if (keyPressed == m_UiDown && m_UiDownHeld == keyState::off) m_UiDownHeld = keyState::pressed;
	if (keyPressed == m_Inventory && m_InventoryHeld == keyState::off) m_InventoryHeld = keyState::pressed;
	if (keyPressed == m_Jump && m_JumpHeld == keyState::off) m_JumpHeld = keyState::pressed;
	if (keyPressed == m_Interact && m_InteractHeld == keyState::off) m_InteractHeld = keyState::pressed;
	if (keyPressed == m_Dodge && m_DodgeHeld == keyState::off) m_DodgeHeld = keyState::pressed;
	if (keyPressed == m_Flask && m_FlaskHeld == keyState::off) m_FlaskHeld = keyState::pressed;
	if (keyPressed == SDLK_ESCAPE && m_EscapeHeld == keyState::off) m_EscapeHeld = keyState::pressed;
}

void InputManager::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	SDL_Keycode keyPressed{ e.keysym.sym };
	if (keyPressed == m_MoveLeft) m_MoveLeftHeld = keyState::off;
	if (keyPressed == m_MoveRight) m_MoveRightHeld = keyState::off;
	if (keyPressed == m_MoveUp) m_MoveUpHeld = keyState::off;
	if (keyPressed == m_MoveDown) m_MoveDownHeld = keyState::off;
	if (keyPressed == m_UiLeft) m_UiLeftHeld = keyState::off;
	if (keyPressed == m_UiRight) m_UiRightHeld = keyState::off;
	if (keyPressed == m_UiUp) m_UiUpHeld = keyState::off;
	if (keyPressed == m_UiDown) m_UiDownHeld = keyState::off;
	if (keyPressed == m_Inventory) m_InventoryHeld = keyState::off;
	if (keyPressed == m_Jump) m_JumpHeld = keyState::off;
	if (keyPressed == m_Interact) m_InteractHeld = keyState::off;
	if (keyPressed == m_Dodge) m_DodgeHeld = keyState::off;
	if (keyPressed == m_Flask) m_FlaskHeld = keyState::off;
	if (keyPressed == SDLK_ESCAPE) m_EscapeHeld = keyState::off;
}

void InputManager::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		if (m_LeftMouseHeld == keyState::off) m_LeftMouseHeld = keyState::pressed;
		break;
	case SDL_BUTTON_RIGHT:
		if (m_RightMouseHeld == keyState::off) m_RightMouseHeld = keyState::pressed;
		break;
	}
}

void InputManager::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		m_LeftMouseHeld = keyState::off;
		break;
	case SDL_BUTTON_RIGHT:
		m_RightMouseHeld = keyState::off;
		break;
	}
}

bool InputManager::GetKeyState(Keybind key, const bool reset)
{
	bool state{ false };
	switch (key)
	{
	case InputManager::Keybind::moveLeft:
		state = m_MoveLeftHeld == keyState::pressed;
		if (state == true && reset) m_MoveLeftHeld = keyState::used;
		break;
	case InputManager::Keybind::moveRight:
		state = m_MoveRightHeld == keyState::pressed;
		if (state == true && reset) m_MoveRightHeld = keyState::used;
		break;
	case InputManager::Keybind::moveUp:
		state = m_MoveUpHeld == keyState::pressed;
		if (state == true && reset) m_MoveUpHeld = keyState::used;
		break;
	case InputManager::Keybind::moveDown:
		state = m_MoveDownHeld == keyState::pressed;
		if (state == true && reset) m_MoveDownHeld = keyState::used;
		break;
	case InputManager::Keybind::uiLeft:
		state = m_UiLeftHeld == keyState::pressed;
		if (state == true && reset) m_UiLeftHeld = keyState::used;
		break;
	case InputManager::Keybind::uiRight:
		state = m_UiRightHeld == keyState::pressed;
		if (state == true && reset) m_UiRightHeld = keyState::used;
		break;
	case InputManager::Keybind::uiUp:
		state = m_UiUpHeld == keyState::pressed;
		if (state == true && reset) m_UiUpHeld = keyState::used;
		break;
	case InputManager::Keybind::uiDown:
		state = m_UiDownHeld == keyState::pressed;
		if (state == true && reset) m_UiDownHeld = keyState::used;
		break;
	case InputManager::Keybind::inventory:
		state = m_InventoryHeld == keyState::pressed;
		if (state == true && reset) m_InventoryHeld = keyState::used;
		break;
	case InputManager::Keybind::jump:
		state = m_JumpHeld == keyState::pressed;
		if (state == true && reset) m_JumpHeld = keyState::used;
		break;
	case InputManager::Keybind::interact:
		state = m_InteractHeld == keyState::pressed;
		if (state == true && reset) m_InteractHeld = keyState::used;
		break;
	case InputManager::Keybind::dodge:
		state = m_DodgeHeld == keyState::pressed;
		if (state == true && reset) m_DodgeHeld = keyState::used;
		break;
	case InputManager::Keybind::block:
		state = m_RightMouseHeld == keyState::pressed;
		if (state == true && reset) m_RightMouseHeld = keyState::used;
		break;
	case InputManager::Keybind::flask:
		state = m_FlaskHeld == keyState::pressed;
		if (state == true && reset) m_FlaskHeld = keyState::used;
		break;
	case InputManager::Keybind::attack:
		state = m_LeftMouseHeld == keyState::pressed;
		if (state == true && reset) m_LeftMouseHeld = keyState::used;
		break;
	case InputManager::Keybind::escape:
		state = m_EscapeHeld == keyState::pressed;
		if (state == true && reset) m_EscapeHeld = keyState::used;
		break;
	}
	return state;
}
