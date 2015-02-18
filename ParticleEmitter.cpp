#include "ParticleEmitter.hpp"
using namespace HIC;

// Default constructor for ParticleEmitter
ParticleEmitter::ParticleEmitter(IMesh* mesh, float emissionRate) :
	mMesh(mesh),
	mNextEmissionTimer(emissionRate)
{
}

// Default Destructor for ParticleEmitter
ParticleEmitter::~ParticleEmitter()
{
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

// Sets the position of the particle
void ParticleEmitter::SetPosition(const XMFLOAT3& position)
{
	mPosition = position;
}

// Called to update the ParticleEmitter
void ParticleEmitter::Update(float frameTime)
{
	// Update the timer
	mNextEmissionTimer.Update(frameTime);

	// Check if a new particle should be emitted
	if (mIsEmitting && mNextEmissionTimer.IsComplete())
	{
		// TODO: Generate random velocity
		XMFLOAT3 velocity(1.0f, 1.0f, 1.0f);

		mParticles.emplace_back(mMesh, mPosition, velocity);

		// Reset the timer
		mNextEmissionTimer.Reset();
	}

	// Update all the particles
	size_t numParticles = mParticles.size();
	for (size_t i = 0; i < numParticles; ++i)
	{
		mParticles[i].Update(frameTime);
	}
}
