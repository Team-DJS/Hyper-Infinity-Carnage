#include "Particle.hpp"

using namespace HIC;

// Default constructor for Particle
Particle::Particle(IMesh* mesh, const XMFLOAT3& position, const XMFLOAT3& velocity) :
	mModel(mesh->CreateModel(position.x, position.y, position.z)),
	mVelocity(velocity)
{
}

// Destructor for Particle
Particle::~Particle()
{
	mModel->GetMesh()->RemoveModel(mModel);
}

// Called to update the position of the Particle
void Particle::Update(float frameTime)
{
	mModel->Move(mVelocity.x * frameTime, mVelocity.y * frameTime, mVelocity.z * frameTime);
}