#pragma once

#include "Common.hpp"

namespace HIC
{
	struct Particle
	{
		IModel* model;
		D3DXVECTOR3 velocity;
		float lifetime;
	};

	// Creates a new particle with the given mesh, position, velocity and lifetime
	extern Particle* CreateParticle(IMesh* mesh, const D3DXVECTOR3& position, const D3DXVECTOR3& velocity, float lifetime);

	// Deletes the given particle and sets it to nullptr
	extern void DeleteParticle(Particle* &particle);

	// Shows the given particle
	extern void ShowParticle(Particle* particle);

	// Hides the given particle
	extern void HideParticle(Particle* particle);

	/*
	class Particle
	{
	public:
	// Default constructor for Particle
	Particle(IMesh* mesh, float lifetime);

	// Destructor for Particle
	~Particle();
	public:
	// Returns true if the lifetime of the particle is over
	bool IsLifetimeOver() const;

	// Set the position of the Particle
	void SetPosition(const D3DXVECTOR3& pos);

	// Set velocity of particle
	void SetVelocity(const D3DXVECTOR3& vel);

	// Hide or Show the particle
	void SetVisibility(bool on);

	// Set the lifetime of the particle
	void SetLifetime(float time);

	// Called to update the position of the particle
	void Update(float frameTime);
	private:
	IModel* mModel;
	float mLifetime;
	D3DXVECTOR3 mVelocity;
	};
	*/
}