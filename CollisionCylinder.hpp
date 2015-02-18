#pragma once
#include "CollisionObject.hpp"

namespace HIC
{
	class CollisionCylinder : public CollisionObject
	{
	public:
		// Default constructor for CollisionCylinder
		CollisionCylinder();

		// Destructor for CollisionCylinder
		~CollisionCylinder();
	public:
		// Returns the radius of the CollisionCylinder
		float GetRadius() const;

		// Sets the radius of the CollisionCylinder
		void SetRadius(float radius);
	private:
		float mRadius;
	};

}
