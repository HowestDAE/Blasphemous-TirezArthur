#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	m_SoundStream = new SoundStream{ "Sounds/buried_church_master_calm.wav" };
	m_SoundStream->Play(true);
}

SoundManager::~SoundManager()
{
	delete m_SoundStream;
	for (const std::pair<std::string, SoundEffect*>& soundEffect : m_SoundEffectMap)
	{
		if (soundEffect.second != nullptr) delete soundEffect.second;
	}
}

int SoundManager::Play(std::string path, bool loop)
{
	if (m_SoundEffectMap.find(path) != m_SoundEffectMap.end()) {
		if (m_SoundEffectMap[path] != nullptr) {
			m_SoundEffectMap[path]->SetVolume(m_EffectVolume);
			return m_SoundEffectMap[path]->Play(loop);
		}
		else return -1;
	}
	else {
		if (LoadSoundEffect(path)) return Play(path, loop);
		else return -1;
	}
}

void SoundManager::PreLoadSoundEffect(std::string path)
{
	if (m_SoundEffectMap.find(path) == m_SoundEffectMap.end()) LoadSoundEffect(path);
}

bool SoundManager::LoadSoundEffect(std::string path)
{
	SoundEffect* soundEffect{};
	soundEffect = new SoundEffect{ "Sounds/" + path + ".wav" };
	if (soundEffect->IsLoaded()) {
		m_SoundEffectMap[path] = soundEffect;
		return true;
	}
	else {
		delete soundEffect;
		m_SoundEffectMap[path] = nullptr;
		return false;
	}
}

int SoundManager::GetMaxVolume()
{
	return MIX_MAX_VOLUME;
}

void SoundManager::SetMasterVolume(int volume)
{
	Mix_MasterVolume(volume);
	m_MasterVolume = volume;
	SetMusicVolume(m_MusicVolume);
}

int SoundManager::GetMasterVolume()
{
	return Mix_MasterVolume(-1);
}

void SoundManager::SetMusicVolume(int volume)
{
	m_SoundStream->SetVolume(volume * m_MasterVolume / MIX_MAX_VOLUME);
	m_MusicVolume = volume;
}

int SoundManager::GetMusicVolume()
{
	return m_SoundStream->GetVolume();
}

void SoundManager::SetEffectVolume(int volume)
{
	m_EffectVolume = volume;
}

int& SoundManager::GetEffectVolume()
{
	return m_EffectVolume;
}

bool SoundManager::IsPlaying(std::string path, const int channel)
{
	if (m_SoundEffectMap.find(path) != m_SoundEffectMap.end() && m_SoundEffectMap[path] != nullptr) return m_SoundEffectMap[path]->IsPlaying(channel);
	return false;
}

void SoundManager::Stop(std::string path, const int channel)
{
	if (m_SoundEffectMap.find(path) != m_SoundEffectMap.end() && m_SoundEffectMap[path] != nullptr) m_SoundEffectMap[path]->Stop(channel);
}

void SoundManager::Pause(std::string path, const int channel)
{
	if (m_SoundEffectMap.find(path) != m_SoundEffectMap.end() && m_SoundEffectMap[path] != nullptr) m_SoundEffectMap[path]->Pause(channel);
}

void SoundManager::Resume(std::string path, const int channel)
{
	if (m_SoundEffectMap.find(path) != m_SoundEffectMap.end() && m_SoundEffectMap[path] != nullptr) m_SoundEffectMap[path]->Resume(channel);
}

void SoundManager::StopAll()
{
	SoundEffect::StopAll();
}

void SoundManager::PauseAll()
{
	SoundEffect::PauseAll();
}

void SoundManager::ResumeAll()
{
	SoundEffect::ResumeAll();
}
