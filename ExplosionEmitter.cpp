#include "ExplosionEmitter.hpp"
using namespace HIC;

// Default construtor for ExplosionEmitter
ExplosionEmitter::ExplosionEmitter(IMesh* mesh, D3DXVECTOR3 position, float lifetime, int numParticles) :
	ParticleEmitter(mesh, position),
	mLifetime(lifetime)
{
	for (auto i = 0; i < numParticles; ++i)
	{
		Particle* particle = CreateParticle(mesh, position, { Random(-25.0f, 25.0f), Random(0.0f, 50.0f), Random(-25.0f, 25.0f) }, lifetime);
		mParticles.push_back(particle);
	}
}

// Destructor for ExplosionEmitter
ExplosionEmitter::~ExplosionEmitter()
{
	for (auto particle : mParticles)
	{
		DeleteParticle(particle);
	}
}

// Updates the emitter and all its particles
void ExplosionEmitter::Update(float frameTime)
{
	if (IsEmitting())
	{
		mLifetime -= frameTime;

		// Update all the particles
		for (auto particle : mParticles)
		{
			auto velocity = particle->velocity;
			particle->model->Move(velocity.x * frameTime, velocity.y * frameTime, velocity.z * frameTime);
			particle->model->LookAt(gCamera);
		}

		// Stop emission
		if (mLifetime <= 0.0f)
		{
			StopEmission();
		}
	}
}
