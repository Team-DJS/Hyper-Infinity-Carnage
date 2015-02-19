#include "CollisionAABB.hpp"
using namespace HIC;

// Default constructor for CollisionAABB
CollisionAABB::CollisionAABB(XMFLOAT2 centre, const XMFLOAT2& minOffset, const XMFLOAT2& maxOffset) :
	CollisionObject(centre),
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

#ifdef _DEBUG
void CollisionAABB::ToggleMarkers()
{
	if (MARKER_MESH)
	{

		if (mMarkersExist)
		{
			//Remove models from memory
			while (!mCollisionMarkers.empty())
			{
				MARKER_MESH->RemoveModel(mCollisionMarkers.back());
				mCollisionMarkers.pop_back();
			}
		}
		else
		{
			//Create the markers, one at each corner and one halfway along each side
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(mMinOffset.x, mHeight, mMinOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(mMinOffset.x, mHeight, mMaxOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(mMaxOffset.x, mHeight, mMinOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(mMaxOffset.x, mHeight, mMaxOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel((mMinOffset.x + mMaxOffset.x) / 2, mHeight, mMinOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel((mMinOffset.x + mMaxOffset.x) / 2, mHeight, mMaxOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(mMinOffset.x, mHeight, (mMinOffset.y + mMaxOffset.y) / 2));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(mMaxOffset.x, mHeight, (mMinOffset.y + mMaxOffset.y) / 2));
			for (uint32_t i = 0; i < mCollisionMarkers.size(); i++)
			{
				mCollisionMarkers[i]->Scale(MARKER_SCALE);
			}
		}
		//Adjust existence flag
		mMarkersExist = !mMarkersExist;
	}
}
#endif