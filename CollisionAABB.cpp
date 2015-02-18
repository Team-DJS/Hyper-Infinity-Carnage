#include "CollisionAABB.hpp"
using namespace HIC;

// Default constructor for CollisionAABB
CollisionAABB::CollisionAABB(const XMFLOAT2& minOffset, const XMFLOAT2& maxOffset) :
	CollisionObject(),
	mMinOffset(minOffset),
	mMaxOffset(maxOffset)
{
}

// Destructor for AABB
CollisionAABB::~CollisionAABB()
{
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
