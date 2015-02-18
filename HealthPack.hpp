#pragma once

#include "Common.hpp"
#include "Pickup.hpp"

namespace HIC
{
	class HealthPack : public Pickup
	{
	public:
		// Default constructor for HealthPack
		HealthPack(IMesh* mesh, const XMFLOAT3& position, float lifeTime, uint32_t restoreAmount);

		// Destructor for HealthPack
		~HealthPack();
	public:
		// Called when the pickup is collides with a player
		void OnPickup(Player* collectingPlayer);
	private:
		uint32_t mHealth;
	};
}
