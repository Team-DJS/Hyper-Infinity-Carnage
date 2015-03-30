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
		CollisionAABB(const D3DXVECTOR2& centre, const D3DXVECTOR2& minOffset, const D3DXVECTOR2& maxOffset);

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
		D3DXVECTOR2 GetMinOffset() const;

		// Returns the maximum offset of the AABB (ignoring position)
		D3DXVECTOR2 GetMaxOffset() const;
		
		// Returns the top left point of the collision box
		D3DXVECTOR2 GetMinPosition();

		// Returns the bottom right point of the collision box
		D3DXVECTOR2 GetMaxPosition();
		
		//--------------
		// Other
		//--------------
		
	private:
		D3DXVECTOR2 mMinOffset;
		D3DXVECTOR2 mMaxOffset;

#ifdef _DEBUG
		//--------------------
		// Collision Markers
		//--------------------

	public:
		//Collision Markers - Used to see where the collision is
		void ToggleMarkers();	//Toggles whether or not this collision markers exist
#endif
	};
}