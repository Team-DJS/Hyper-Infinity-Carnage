#include "Pickup.hpp"
using namespace HIC;

const float Pickup::ROTATION_SPEED = 90.0f;
const float Pickup::HOVER_RANGE = 5.0f;
const float Pickup::HOVER_SPEED = 3.0f;

// Default constructor for Pickup
Pickup::Pickup(IMesh* mesh, const D3DXVECTOR3& position, float radius, float lifeTime) :
	mModel(mesh->CreateModel(position.x, position.y, position.z)),
	mHeight(position.y),
	mTimer(lifeTime),
	mCollisionObject(CollisionCylinder(D3DXVECTOR2(position.x, position.z), radius)),
	mHoverCounter(0.0f)
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

CollisionCylinder* Pickup::GetCollisionCylinder()
{
	return &mCollisionObject;
}

// Called to update the pickup each frame
void Pickup::Update(float frameTime)
{
	// Update the timer
	// mTimer.Update(frameTime);

	mModel->RotateY(ROTATION_SPEED * frameTime);

	mHoverCounter += frameTime * HOVER_SPEED;
	mModel->SetY(cosf(mHoverCounter) * HOVER_RANGE + mHeight);// * HOVER_RANGE);

}
