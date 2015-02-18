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
		ParticleEmitter(IMesh* mesh, float emissionRate);

		// Default Destructor for ParticleEmitter
		~ParticleEmitter();
	public:
		// Starts the emission of the particle
		void StartEmission();

		// Stops the emission of the particle
		void StopEmission();

		// Sets the position of the ParticleEmitter
		void SetPosition(const XMFLOAT3& position);

		// Called to update the ParticleEmitter
		void Update(float frameTime);
	private:
		IMesh* mMesh;
		bool mIsEmitting;
		XMFLOAT3 mPosition;
		Timer mNextEmissionTimer;
		vector<Particle> mParticles;
	};
}
