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
		ParticleEmitter(IMesh* mesh, D3DXVECTOR3 position, float emissionRate, float lifetime);

		// Default Destructor for ParticleEmitter
		~ParticleEmitter();
	public:
		// Starts the emission of the particle
		void StartEmission();

		// Stops the emission of the particle
		void StopEmission();

		// Sets the position of the ParticleEmitter
		void SetPosition(const D3DXVECTOR3& position);

		// Called to update the ParticleEmitter
		void Update(float frameTime);
	private:
		IMesh* mMesh;
		float mLifetime;
		bool mIsEmitting;
		D3DXVECTOR3 mPosition;
		Timer mNextEmissionTimer;
		std::vector<Particle*> mParticlePool;
		std::vector<Particle*> mActiveParticles;

	};
}
