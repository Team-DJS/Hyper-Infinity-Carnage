#include "CollisionAABB.hpp"
using namespace HIC;

// Default constructor for CollisionAABB
CollisionAABB::CollisionAABB(XMFLOAT2 centre, const XMFLOAT2& minOffset, const XMFLOAT2& maxOffset, IMesh* markerMesh) :
	CollisionObject(centre, markerMesh),
	mMinOffset(minOffset),
	mMaxOffset(maxOffset)
{
}

// Destructor for AABB
CollisionAABB::~CollisionAABB()
{
#ifdef _DEBUG
	if (mMarkersExist)
	{
		ToggleMarkers();
	}
#endif
}

// Returns the minimum offset of the AABB
XMFLOAT2 CollisionAABB::GetMinOffset() const
{
	return mMinOffset;
}

// Returns the maximum offset of the AABB
XMFLOAT2 CollisionAABB::GetMaxOffset() const
{
	return mMaxOffset;
}

#ifdef _DEBUG
void CollisionAABB::ToggleMarkers()
{
	//Add toggle code
}
#endif