#pragma once
#include "CollisionObject.hpp"

namespace HIC
{
	class CollisionCylinder : public CollisionObject
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
	
		// Default constructor for CollisionCylinder
		CollisionCylinder(XMFLOAT2 centre, float radius = 0.0f);

		// Destructor for CollisionCylinder
		~CollisionCylinder();
	
	public:
		//--------------
		// Setters
		//--------------
		
		// Sets the radius of the CollisionCylinder
		void SetRadius(float radius);
		
		//---------------
		// Getters
		//---------------
		
		// Returns the radius of the CollisionCylinder
		float GetRadius() const;
		
		//--------------
		// Other
		//--------------

	private:
		float mRadius;

#ifdef _DEBUG
		//--------------------
		// Collision Markers
		//--------------------

	private:
		//No of collision markers per cylinder collision
		static const uint32_t NO_COLLISION_MARKERS;

	public:
		//Collision Markers - Used to see where the collision is
		void ToggleMarkers();	//Toggles whether or not this markers' mesh
#endif
	};

}
