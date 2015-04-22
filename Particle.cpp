#include "Particle.hpp"
using namespace HIC;

// Creates a new particle with the given mesh, position, velocity and lifetime
Particle* HIC::CreateParticle(IMesh* mesh, const D3DXVECTOR3& position, const D3DXVECTOR3& velocity, float lifetime)
{
	return new Particle{ mesh->CreateModel(position.x, position.y, position.z), velocity, lifetime };
}

// Deletes the given particle and sets it to nullptr
void HIC::DeleteParticle(Particle* &particle)
{
	auto particleModel = particle->model;
	particleModel->GetMesh()->RemoveModel(particleModel);

	delete(particle);
	particle = nullptr;
}

// Shows the given particle
void HIC::ShowParticle(Particle* particle)
{
	particle->model->ResetScale();
}

// Hides the given particle
void HIC::HideParticle(Particle* particle)
{
	particle->model->Scale(0.0001f);
}

//
//// Default constructor for Particle
//Particle::Particle(IMesh* mesh, float lifetime) :
//	mModel(mesh->CreateModel()),
//	mLifetime(lifetime)
//{
//}
//
//// Destructor for Particle
//Particle::~Particle()
//{
//	mModel->GetMesh()->RemoveModel(mModel);
//}
//
//// Returns true if the lifetime of the particle is over
//bool Particle::IsLifetimeOver() const
//{
//	return mLifetime <= 0.0f;
//}
//
//// Set position of the particle
//void Particle::SetPosition(const D3DXVECTOR3& pos)
//{
//	mModel->SetPosition(pos.x, pos.y, pos.z);
//}
//
//// Set velocity of particle
//void Particle::SetVelocity(const D3DXVECTOR3& vel)
//{
//	mVelocity = vel;
//}
//
//// Hide or Show the particle
//void Particle::SetVisibility(bool on)
//{
//	if (on)
//	{
//		mModel->ResetScale();
//	}
//	else
//	{
//		mModel->Scale(0.0001f);
//	}
//}
//
//// Set the lifetime of the particle
//void Particle::SetLifetime(float time)
//{
//	mLifetime = time;
//}
//
//// Called to update the position of the Particle
//void Particle::Update(float frameTime)
//{
//	mLifetime -= frameTime;
//	mModel->Move(mVelocity.x * frameTime, mVelocity.y * frameTime, mVelocity.z * frameTime);
//	mModel->LookAt(gCamera);
//}