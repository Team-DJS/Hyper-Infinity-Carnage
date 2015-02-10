#include "Entity.hpp"
using namespace HIC;

// Default constructor for Entity
Entity::Entity()
{
}

// Destructor for Entity
Entity::~Entity()
{
	// Remove the entity model
	mModel->GetMesh()->RemoveModel(mModel);
}
