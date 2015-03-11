#include "Entity.hpp"
using namespace HIC;

// Default constructor for Entity
Entity::Entity(IMesh* mesh, const XMFLOAT3& position)
{
	// Create the entity model
	mModel = mesh->CreateModel(position.x, position.y, position.z);
}

// Destructor for Entity
Entity::~Entity()
{
	// Remove the entity model
	mModel->GetMesh()->RemoveModel(mModel);
}

// Returns the position of the entity in world space
XMFLOAT3 Entity::GetPosition() const
{
	return XMFLOAT3(mModel->GetX(), mModel->GetY(), mModel->GetZ());
}

// Sets the position of the entity in world space
void Entity::SetPosition(const XMFLOAT3& position)
{
	mModel->SetPosition(position.x, position.y, position.z);
}

// Move the entity in the world axis
void Entity::Move(float dx, float dy, float dz)
{
	mModel->MoveLocal(dx, dy, dz);
}

// Point the model at a given position
void Entity::LookAt(const XMFLOAT3& position)
{
	mModel->LookAt(position.x, position.y, position.z);
}

// Returns the model of the entity
IModel* Entity::GetModel()
{
	return mModel;
}
