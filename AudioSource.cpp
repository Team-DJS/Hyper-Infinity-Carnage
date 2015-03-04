#include "AudioSource.hpp"
using namespace HIC;

//-----------------------------
// Constructors/Destructors
//-----------------------------
// Default constructor for AudioSource
AudioSource::AudioSource(ALuint buffer)
{
	alGenSources(1, &mID);
	alSourcei(mID, AL_BUFFER, buffer);
}

// Destructor for AudioSource
AudioSource::~AudioSource()
{
	alDeleteSources(1, &mID);
}

//--------------
// Setters
//--------------
// Sets the position of the audio in the scene
void AudioSource::SetPosition(const XMFLOAT3& position)
{
	ALfloat pos[] = { position.x, position.y, position.z };
	alSourcefv(mID, AL_POSITION, pos);
}

// Sets the volume of the source
void AudioSource::SetVolume(float volume)
{
	if (volume >= 0.0f && volume <= 1.0f)
	{
		alSourcef(mID, AL_GAIN, volume);
	}
}

//---------------
// Getters
//---------------

//--------------
// Other
//--------------
// Plays the audio from the previous position
void AudioSource::Play()
{
	alSourcePlay(mID);
}

// Pauses the audio at the current position
void AudioSource::Pause()
{
	alSourcePause(mID);
}

// Stop the audio, and reset it back to the start
void AudioSource::Stop()
{
	alSourceStop(mID);
}
