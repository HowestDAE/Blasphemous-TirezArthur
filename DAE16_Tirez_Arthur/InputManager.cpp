#include "pch.h"
#include "InputManager.h"
#include <iostream>
#include <fstream>

InputManager::InputManager()
{
	std::ifstream keybindFile{ "Saves/keybinds.csv", std::ifstream::in };
	std::string keybind{};
	for (int keyIndex{}; keyIndex < ENUMSIZE; ++keyIndex) {
		std::getline(keybindFile, keybind, ',');
		m_Keybinds[keyIndex] = std::stoi(keybind);
	}
}

InputManager::~InputManager()
{
	std::ofstream keybindFile{ "Saves/keybinds.csv", std::ios::binary };
	for (int keyIndex{}; keyIndex < ENUMSIZE; ++keyIndex) {
		keybindFile << (int)m_Keybinds[keyIndex] << ',';
	}
}

void InputManager::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	SDL_Keycode keyPressed{ e.keysym.sym };
	for (int keyIndex{}; keyIndex < ENUMSIZE; ++keyIndex) {
		if (m_Keybinds[keyIndex] == keyPressed && m_KeyStates[keyIndex] == keyState::off) m_KeyStates[keyIndex] = keyState::pressed;
	}
}

void InputManager::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	SDL_Keycode keyPressed{ e.keysym.sym };
	for (int keyIndex{}; keyIndex < ENUMSIZE; ++keyIndex) {
		if (m_Keybinds[keyIndex] == keyPressed) m_KeyStates[keyIndex] = keyState::off;
	}
}

void InputManager::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		if (m_KeyStates[(int)Keybind::attack] == keyState::off) m_KeyStates[(int)Keybind::attack] = keyState::pressed;
		break;
	case SDL_BUTTON_RIGHT:
		if (m_KeyStates[(int)Keybind::block] == keyState::off) m_KeyStates[(int)Keybind::block] = keyState::pressed;
		break;
	}
}

void InputManager::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		m_KeyStates[(int)Keybind::attack] = keyState::off;
		break;
	case SDL_BUTTON_RIGHT:
		m_KeyStates[(int)Keybind::block] = keyState::off;
		break;
	}
}

bool InputManager::GetKeyState(Keybind key, const bool reset)
{
	const bool state{ m_KeyStates[(int)key] == keyState::pressed };
	if (state && reset) m_KeyStates[(int)key] = keyState::used;return state;
}

void InputManager::ChangeKeyBind(Keybind key, SDL_KeyCode keyCode)
{
	m_Keybinds[(int)key] = keyCode;
}
