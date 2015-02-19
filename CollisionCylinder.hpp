#pragma once
#include "CollisionObject.hpp"

namespace HIC
{
	class CollisionCylinder : public CollisionObject
	{
	public:
		// Default constructor for CollisionCylinder
		CollisionCylinder(XMFLOAT2 centre, float radius);

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
		//No of collision markers per cylinder collision
		static const uint32_t NO_COLLISION_MARKERS;
	public:
		//Collision Markers - Used to see where the collision is
		void ToggleMarkers();	//Toggles whether or not this markers' mesh
#endif
	};

}
