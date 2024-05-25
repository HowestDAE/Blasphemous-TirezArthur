#include "base.h"
#include <iostream>
#include "SoundEffect.h"

SoundEffect::SoundEffect( const std::string& path )
	:m_pMixChunk{ Mix_LoadWAV( path.c_str( ) ) }
{
	if ( m_pMixChunk == nullptr )
	{
		const std::string errorMsg = "SoundEffect: Failed to load " + path + ",\nSDL_mixer Error: " + Mix_GetError( );
		std::cerr << errorMsg;
	}
}
SoundEffect::~SoundEffect( )
{
	Mix_FreeChunk( m_pMixChunk );
	m_pMixChunk = nullptr;
}
/*
\returns wether or not the sound loaded succesfully
*/
bool SoundEffect::IsLoaded( ) const
{
	return m_pMixChunk != nullptr;
}
/*
Plays this sound effect in the first free channel
\param loop the number of times the chunk should loop, -1 to loop infinitely.
\returns the channel in which the chunk started playing. -1 if no channel was free
*/
int SoundEffect::Play( const int loops )
{
	if ( m_pMixChunk != nullptr )
	{
		const int channel{ Mix_PlayChannel( -1, m_pMixChunk, loops ) };
		if (channel != -1) {
			m_Channels.push_back(channel);
			return channel;
		}
	}
	return -1;
}

void SoundEffect::SetVolume( const int value )
{
	if ( m_pMixChunk != nullptr )
	{
		Mix_VolumeChunk( m_pMixChunk, value );
	}
}

int SoundEffect::GetVolume( ) const
{
	if ( m_pMixChunk != nullptr )
	{
		return Mix_VolumeChunk( m_pMixChunk, -1 );
	}
	else
	{
		return -1;
	}
}
/*
Check if this sound effect is playing
\param channel optional argument to check if this sound effect is playing in a specific channel.
\return true if playing in the specified channel or in any channel at all if channel is -1 ,
false if not playing in the specified channel or in any channel at all if channel is -1*/
bool SoundEffect::IsPlaying(const int channel)
{
	UpdateKnownChannels();
	if (channel == -1) return m_Channels.size() == 0 ? false : true;
	for (int currentChannel : m_Channels) {
		if (currentChannel == channel) return true;
	}
	return false;
}
/*
Stop all instances of this sound effect from playing
\param channel optional argument to stop only in one specific channel.
*/
void SoundEffect::Stop(const int channel)
{
	UpdateKnownChannels();
	for (int channelIndex{}; channelIndex < m_Channels.size(); ++channelIndex) {
		if (channel != -1 && m_Channels.at(channelIndex) != channel) continue;
		Mix_HaltChannel(m_Channels.at(channelIndex));
		m_Channels.erase(m_Channels.begin() + channelIndex);
	}
}
/*
Pause all instances of this sound effect from playing
\param channel optional argument to pause only in one specific channel.
*/
void SoundEffect::Pause(const int channel)
{
	UpdateKnownChannels();
	for (int currentChannel : m_Channels) {
		if (channel != -1 && currentChannel != channel) continue;
		Mix_Pause(currentChannel);
	}
}
/*
Resume all instances of this sound effect from playing
\param channel optional argument to resume only in one specific channel.
*/
void SoundEffect::Resume(const int channel)
{
	UpdateKnownChannels();
	for (int currentChannel : m_Channels) {
		if (channel != -1 && currentChannel != channel) continue;
		Mix_Resume(currentChannel);
	}
}
/*
Stop every single sound effect
*/
void SoundEffect::StopAll( )
{
	Mix_HaltChannel(-1 );
}
/*
Pause every single sound effect
*/
void SoundEffect::PauseAll( )
{
	Mix_Pause( -1 );
}
/*
Resume every single sound effect
*/
void SoundEffect::ResumeAll( )
{
	Mix_Resume( -1 );
}

void SoundEffect::UpdateKnownChannels()
{
	for (int channelIndex{}; channelIndex < m_Channels.size(); ++channelIndex) {
		if (Mix_GetChunk(m_Channels.at(channelIndex)) != m_pMixChunk || 
			(Mix_Playing(m_Channels.at(channelIndex)) == 0 && Mix_Paused(m_Channels.at(channelIndex)) == 0))
			m_Channels.erase(m_Channels.begin() + channelIndex);
	}
}




