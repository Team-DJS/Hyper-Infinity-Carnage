#include "CollisionCylinder.hpp"
using namespace HIC;

// Default constructor for CollisionCylinder
CollisionCylinder::CollisionCylinder() :
	CollisionObject(),
	mRadius(0.0f)
{
}

// Destructor for CollisionCylinder
CollisionCylinder::~CollisionCylinder()
{
}

// Returns the radius of the CollisionCylinder
float CollisionCylinder::GetRadius() const
{
	return mRadius;
}

// Sets the radius of the CollisionCylinder
void CollisionCylinder::SetRadius(float radius)
{
	mRadius = radius;
}
