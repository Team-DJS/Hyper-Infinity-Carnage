#pragma once

#include "Common.hpp"
#include "Timer.hpp"
#include "Particle.hpp"

namespace HIC
{
	class ParticleEmitter
	{
	public:
		// Default Constructor for ParticleEmitter
		ParticleEmitter(float lifeTime);

		// Default Destructor for ParticleEmitter
		~ParticleEmitter();

	public:
		// Starts the emission of the particle
		void StartEmission();

		// Stops the emission of the particle
		void StopEmission();

		// Sets the position of the ParticleEmitter
		void SetPosition();

		// Called to update the ParticleEmitter
		void Update(float frameTime);

	private:

		bool mIsEmitting;
		XMFLOAT3 mStartingVelocity;
		Timer mLifeTimer;
		vector<Particle> mParticles;
	};
}