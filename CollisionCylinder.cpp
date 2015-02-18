#include "CollisionCylinder.hpp"
using namespace HIC;

// Default constructor for CollisionCylinder
CollisionCylinder::CollisionCylinder(XMFLOAT2 centre, float radius, IMesh* markerMesh) :
	CollisionObject(centre, markerMesh),
	mRadius(radius)
{

}

// Destructor for CollisionCylinder
CollisionCylinder::~CollisionCylinder()
{
#ifdef _DEBUG
	if (mMarkersExist)
	{
		ToggleMarkers();
	}
#endif
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

#ifdef _DEBUG
void CollisionCylinder::ToggleMarkers()
{
	//Add toggle code
}
#endif