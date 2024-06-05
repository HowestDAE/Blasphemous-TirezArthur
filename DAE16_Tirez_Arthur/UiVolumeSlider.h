#pragma once
#include "UiElement.h"
#include "InputManager.h"

class SoundManager;
class TextureManager;

class UiVolumeSlider final : public UiElement
{
public:
	explicit UiVolumeSlider(float x, float y, SoundManager* soundManager, TextureManager* textureManager, InputManager* inputManager, InputManager::Keybind cycleLeft, InputManager::Keybind cycleRight, float width, float height, int steps, int volumeType, const Color4f& barColor, const Color4f& borderColor, const std::string sliderTexture);
	explicit UiVolumeSlider(Point2f pos, SoundManager* soundManager, TextureManager* textureManager, InputManager* inputManager, InputManager::Keybind cycleLeft, InputManager::Keybind cycleRight, float width, float height, int steps, int volumeType, const Color4f& barColor, const Color4f& borderColor, const std::string sliderTexture);

	virtual void Update(float elapsedSec) override;
	virtual void Draw(bool selected = false) const override;
private:
	TextureManager* m_TextureManagerPtr;
	SoundManager* m_SoundManagerPtr;
	InputManager* m_InputManagerPtr;
	const std::string m_SliderTexture;
	const float m_Width;
	const float m_Height;
	const int m_Steps;
	const int m_Type;
	const Color4f m_BarColor;
	const Color4f m_BorderColor;
	const InputManager::Keybind m_CycleLeft;
	const InputManager::Keybind m_CycleRight;
	int m_MaxVolume;
	int m_CurrentVolume;
};

