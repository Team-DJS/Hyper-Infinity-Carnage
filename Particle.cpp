#include "Particle.hpp"
using namespace HIC;

// Default constructor for Particle
Particle::Particle(IMesh* mesh, float lifetime) :
	mModel(mesh->CreateModel()),
	mLifetime(lifetime)
{
}

// Destructor for Particle
Particle::~Particle()
{
	mModel->GetMesh()->RemoveModel(mModel);
}

// Returns true if the lifetime of the particle is over
bool Particle::IsLifetimeOver() const
{
	return mLifetime <= 0.0f;
}

// Set position of the particle
void Particle::SetPosition(const D3DXVECTOR3& pos)
{
	mModel->SetPosition(pos.x, pos.y, pos.z);
}

// Set velocity of particle
void Particle::SetVelocity(const D3DXVECTOR3& vel)
{
	mVelocity = vel;
}

// Hide or Show the particle
void Particle::SetVisibility(bool on)
{
	if (on)
	{
		mModel->ResetScale();
	}
	else
	{
		mModel->Scale(0.0001f);
	}
}

// Set the lifetime of the particle
void Particle::SetLifetime(float time)
{
	mLifetime = time;
}

// Called to update the position of the Particle
void Particle::Update(float frameTime)
{
	mLifetime -= frameTime;
	mModel->Move(mVelocity.x * frameTime, mVelocity.y * frameTime, mVelocity.z * frameTime);
}