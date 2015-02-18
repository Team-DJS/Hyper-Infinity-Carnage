#include "CollisionObject.hpp"
using namespace HIC;

// Default constructor for CollisionObject
CollisionObject::CollisionObject(XMFLOAT2 centre, IMesh* markerMesh) :
	mPosition(centre)
{
#ifdef _DEBUG
	mHeight = 5.0f;
	mMarkerMesh = markerMesh;
#endif
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
