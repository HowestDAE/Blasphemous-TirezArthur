#pragma once
class InputManager
{
public:
	explicit InputManager();
	explicit InputManager(const InputManager& other) = delete;
	explicit InputManager(InputManager&& other) = delete;
	~InputManager();

	InputManager& operator=(const InputManager& other) = delete;
	InputManager& operator=(InputManager&& other) = delete;

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
		flask = 12,
		escape = 13,
		enter = 14,
		block = 15,
		attack = 16,
		saveWipe = 17
	};

	const SDL_Keycode& GetKeybind(Keybind key) const;
	bool GetKeyState(Keybind key, const bool reset = true);
	void ChangeKeybind(Keybind key, SDL_KeyCode keyCode);
	void ChangeKeybindEvent(Keybind key);
private:
	enum class keyState {
		pressed = 2,
		used = 1,
		off = 0
	};

	Keybind m_ChangingKeybind{};
	bool m_KeyChange{false};
	static const int ENUMSIZE{ 18 };
	SDL_Keycode m_Keybinds[ENUMSIZE]{ SDLK_a,SDLK_d,SDLK_w,SDLK_s,SDLK_LEFT,SDLK_RIGHT,SDLK_UP,SDLK_DOWN,SDLK_i,SDLK_SPACE,SDLK_e,SDLK_LSHIFT,SDLK_f,SDLK_ESCAPE,SDLK_KP_ENTER,SDLK_j,SDLK_k,SDLK_k };
	keyState m_KeyStates[ENUMSIZE]{ keyState::off };
};

