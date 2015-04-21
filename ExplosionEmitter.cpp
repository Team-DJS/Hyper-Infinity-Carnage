#include "ExplosionEmitter.hpp"
using namespace HIC;

// Default construtor for ExplosionEmitter
ExplosionEmitter::ExplosionEmitter(IMesh* mesh, D3DXVECTOR3 position, float lifetime) :
	ParticleEmitter(mesh, position),
	mLifetime(lifetime)
{
	for (auto i = 0; i < 8; ++i)
	{
		Particle* particle = new Particle{ mesh, lifetime };
		particle->SetPosition(position);
		particle->SetVelocity({ Random(-50.0f, 50.0f), Random(0.0f, 100.0f), Random(-50.0f, 50.0f) });

		mParticles.push_back(particle);
	}
}

// Destructor for ExplosionEmitter
ExplosionEmitter::~ExplosionEmitter()
{
	for (auto particle : mParticles)
	{
		delete(particle);
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
			particle->Update(frameTime);
		}

		// Stop emission
		if (mLifetime <= 0.0f)
		{
			StopEmission();
		}
	}
}
