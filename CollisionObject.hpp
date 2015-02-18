#pragma once

#include "Common.hpp"

namespace HIC
{
	class CollisionObject
	{
	public:
		// Default constructor for CollisionObject
		CollisionObject(XMFLOAT2 centre);

		// Destructor for CollisionObject
		virtual ~CollisionObject();
	public:
		// Returns the position of the collision object
		XMFLOAT2 GetPosition() const;

		// Sets the position of the collision object
		void SetPosition(const XMFLOAT2& position);
	private:
		XMFLOAT2 mPosition;
	};
}