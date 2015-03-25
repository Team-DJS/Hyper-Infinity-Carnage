#include "ParticleEmitter.hpp"
using namespace HIC;

// Default constructor for ParticleEmitter
ParticleEmitter::ParticleEmitter(IMesh* mesh, XMFLOAT3 position, float emissionRate, float lifetime) :
mMesh(mesh),
mNextEmissionTimer(emissionRate)
{
	for (uint32_t i = 0; i < 200; i++)
	{
		Particle* particle = new Particle(mesh, position, XMFLOAT3(0,0,0), lifetime);
		mParticlePool.push_back(particle);
	}
}

// Default Destructor for ParticleEmitter
ParticleEmitter::~ParticleEmitter()
{
	for (auto particle : mParticlePool)
	{
		delete(particle);
	}

	for (auto particle : mActiveParticles)
	{
		delete(particle);
	}
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
	if (mIsEmitting && mNextEmissionTimer.IsComplete() && !mParticlePool.empty())
	{
		Particle* particle = mParticlePool.back();
		mParticlePool.pop_back();

		if (particle != nullptr)
		{
			particle->SetLifetime(mLifetime);
			particle->SetPosition(mPosition);
			XMFLOAT3 velocity(Random(10.0f, 15.0f), 10.0f, Random(10.0f, 15.0f));
			particle->SetVelocity(velocity);
			mActiveParticles.push_back(particle);
		}

		// Reset the timer
		mNextEmissionTimer.Reset();
	}

	// Update all the particles
	for (uint32_t i = 0; i < mActiveParticles.size(); i++)
	{
		mActiveParticles[i]->Update(frameTime);

		if (mActiveParticles[i]->IsLifetimeOver())
		{
			mParticlePool.push_back(mActiveParticles[i]);
			mActiveParticles.erase(mActiveParticles.begin() + i);
			i--;
		}
	}
}
