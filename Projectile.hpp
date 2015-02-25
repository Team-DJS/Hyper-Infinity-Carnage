#pragma once

#include "Common.hpp"
#include "CollisionCylinder.hpp"

namespace HIC
{
	class Projectile
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		// Constructor for Projectile
		Projectile(const XMFLOAT3& position, const XMFLOAT2& velocity, uint32_t damage);

		// Destructor for Projectile
		~Projectile();

	public:
		//--------------
		// Setters
		//--------------
		
		//---------------
		// Getters
		//---------------
		
		// Returns the damage of the projectile
		uint32_t GetDamage();
		
		// Returns the collision cylinder of this projectile
		CollisionCylinder& GetCollisionObject();
		
		//--------------
		// Other
		//--------------

		// Updates the movement of the projectile
		void Update(float frameTime);

	public:
		static IMesh* MESH;
		static const float RADIUS;

	private:
		IModel* mModel;
		uint32_t mDamage;
		XMFLOAT2 mVelocity;
		CollisionCylinder mCollisionCylinder;
	};
}
