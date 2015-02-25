#include "Pickup.hpp"
using namespace HIC;

// Default constructor for Pickup
Pickup::Pickup(IMesh* mesh, const XMFLOAT3& position, float radius, float lifeTime) :
	mModel(mesh->CreateModel(position.x, position.y, position.z)),
	mTimer(lifeTime),
	mCollisionObject(CollisionCylinder(XMFLOAT2(position.x, position.y), radius))
{
}

// Destructor for Entity
Pickup::~Pickup()
{
	mModel->GetMesh()->RemoveModel(mModel);
}

//Returns the model of the pickup
IModel* Pickup::GetModel()
{
	return mModel;
}

// Returns true if the lifetime of the pickup has been exceeded
bool Pickup::IsLifetimeComplete()
{
	return mTimer.IsComplete();
}

// Called to update the pickup each frame
void Pickup::Update(float frameTime)
{
	// Update the timer
	mTimer.Update(frameTime);
}
