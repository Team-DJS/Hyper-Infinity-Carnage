#include "Entity.hpp"
using namespace HIC;

const uint32_t Entity::DEFAULT_HEALTH = 100U;
const uint32_t Entity::DEFAULT_MAX_HEALTH = 100U;

// Default constructor for Entity
Entity::Entity(IMesh* mesh, const XMFLOAT3& position, float radius) :
	mMesh(mesh),
	mModel(mesh->CreateModel(position.x, position.y, position.z)),
	mHealth(DEFAULT_HEALTH),
	mMaxHealth(DEFAULT_MAX_HEALTH),
	mCollisionCylinder(XMFLOAT2(position.x, position.z), radius)
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

// Moves the entity in the world x-axis
void Entity::MoveX(float dx)
{
	mModel->MoveX(dx);
}

// Moves the entity in the world z-axis
void Entity::MoveZ(float dz)
{
	mModel->MoveZ(dz);
}

void Entity::RotateY(float degrees)
{
	mModel->RotateY(degrees);
}

IModel* Entity::GetModel()
{
	return mModel;
}

CollisionCylinder& Entity::GetCollisionCylinder()
{
	return mCollisionCylinder;
}

XMFLOAT3 Entity::GetWorldPos()
{
	return XMFLOAT3(mModel->GetX(), mModel->GetY(), mModel->GetZ());
}

XMFLOAT3 Entity::GetFacingVector()
{
	float matrix[16];
	mModel->GetMatrix(matrix);

	XMFLOAT3 face;
	face.x = matrix[8];
	face.y = matrix[9];
	face.z = matrix[10];
	return face;
}

XMFLOAT3 Entity::GetRightVector()
{
	float matrix[16];
	mModel->GetMatrix(matrix);

	XMFLOAT3 right;
	right.x = matrix[0];
	right.y = matrix[1];
	right.z = matrix[2];
	return right;
}