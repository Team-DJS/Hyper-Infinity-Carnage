#pragma once

#include "Common.hpp"
#include "Player.hpp"
#include "Timer.hpp"

namespace HIC
{
	class Pickup
	{
	public:
		// Default constructor for Pickup
		Pickup(IMesh* mesh, const XMFLOAT3& position, float lifeTime);

		// Destructor for Entity
		virtual ~Pickup();

	public:
		//Returns the model of the pickup
		IModel* GetModel();

		// Returns true if the lifetime of the pickup has been exceeded
		bool IsLifetimeComplete();

		// Called to update the pickup each frame
		void Update(float frameTime);

		// Called when the pickup is collides with a player
		virtual void OnPickup(Player* collectingPlayer) = 0;
	private:
		IModel* mModel;
		Timer mTimer;
		//Collision object
		//Particle emitter
	};
}
