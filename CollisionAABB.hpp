#pragma once
#include "CollisionObject.hpp"

namespace HIC
{
	class CollisionAABB : public CollisionObject
	{
	public:
		// Default constructor for CollisionAABB
		CollisionAABB(XMFLOAT2 centre, const XMFLOAT2& minOffset, const XMFLOAT2& maxOffset, IMesh* markerMesh = 0);

		// Destructor for AABB
		~CollisionAABB();
	public:
		// Returns the minimum offset of the AABB
		XMFLOAT2 GetMinOffset() const;

		// Returns the maximum offset of the AABB
		XMFLOAT2 GetMaxOffset() const;
	private:
		XMFLOAT2 mMinOffset;
		XMFLOAT2 mMaxOffset;
#ifdef _DEBUG
	private:
		//Collision Markers - Used to see where the collision is
		static const uint32_t kNoCollisionMarkers = 10;
		IModel* mMarkers[kNoCollisionMarkers];			//Objects to show the boundaries of the collision area
	public:
		//Collision Markers - Used to see where the collision is
		void ToggleMarkers();	//Toggles whether or not this markers' mesh
#endif
	};
}