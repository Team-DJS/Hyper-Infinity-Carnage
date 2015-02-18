#pragma once
#include "CollisionObject.hpp"

namespace HIC
{
	class CollisionAABB : public CollisionObject
	{
	public:
		// Default constructor for CollisionAABB
		CollisionAABB(const XMFLOAT2& minOffset, const XMFLOAT2& maxOffset);

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
	};
}