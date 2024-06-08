#include "pch.h"
#include "UiVolumeSlider.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "utils.h"

UiVolumeSlider::UiVolumeSlider(float x, float y, SoundManager* soundManager, TextureManager* textureManager, InputManager* inputManager, InputManager::Keybind cycleLeft, InputManager::Keybind cycleRight, float width, float height, int steps, int volumeType, const Color4f& barColor, const Color4f& borderColor, const std::string sliderTexture) :
	UiVolumeSlider{ Point2f{x,y},soundManager,textureManager,inputManager,cycleLeft,cycleRight,width,height,steps,volumeType,barColor,borderColor,sliderTexture }
{
}

UiVolumeSlider::UiVolumeSlider(Point2f pos, SoundManager* soundManager, TextureManager* textureManager, InputManager* inputManager, InputManager::Keybind cycleLeft, InputManager::Keybind cycleRight, float width, float height, int steps, int volumeType, const Color4f& barColor, const Color4f& borderColor, const std::string sliderTexture):
	UiElement{pos},
	m_Height{height},
	m_Width{width},
	m_BarColor{barColor},
	m_BorderColor{borderColor},
	m_SliderTexture{sliderTexture},
	m_TextureManagerPtr{textureManager},
	m_SoundManagerPtr{soundManager},
	m_InputManagerPtr{inputManager},
	m_CycleLeft{cycleLeft},
	m_CycleRight{cycleRight},
	m_Steps{steps},
	m_Type{volumeType}
{
	m_MaxVolume = m_SoundManagerPtr->GetMaxVolume();
	switch (m_Type)
	{
	case 1:
		m_CurrentVolume = m_SoundManagerPtr->GetEffectVolume();
		break;
	case 2:
		m_CurrentVolume = m_SoundManagerPtr->GetMusicVolume();
		break;
	case 0:
	default:
		m_CurrentVolume = m_SoundManagerPtr->GetMasterVolume();
		break;
	}
}

void UiVolumeSlider::Update(float elapsedSec, bool selected)
{
	if (!selected) return;
	if (m_InputManagerPtr->GetKeyState(m_CycleLeft)) {
		m_CurrentVolume = std::max(m_CurrentVolume - m_MaxVolume / m_Steps, 0);
		m_SoundManagerPtr->Play("button_click");
		switch (m_Type)
		{
		case 1:
			m_SoundManagerPtr->SetEffectVolume(m_CurrentVolume);
			break;
		case 2:
			m_SoundManagerPtr->SetMusicVolume(m_CurrentVolume);
			break;
		case 0:
		default:
			m_SoundManagerPtr->SetMasterVolume(m_CurrentVolume);
			break;
		}
	}
	if (m_InputManagerPtr->GetKeyState(m_CycleRight)) {
		m_CurrentVolume = std::min(m_CurrentVolume + m_MaxVolume / m_Steps, m_MaxVolume);
		m_SoundManagerPtr->Play("button_click");
		switch (m_Type)
		{
		case 1:
			m_SoundManagerPtr->SetEffectVolume(m_CurrentVolume);
			break;
		case 2:
			m_SoundManagerPtr->SetMusicVolume(m_CurrentVolume);
			break;
		case 0:
		default:
			m_SoundManagerPtr->SetMasterVolume(m_CurrentVolume);
			break;
		}
	}
}

void UiVolumeSlider::Draw(bool selected) const
{
	utils::SetColor(m_BorderColor);
	utils::FillRect(m_Position, m_Width, m_Height);
	utils::SetColor(m_BarColor);
	const float progress{ (float)m_CurrentVolume / (float)m_MaxVolume };
	utils::FillRect(m_Position.x + 1.0f, m_Position.y + 1.0f, m_Width * progress - 2.0f, m_Height - 2.0f);
	m_TextureManagerPtr->Draw(m_SliderTexture, m_Position.x + m_Width * progress - m_TextureManagerPtr->GetTextureWidth(m_SliderTexture) * 0.5f, m_Position.y + m_Height * 0.5f - m_TextureManagerPtr->GetTextureHeight(m_SliderTexture) * 0.5f, false);
}
