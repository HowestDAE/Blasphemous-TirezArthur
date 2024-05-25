#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	m_SoundStream = new SoundStream{ "Sounds/buried_church_master_calm.wav" };
	m_SoundStream->Play(true);
	m_SoundStream->SetVolume(m_SoundVolume / 2);
}

SoundManager::~SoundManager()
{
	delete m_SoundStream;
	for (const std::pair<std::string, SoundEffect*>& soundEffect : m_SoundEffectMap)
	{
		if (soundEffect.second != nullptr) delete soundEffect.second;
	}
}

int SoundManager::PlaySoundEffect(std::string path, bool loop)
{
	if (m_SoundEffectMap.find(path) != m_SoundEffectMap.end()) {
		if (m_SoundEffectMap[path] != nullptr) {
			m_SoundEffectMap[path]->SetVolume(m_SoundVolume);
			return m_SoundEffectMap[path]->Play(loop);
		}
	}
	else {
		if (LoadSoundEffect(path)) return PlaySoundEffect(path, loop);
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

void SoundManager::SetVolume(int volume)
{
	m_SoundVolume = volume;
	m_SoundStream->SetVolume(m_SoundVolume);
}

int SoundManager::GetVolume()
{
	return m_SoundVolume;
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
