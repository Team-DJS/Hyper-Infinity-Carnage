#include "AudioManager.hpp"
using namespace HIC;

AudioManager* HIC::gAudioManager = nullptr;

//-----------------------------
// Constructors/Destructors
//-----------------------------
// Default constructor for AudioManager
AudioManager::AudioManager()
{
	// Initialise ALUT
	if (!alutInit(nullptr, nullptr))
	{
		throw std::runtime_error("Failed to initialise ALUT");
	}

	ALfloat position[] = { 0.0f, 0.0f, 0.0f };
	alListenerfv(AL_POSITION, position);

	ALfloat velocity[] = { 0.0f, 0.0f, 0.0f };
	alListenerfv(AL_VELOCITY, velocity);

	ALfloat orientation[] = { 0.0f, 0.0f, -1.0f };
	alListenerfv(AL_ORIENTATION, orientation);

	alListenerf(AL_GAIN, 1.0f);
}

// Destructor for AudioManager
AudioManager::~AudioManager()
{
	// Exit ALUT
	alutExit();
}

//--------------
// Setters
//--------------
void AudioManager::SetListenerPosition(const D3DXVECTOR3& position)
{
	ALfloat pos[] = { position.x, position.y, position.z };
	alListenerfv(AL_POSITION, pos);
}

//---------------
// Getters
//---------------

//--------------
// Other
//--------------
// Loads the given audio file and tags it with the given name
bool AudioManager::LoadAudio(const std::string& name, const std::string& filename)
{
	ALuint buffer = alutCreateBufferFromFile(filename.c_str());
	if (buffer == AL_NONE)
	{
		return false;
	}
	mBuffers.emplace(name, buffer);
	return true;
}

// Releases the specified audio file from memory
void AudioManager::ReleaseAudio(const std::string& name)
{
	ALuint buffer = mBuffers[name];
	alDeleteBuffers(1, &buffer);
}

// Creates a new audio source at the given position
AudioSource* AudioManager::CreateSource(const std::string& name, const D3DXVECTOR3& position)
{
	AudioSource* source = nullptr;

	auto iter = mBuffers.find(name);
	if (iter != mBuffers.end())
	{
		source = new AudioSource(iter->second);
		source->SetPosition(position);
		mSources.push_back(source);
	}
	return source;
}

// Releases the given source from memory
void AudioManager::ReleaseSource(AudioSource* source)
{
	for (auto iter = mSources.begin(); iter != mSources.end(); ++iter)
	{
		if ((*iter) == source)
		{
			mSources.erase(iter);
			break;
		}
	}
	SafeRelease(source);
}
