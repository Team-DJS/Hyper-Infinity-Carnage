#pragma once

#include "Common.hpp"
#include "Pickup.hpp"

namespace HIC
{
	class ExtraLife : public Pickup
	{
	public:
		// Default constructor for ExtraLife
		ExtraLife(IMesh* mesh, const XMFLOAT3& position, float lifeTime);

		// Destructor for ExtraLife
		~ExtraLife();
	public:
		// Called when the pickup is collides with a player
		void OnPickup(Player* collectingPlayer);
	};
}
