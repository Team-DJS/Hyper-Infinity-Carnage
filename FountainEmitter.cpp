#include "FountainEmitter.hpp"
using namespace HIC;

// Default constructor for FountainEmitter
FountainEmitter::FountainEmitter(IMesh* mesh, D3DXVECTOR3 position, float emissionRate, float lifetime) :
	ParticleEmitter(mesh, position),
	mLifetime(lifetime),
	mNextEmissionTimer(emissionRate)
{
	// Calculate the maximum number of particles which can spawn
	size_t maxSpawn = static_cast<size_t>((1.0f / emissionRate) * lifetime);

	// Reserve the space in the particle storage for maximum efficiency
	mParticlePool.reserve(maxSpawn);
	mActiveParticles.reserve(maxSpawn);

	// Generate all the particles
	for (uint32_t i = 0; i < maxSpawn; i++)
	{
		Particle* particle = CreateParticle(mesh, position, {}, lifetime);
		HideParticle(particle);
		mParticlePool.push_back(particle);
	}
}

// Destructor for FountainEmitter
FountainEmitter::~FountainEmitter()
{
	// Delete all the particles in the particle pool
	for (auto particle : mParticlePool)
	{
		DeleteParticle(particle);
	}

	// Delete all the active particles
	for (auto particle : mActiveParticles)
	{
		DeleteParticle(particle);
	}
}

// Updates the emitter and all its particles
void FountainEmitter::Update(float frameTime)
{
	// Update the timer
	mNextEmissionTimer.Update(frameTime);

	// Check if a new particle should be emitted
	if (IsEmitting() && mNextEmissionTimer.IsComplete() && !mParticlePool.empty())
	{
		// Setup the next particle available from the particle pool
		Particle* particle = mParticlePool.back();
		mParticlePool.pop_back();

		// Generate a random velocity for the particle
		D3DXVECTOR3 velocity;
		velocity.x = Random(-40.0f, 40.0f);
		velocity.y = 80.0f;
		velocity.z = Random(-40.0f, 40.0f);

		auto position = GetPosition();
		particle->model->SetPosition(position.x, position.y, position.z);
		particle->lifetime = mLifetime;
		particle->velocity = velocity;
		ShowParticle(particle);

		// Activate the particle
		mActiveParticles.push_back(particle);

		// Reset the timer
		mNextEmissionTimer.Reset();
	}

	// Update all the active particles
	for (auto iter = mActiveParticles.begin(); iter != mActiveParticles.end();)
	{
		Particle* particle = (*iter);

		// Update the particle
		particle->lifetime -= frameTime;

		auto velocity = particle->velocity;
		particle->model->Move(velocity.x * frameTime, velocity.y * frameTime, velocity.z * frameTime);
		particle->model->LookAt(gCamera);

		// Reset any completed particles
		if (particle->lifetime <= 0.0f)
		{
			HideParticle(particle);
			mParticlePool.push_back(particle);
			iter = mActiveParticles.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}
