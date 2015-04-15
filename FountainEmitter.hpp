#pragma once

#include "ParticleEmitter.hpp"
#include "Timer.hpp"

namespace HIC
{
	class FountainEmitter : public ParticleEmitter
	{
	public:
		// Default constructor for FountainEmitter
		FountainEmitter(IMesh* mesh, D3DXVECTOR3 position, float emissionRate, float lifetime);

		// Destructor for FountainEmitter
		~FountainEmitter();
	public:
		// Updates the emitter and all its particles
		void Update(float frameTime);
	private:
		float mLifetime;
		Timer mNextEmissionTimer;
		std::vector<Particle*> mParticlePool;
		std::vector<Particle*> mActiveParticles;
	};
}
