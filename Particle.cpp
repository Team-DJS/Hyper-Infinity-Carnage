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
