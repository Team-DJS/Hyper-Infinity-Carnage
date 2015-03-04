#pragma once

#include "Common.hpp"

namespace HIC
{
	class AudioSource
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		// Default constructor for AudioSource
		AudioSource(ALuint buffer);

		// Destructor for AudioSource
		~AudioSource();
	public:
		//--------------
		// Setters
		//--------------
		// Sets the position of the audio in the scene
		void SetPosition(const XMFLOAT3& position);

		// Sets the volume of the source
		void SetVolume(float volume);

		//---------------
		// Getters
		//---------------

		//--------------
		// Other
		//--------------
		// Plays the audio from the previous position
		void Play();

		// Pauses the audio at the current position
		void Pause();

		// Stop the audio, and reset it back to the start
		void Stop();
	private:
		ALuint mID;
	};
}
