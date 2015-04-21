#pragma once

#include "ParticleEmitter.hpp"

namespace HIC
{
	class ExplosionEmitter : public ParticleEmitter
	{
	public:
		// Default construtor for ExplosionEmitter
		ExplosionEmitter(IMesh* mesh, D3DXVECTOR3 position, float lifetime);

		// Destructor for ExplosionEmitter
		~ExplosionEmitter();
	public:
		// Updates the emitter and all its particles
		void Update(float frameTime);
	private:
		float mLifetime;
		std::vector<Particle*> mParticles;
	};
}
