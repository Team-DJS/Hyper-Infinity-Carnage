#include "Projectile.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

IMesh* Projectile::MESH = nullptr;
const float Projectile::RADIUS = 1.0f;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Projectile
Projectile::Projectile(const XMFLOAT3& position, const XMFLOAT2& velocity, uint32_t damage) :
	mModel(MESH->CreateModel(position.x, position.y, position.z)),
	mDamage(damage),
	mVelocity(velocity),
	mCollisionCylinder(XMFLOAT2(position.x, position.z), RADIUS)
{
}

// Destructor for Projectile
Projectile::~Projectile()
{
	MESH->RemoveModel(mModel);
}

//-----------------------------------
// Setters
//-----------------------------------

//-----------------------------------
// Getters
//-----------------------------------

// Returns the damage of the projectile
uint32_t Projectile::GetDamage()
{
	return mDamage;
}

CollisionCylinder& Projectile::GetCollisionObject()
{
	return mCollisionCylinder;
}

//-----------------------------------
// Other
//-----------------------------------

// Updates the movement of the projectile
void Projectile::Update(float frameTime)
{
	// Move the model
	mModel->Move(mVelocity.x * 50.0f * frameTime,
				 0.0f,
				 mVelocity.y * 50.0f * frameTime);

	// Update the collision cylinder with the new position
	XMFLOAT2 newPosition(mModel->GetX(), mModel->GetZ());
	mCollisionCylinder.SetPosition(newPosition);
}
