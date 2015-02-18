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
		virtual ~HealthPack();
	public:
		// Called to update the pickup each frame
		virtual void Update(float frameTime);

		virtual void OnPickup(Player* collectingPlayer);

	private:
		uint32_t mHealth;
	};
}
