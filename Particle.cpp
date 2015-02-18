#include "Particle.hpp"

using namespace HIC;

// Default constructor for the Particle
Particle::Particle(IMesh* mesh, XMFLOAT3& position, XMFLOAT3 velocity) :
	mModel(mesh->CreateModel(position.x, position.y, position.z)),
	mVelocity(velocity)
{
}

// Default destructor for the Particle
Particle::~Particle()
{
	mModel->GetMesh()->RemoveModel(mModel);
}

// Called to update the position of the Particle
void Particle::Update(float frameTime)
{
	mModel->Move(mVelocity.x * frameTime, mVelocity.y * frameTime, mVelocity.z * frameTime);
}