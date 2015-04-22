#pragma once

#include "Common.hpp"
#include "Player.hpp"
#include "Timer.hpp"

namespace HIC
{
	class Pickup
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		
		// Default constructor for Pickup
		Pickup(IMesh* mesh, const D3DXVECTOR3& position, float radius, float lifeTime);

		// Destructor for Entity
		virtual ~Pickup();
		
	public:
		//--------------
		// Setters
		//--------------
		
		//---------------
		// Getters
		//---------------
		
		//Returns the model of the pickup
		IModel* GetModel();
		
		// Returns true if the lifetime of the pickup has been exceeded
		bool IsLifetimeComplete();

		// Returns the collisionObject for the pickup
		CollisionCylinder* GetCollisionCylinder();
		
		//--------------
		// Other
		//--------------
		
		// Called to update the pickup each frame
		void Update(float frameTime);

		// Called when the pickup is collides with a player
		virtual void OnPickup(Player* collectingPlayer) = 0;

	private:
		static const float ROTATION_SPEED;
		static const float HOVER_RANGE;
		static const float HOVER_SPEED;

	private:
		IModel* mModel;
		Timer mTimer;
		CollisionCylinder mCollisionObject;
		float mHeight;
		float mHoverCounter;
		//Particle emitter
	};
}
