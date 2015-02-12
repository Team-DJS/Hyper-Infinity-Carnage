#include "Enemy.hpp"
using namespace HIC;

// Default constructor for Enemy
Enemy::Enemy(IMesh* mesh, const XMFLOAT3& position, uint32_t damage) :
	Entity(mesh, position),
	mDamage(damage)
{

}

// Destructor for Enemy
Enemy::~Enemy()
{
}

// Returns the damage the enemy deals
uint32_t Enemy::GetDamage() const
{
	return mDamage;
}

// Called to update the entity
void Enemy::Update(float frameTime)
{
}
