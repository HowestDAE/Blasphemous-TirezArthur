#pragma once
#include <string>
#include <vector>
class Mix_Chunk;
class SoundEffect final
{
public:
	explicit SoundEffect( const std::string& path );
	~SoundEffect( );
	SoundEffect(const SoundEffect& other) = delete;
	SoundEffect& operator=(const SoundEffect& rhs) = delete;
	SoundEffect( SoundEffect&& other) = delete;
	SoundEffect& operator=( SoundEffect&& rhs) = delete;

	bool IsLoaded( ) const;
	int Play( const int loops );
	void SetVolume( const int value ); 
	int GetVolume( ) const;
	bool IsPlaying( const int channel = -1 );
	void Stop( const int channel = -1 );
	void Pause( const int channel = -1 );
	void Resume( const int channel = -1 );
	static void StopAll( );
	static void PauseAll( );
	static void ResumeAll( );

private:
	void UpdateKnownChannels();
	std::vector<int> m_Channels;
	Mix_Chunk* m_pMixChunk;
};
