#include "CollisionCylinder.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

#ifdef _DEBUG
const uint32_t CollisionCylinder::NO_COLLISION_MARKERS = 10;
#endif

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for CollisionCylinder
CollisionCylinder::CollisionCylinder(const XMFLOAT2& centre, float radius) :
	CollisionObject(centre),
	mRadius(radius)
{

}

// Destructor for CollisionCylinder
CollisionCylinder::~CollisionCylinder()
{
}

//-----------------------------------
// Setters
//-----------------------------------

// Sets the radius of the CollisionCylinder
void CollisionCylinder::SetRadius(float radius)
{
	mRadius = radius;
}

//-----------------------------------
// Getters
//-----------------------------------

// Returns the radius of the CollisionCylinder
float CollisionCylinder::GetRadius() const
{
	return mRadius;
}

//-----------------------------------
// Other
//-----------------------------------

#ifdef _DEBUG

//-----------------------------------
// Collision Markers
//-----------------------------------

void CollisionCylinder::ToggleMarkers()
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
			//Calculate positions for models based on how many cubes in a circle and circle based maths
			float degreeOfMarker = 0.0f;
			for (int i = 0; i < NO_COLLISION_MARKERS; i++)
			{
				mCollisionMarkers.push_back(MARKER_MESH->CreateModel(cosf(XMConvertToRadians(degreeOfMarker)) * mRadius + GetPosition().x,
					mHeight,
					sinf(XMConvertToRadians(degreeOfMarker)) * mRadius + GetPosition().y));	//Create marker

				degreeOfMarker += (360.0f / NO_COLLISION_MARKERS);	//Increase angle for new marker placement
				mCollisionMarkers.back()->Scale(MARKER_SCALE);		//Scale the markers to the correct scale so they are visible
			}
		}
		mMarkersExist = !mMarkersExist;	//Adjust existence flag
	}
}
#endif