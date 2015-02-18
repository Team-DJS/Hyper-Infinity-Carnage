#pragma once

#include "Common.hpp"
#include "Pickup.hpp"

namespace HIC
{
	class Bomb : public Pickup
	{
	public:
		// Default constructor for Entity
		Bomb(const XMFLOAT3& position, float lifeTime);

		// Destructor for Entity
		~Bomb();
	public:
		// Called when the pickup is collides with a player
		void OnPickup(Player* collectingPlayer);

		static IMesh* MESH;
	};
}
