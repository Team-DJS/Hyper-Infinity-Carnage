#include "ParticleEmitter.hpp"
using namespace HIC;

// Default constructor for ParticleEmitter
ParticleEmitter::ParticleEmitter(IMesh* mesh, D3DXVECTOR3 position) :
	mMesh(mesh),
	mPosition(position)
{
}

// Returns the position of the emitter
D3DXVECTOR3 ParticleEmitter::GetPosition() const
{
	return mPosition;
}

// Sets the position of the emitter
void ParticleEmitter::SetPosition(const D3DXVECTOR3& position)
{
	mPosition = position;
}

// Starts the emission of the particle
void ParticleEmitter::StartEmission()
{
	mIsEmitting = true;
}

// Stops the emission of the particle
void ParticleEmitter::StopEmission()
{
	mIsEmitting = false;
}

// Returns true if the particle emitter is emitting particles
bool ParticleEmitter::IsEmitting() const
{
	return mIsEmitting;
}
