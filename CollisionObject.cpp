#include "CollisionObject.hpp"
using namespace HIC;

// Default constructor for CollisionObject
CollisionObject::CollisionObject() :
	mPosition(0.0f, 0.0f)
{
}

// Destructor for CollisionObject
CollisionObject::~CollisionObject()
{
}

// Returns the position of the collision object
XMFLOAT2 CollisionObject::GetPosition() const
{
	return mPosition;
}

// Sets the position of the collision object
void CollisionObject::SetPosition(const XMFLOAT2& position)
{
	mPosition = position;
}
