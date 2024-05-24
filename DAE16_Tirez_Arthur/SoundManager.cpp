#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	m_SoundStream = new SoundStream{ "Sounds/buried_church_master.wav" };
	m_SoundStream->Play(true);
	m_SoundStream->GetVolume();
}

SoundManager::~SoundManager()
{
	delete m_SoundStream;
	for (const std::pair<std::string, SoundEffect*>& soundEffect : m_SoundEffectMap)
	{
		delete soundEffect.second;
	}
}

void SoundManager::PlaySoundEffect(std::string path, bool loop)
{
	if (m_SoundEffectMap.find(path) != m_SoundEffectMap.end()) {
		if (m_SoundEffectMap[path] != nullptr) {
			m_SoundEffectMap[path]->Play(loop);
			m_SoundEffectMap[path]->SetVolume(m_SoundVolume);
		}
	}
	else {
		LoadSoundEffect(path);
	}
}

void SoundManager::PreLoadSoundEffect(std::string path)
{
	if (m_SoundEffectMap.find(path) == m_SoundEffectMap.end()) LoadSoundEffect(path);
}

void SoundManager::LoadSoundEffect(std::string path)
{
	SoundEffect* soundEffect{};
	soundEffect = new SoundEffect{ "Sounds/" + path + ".wav" };
	if (soundEffect->IsLoaded()) {
		m_SoundEffectMap[path] = soundEffect;
	}
	else {
		delete soundEffect;
		m_SoundEffectMap[path] = nullptr;
	}
}

void SoundManager::SetVolume(int volume)
{
	m_SoundVolume = volume;
	m_SoundStream->SetVolume(m_SoundVolume);
}

int SoundManager::GetVolume()
{
	return m_SoundVolume;
}
