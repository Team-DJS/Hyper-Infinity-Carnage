#include "CollisionObject.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

#ifdef _DEBUG
const float CollisionObject::MARKER_SCALE = 20.0f;
IMesh* CollisionObject::MARKER_MESH = nullptr;
#endif

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for CollisionObject
CollisionObject::CollisionObject(const XMFLOAT2& centre) :
	mPosition(centre)
{
#ifdef _DEBUG
	mHeight = 5.0f;
	mMarkersExist = false;
#endif
}

// Destructor for CollisionObject
CollisionObject::~CollisionObject()
{
#ifdef _DEBUG
	//Remove models from memory
	while (!mCollisionMarkers.empty())
	{
		MARKER_MESH->RemoveModel(mCollisionMarkers.back());
		mCollisionMarkers.pop_back();
	}
#endif
}

//-----------------------------------
// Setters
//-----------------------------------

// Sets the position of the collision object
void CollisionObject::SetPosition(const XMFLOAT2& newPosition)
{
#ifdef _DEBUG
	//Calculate the required movement to re-centre the collision - use current centre and new centre to reposition
	XMFLOAT2 diff = XMFLOAT2(newPosition.x - mPosition.x, newPosition.y - mPosition.y);

	//If the markers exist they need to be moved alongside the centre
	if (mMarkersExist)
	{
		for (uint32_t i = 0; i < mCollisionMarkers.size(); i++)
		{
			mCollisionMarkers.at(i)->Move(diff.x, 0.0f, diff.y);
		}
	}
#endif

	mPosition = newPosition;	//Update stored position variable
}

//-----------------------------------
// Getters
//-----------------------------------

//-----------------------------------
// Other
//-----------------------------------

// Returns the position of the collision object
XMFLOAT2 CollisionObject::GetPosition() const
{
	return mPosition;
}