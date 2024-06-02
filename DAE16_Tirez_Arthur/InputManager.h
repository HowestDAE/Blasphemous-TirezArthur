#pragma once
class InputManager
{
public:
	InputManager();

	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

	enum class Keybind {
		moveLeft = 0,
		moveRight = 1,
		moveUp = 2,
		moveDown = 3,
		uiLeft = 4,
		uiRight = 5,
		uiUp = 6,
		uiDown = 7,
		inventory = 8,
		jump = 9,
		interact = 10,
		dodge = 11,
		block = 12,
		flask = 13,
		attack = 14,
		escape = 15
	};

	bool GetKeyState(Keybind key, const bool reset = true);
private:
	enum class keyState {
		pressed = 2,
		used = 1,
		off = 0
	};

	SDL_Keycode m_MoveLeft{ SDLK_a };
	SDL_Keycode m_MoveRight{ SDLK_d };
	SDL_Keycode m_MoveUp{ SDLK_w };
	SDL_Keycode m_MoveDown{ SDLK_s };
	SDL_Keycode m_UiLeft{ SDLK_LEFT };
	SDL_Keycode m_UiRight{ SDLK_RIGHT };
	SDL_Keycode m_UiUp{ SDLK_UP };
	SDL_Keycode m_UiDown{ SDLK_DOWN };
	SDL_Keycode m_Inventory{ SDLK_i };
	SDL_Keycode m_Jump{ SDLK_SPACE };
	SDL_Keycode m_Interact{ SDLK_e };
	SDL_Keycode m_Dodge{ SDLK_LSHIFT };
	SDL_Keycode m_Flask{ SDLK_f };

	keyState m_MoveLeftHeld{};
	keyState m_MoveRightHeld{};
	keyState m_MoveUpHeld{};
	keyState m_MoveDownHeld{};
	keyState m_UiLeftHeld{};
	keyState m_UiRightHeld{};
	keyState m_UiUpHeld{};
	keyState m_UiDownHeld{};
	keyState m_InventoryHeld{};
	keyState m_JumpHeld{};
	keyState m_InteractHeld{};
	keyState m_DodgeHeld{};
	keyState m_FlaskHeld{};
	keyState m_EscapeHeld{};

	keyState m_LeftMouseHeld{};
	keyState m_RightMouseHeld{};
};

