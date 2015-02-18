#pragma once

#include "Common.hpp"
#include "Player.hpp"

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

		// Called to update the pickup each frame
		virtual void Update(float frameTime) = 0;

		virtual void OnPickup(Player* collectingPlayer) = 0;

	private:
		IModel* mModel;
		//Collision object
		//Timer object
		//Particle emitter
	};
}
