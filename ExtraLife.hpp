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
		virtual ~ExtraLife();
	public:
		// Called to update the pickup each frame
		virtual void Update(float frameTime);

		virtual void OnPickup(Player* collectingPlayer);
	};
}
