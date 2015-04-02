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
		Projectile(const D3DXVECTOR3& position, const D3DXVECTOR2& velocity, uint32_t damage = 0);

		// Destructor for Projectile
		~Projectile();

	public:
		//--------------
		// Setters
		//--------------
		
		// Sets the models position to offscreen
		void SetPosOffscreen()
		{
			mModel->SetPosition(0, 0, -800);
		}

		// Set the models position
		void SetPos(D3DXVECTOR3 pos)
		{
			mModel->SetPosition(pos.x, pos.y, pos.z);
		}

		// Set projectiles velocity
		void SetVelocity(D3DXVECTOR2 vel)
		{
			mVelocity = vel;
		}

		void SetDamage(uint32_t dmg)
		{
			mDamage = dmg;
		}


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
		D3DXVECTOR2 mVelocity;
		CollisionCylinder mCollisionCylinder;
	};
}
