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
		void SetPosition(const D3DXVECTOR3& position);

		// Sets the volume of the source
		void SetVolume(float volume);

		// Sets the looping state of the audio
		void SetLooping(bool loop);

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
