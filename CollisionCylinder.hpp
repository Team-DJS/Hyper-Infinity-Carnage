#pragma once
#include "CollisionObject.hpp"

namespace HIC
{
	class CollisionCylinder : public CollisionObject
	{
	public:
		// Default constructor for CollisionCylinder
		CollisionCylinder(XMFLOAT2 centre, float radius, IMesh* markerMesh, float markerHeight);

		// Destructor for CollisionCylinder
		~CollisionCylinder();
	public:
		// Returns the radius of the CollisionCylinder
		float GetRadius() const;

		// Sets the radius of the CollisionCylinder
		void SetRadius(float radius);
	private:
		float mRadius;
#ifdef _DEBUG
	private:
		//Collision Markers - Used to see where the collision is
		static const uint32_t kNoSphereMarkers = 10;
		IModel* mMarkers[kNoSphereMarkers];	//Objects to show the boundaries of the collision area
		bool markersExist;					//Says whether or not the collision markers exist
		float height;						//The height at which the collision markers will be drawn
	public:
		//Collision Markers - Used to see where the collision is
		void ToggleMarkers();	//Toggles whether or not this markers' mesh
#endif
	};

}
