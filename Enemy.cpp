#include "Enemy.hpp"
using namespace HIC;


//-----------------------------------
// Static Initialisations
//-----------------------------------

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Enemy
Enemy::Enemy(IMesh* mesh, const XMFLOAT3& position, uint32_t damage, float mass) :
	Entity(mesh, position, mass),
	mDamage(damage)
{
	mRigidBody->setAngularDamping(10.0f);
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
	//this->SetModelToPhysicsBody();
	
	//GetRigidBody()->setLinearVelocity(hkVector4(5000.0f, 0.0f, 0.0f));

	//MoveX(GetFacingVector().x * 50.0f * frameTime);
	//MoveZ(GetFacingVector().z * 50.0f * frameTime);

}