#include "Entity.hpp"
using namespace HIC;

const uint32_t Entity::DEFAULT_HEALTH = 100U;
const uint32_t Entity::DEFAULT_MAX_HEALTH = 100U;

// Default constructor for Entity
Entity::Entity(IMesh* mesh, const XMFLOAT3& position) : 
	mMesh(mesh),
	mModel(mesh->CreateModel(position.x, position.y, position.z)),
	mHealth(DEFAULT_HEALTH),
	mMaxHealth(DEFAULT_MAX_HEALTH)
{
}

// Destructor for Entity
Entity::~Entity()
{
	// Remove the entity model
	mModel->GetMesh()->RemoveModel(mModel);
}

// Returns the health of entity
uint32_t Entity::GetHealth() const
{
	return mHealth;
}

// Gives health to the entity. If this exceeds the maximum health, then it is clamped
void Entity::GiveHealth(uint32_t health)
{
	// Add the health to the current health
	mHealth += health;

	// Clamp it so it does not go beyond the maximum health
	if (mHealth > mMaxHealth)
	{
		mHealth = mMaxHealth;
	}
}

// Takes the given health away from the entity. If the damage is greater than the current
// health of the entity, then this method will return true, otherwise false.
bool Entity::TakeHealth(uint32_t damage)
{
	if (damage > mHealth)
	{
		mHealth = 0U;
		return true;
	}
	mHealth -= damage;
	return false;
}

// Points the model at a given position
void Entity::LookAt(const XMFLOAT3& position)
{
	mModel->LookAt(position.x, position.y, position.z);
}

// Moves the entity in the world axis
void Entity::Move(float dx, float dy, float dz)
{
	mModel->Move(dx, dy, dz);
}
