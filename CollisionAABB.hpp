#pragma once
#include "CollisionObject.hpp"

namespace HIC
{
	class CollisionAABB : public CollisionObject
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
	
		// Default constructor for CollisionAABB
		CollisionAABB(const XMFLOAT2& centre, const XMFLOAT2& minOffset, const XMFLOAT2& maxOffset);

		// Destructor for AABB
		~CollisionAABB();
	
	public:
		//--------------
		// Setters
		//--------------
		
		//---------------
		// Getters
		//---------------
		
		// Returns the minimum offset of the AABB (ignoring position)
		XMFLOAT2 GetMinOffset() const;

		// Returns the maximum offset of the AABB (ignoring position)
		XMFLOAT2 GetMaxOffset() const;
		
		// Returns the top left point of the collision box
		XMFLOAT2 GetMinPosition();

		// Returns the bottom right point of the collision box
		XMFLOAT2 GetMaxPosition();
		
		//--------------
		// Other
		//--------------
		
	private:
		XMFLOAT2 mMinOffset;
		XMFLOAT2 mMaxOffset;

#ifdef _DEBUG
		//--------------------
		// Collision Markers
		//--------------------

	public:
		//Collision Markers - Used to see where the collision is
		void ToggleMarkers();	//Toggles whether or not this markers' mesh
#endif
	};
}