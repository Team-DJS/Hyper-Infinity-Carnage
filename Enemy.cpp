#include "Enemy.hpp"
using namespace HIC;


//-----------------------------------
// Static Initialisations
//-----------------------------------

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Enemy
Enemy::Enemy(IMesh* mesh, const D3DXVECTOR3& position, float radius, uint32_t damage) :
	Entity(mesh, position, radius),
	mDamage(damage)
{
}

// Destructor for Enemy
Enemy::~Enemy()
{
}

//-----------------------------------
// Setters
//-----------------------------------

//-----------------------------------
// Getters
//-----------------------------------

// Returns the damage the enemy deals
uint32_t Enemy::GetDamage() const
{
	return mDamage;
}

//-----------------------------------
// Other
//-----------------------------------

// Called to update the entity
void Enemy::Update(float frameTime)
{
	MoveX(GetFacingVector().x * 50.0f * frameTime);
	MoveZ(GetFacingVector().z * 50.0f * frameTime);

	GetCollisionCylinder().SetPosition(D3DXVECTOR2(GetWorldPos().x, GetWorldPos().z));
}