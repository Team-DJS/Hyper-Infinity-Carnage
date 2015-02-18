#pragma once

#include "Common.hpp"
#include "Pickup.hpp"

namespace HIC
{
	class Bomb : public Pickup
	{
	public:
		// Default constructor for Entity
		Bomb(IMesh* mesh, const XMFLOAT3& position, float lifeTime);

		// Destructor for Entity
		virtual ~Bomb();
	public:
		// Called to update the pickup each frame
		virtual void Update(float frameTime);

		virtual void OnPickup(Player* collectingPlayer);
	};
}
