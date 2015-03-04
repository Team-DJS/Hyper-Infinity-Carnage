#pragma once

#include "AudioSource.hpp"

namespace HIC
{
	class AudioManager
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		// Default constructor for AudioManager
		AudioManager();

		// Destructor for AudioManager
		~AudioManager();
	public:
		//--------------
		// Setters
		//--------------
		void SetListenerPosition(const XMFLOAT3& position);

		//---------------
		// Getters
		//---------------

		//--------------
		// Other
		//--------------
		// Loads the given audio file and tags it with the given name
		bool LoadAudio(const std::string& name, const std::string& filename);

		// Releases the specified audio file from memory
		void ReleaseAudio(const std::string& name);

		// Creates a new audio source at the given position
		AudioSource* CreateSource(const std::string& name, const XMFLOAT3& position);

		// Releases the given source from memory
		void ReleaseSource(AudioSource* source);
	private:
		std::vector<AudioSource*> mSources;
		std::unordered_map<std::string, ALuint> mBuffers;
	};

	extern AudioManager* gAudioManager;
}
