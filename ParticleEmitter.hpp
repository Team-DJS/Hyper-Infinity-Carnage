#pragma once

#include "Common.hpp"
#include "Particle.hpp"

namespace HIC
{
	class ParticleEmitter
	{
	public:
		// Default Constructor for ParticleEmitter
		ParticleEmitter(IMesh* mesh, D3DXVECTOR3 position);

		// Default Destructor for ParticleEmitter
		virtual ~ParticleEmitter() = default;
	public:
		// Returns the position of the emitter
		D3DXVECTOR3 GetPosition() const;

		// Sets the position of the emitter
		void SetPosition(const D3DXVECTOR3& position);

		// Starts the emission of the particle
		void StartEmission();

		// Stops the emission of the particle
		void StopEmission();

		// Returns true if the particle emitter is emitting particles
		bool IsEmitting() const;

		// Updates the emitter and all its particles
		virtual void Update(float frameTime) = 0;
	private:
		IMesh* mMesh;
		bool mIsEmitting;
		D3DXVECTOR3 mPosition;
	};
}
