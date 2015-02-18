#pragma once

#include "Common.hpp"

class ParticleEmitter
{
public:
	// Default Constructor for ParticleEmitter
	ParticleEmitter();

	// Default Destructor for ParticleEmitter
	~ParticleEmitter();

public:
	// Starts the emission of the particle
	void StartEmission();

	// Stops the emission of the partlice
	void StopEmission();

	// Sets the position of the particle
	void SetPosition();

	// Called to update the ParticleEmitter
	void Update(float frameTime);

private:

	bool mIsEmitting;
	XMFLOAT3 mStartingVelocity;
};

