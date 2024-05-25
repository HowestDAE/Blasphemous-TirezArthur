#pragma once
#include "SoundStream.h"
#include "SoundEffect.h"
#include <map>

class SoundManager final
{
public: 
	SoundManager();
	SoundManager(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	~SoundManager();

	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;

	int PlaySoundEffect(std::string path, bool loop = false);
	void PreLoadSoundEffect(std::string path);

	void SetVolume(int volume);
	int GetVolume();
	bool IsPlaying(std::string path, const int channel = -1);
	void Stop(std::string path, const int channel = -1);
	void Pause(std::string path, const int channel = -1);
	void Resume(std::string path, const int channel = -1);
	static void StopAll();
	static void PauseAll();
	static void ResumeAll();
private:
	bool LoadSoundEffect(std::string path);

	SoundStream* m_SoundStream;
	std::map<std::string, SoundEffect*> m_SoundEffectMap;
	int m_SoundVolume{ 128 };
};

