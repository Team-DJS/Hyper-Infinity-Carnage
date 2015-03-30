#include "ParticleEmitter.hpp"
using namespace HIC;

// Default constructor for ParticleEmitter
ParticleEmitter::ParticleEmitter(IMesh* mesh, D3DXVECTOR3 position, float emissionRate, float lifetime) :
	mMesh(mesh),
	mPosition(position),
	mLifetime(lifetime),
	mNextEmissionTimer(emissionRate)
{
	for (uint32_t i = 0; i < 200; i++)
	{
		Particle* particle = new Particle(mesh, position, position, lifetime);
		particle->SetVisibility(false);
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
void ParticleEmitter::SetPosition(const D3DXVECTOR3& position)
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

		particle->SetLifetime(mLifetime);
		particle->SetPosition(mPosition);
		D3DXVECTOR3 velocity(Random(-50.0f, 50.0f), 10.0f, Random(-50.0f, 50.0f));
		particle->SetVelocity(velocity);
		particle->SetVisibility(true);
		mActiveParticles.push_back(particle);

		// Reset the timer
		mNextEmissionTimer.Reset();
	}

	// Update all active particles
	for (auto iter = mActiveParticles.begin(); iter != mActiveParticles.end();)
	{
		Particle* particle = (*iter);
		particle->Update(frameTime);

		// Reset any completed particles
		if (particle->IsLifetimeOver())
		{
			particle->SetVisibility(false);
			mParticlePool.push_back(particle);
			iter = mActiveParticles.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}
