#include "CollisionAABB.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for CollisionAABB
CollisionAABB::CollisionAABB(const XMFLOAT2& centre, const XMFLOAT2& minOffset, const XMFLOAT2& maxOffset) :
	CollisionObject(centre),
	mMinOffset(minOffset),
	mMaxOffset(maxOffset)
{
}

// Destructor for AABB
CollisionAABB::~CollisionAABB()
{
}

//-----------------------------------
// Setters
//-----------------------------------

//-----------------------------------
// Getters
//-----------------------------------

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

XMFLOAT2 CollisionAABB::GetMinPosition()
{
	XMFLOAT2 pos = GetPosition();
	pos.x += mMinOffset.x;
	pos.y += mMinOffset.y;
	return pos;
}
		 
XMFLOAT2 CollisionAABB::GetMaxPosition()
{
	XMFLOAT2 pos = GetPosition();
	pos.x += mMaxOffset.x;
	pos.y += mMaxOffset.y;
	return pos;
}

//-----------------------------------
// Other
//-----------------------------------

#ifdef _DEBUG

//-----------------------------------
// Collision Markers
//-----------------------------------

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
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(GetPosition().x + mMinOffset.x, mHeight, GetPosition().x + mMinOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(GetPosition().x + mMinOffset.x, mHeight, GetPosition().x + mMaxOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(GetPosition().x + mMaxOffset.x, mHeight, GetPosition().x + mMinOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(GetPosition().x + mMaxOffset.x, mHeight, GetPosition().x + mMaxOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel((GetPosition().x + mMinOffset.x + mMaxOffset.x) / 2, mHeight, GetPosition().x + mMinOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel((GetPosition().x + mMinOffset.x + mMaxOffset.x) / 2, mHeight, GetPosition().x + mMaxOffset.y));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(GetPosition().x + mMinOffset.x, mHeight, (mMinOffset.y + GetPosition().x + mMaxOffset.y) / 2));
			mCollisionMarkers.push_back(MARKER_MESH->CreateModel(GetPosition().x + mMaxOffset.x, mHeight, (mMinOffset.y + GetPosition().x + mMaxOffset.y) / 2));
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