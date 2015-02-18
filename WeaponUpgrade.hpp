#pragma once

#include "Common.hpp"
#include "Pickup.hpp"

namespace HIC
{
	class WeaponUpgrade : public Pickup
	{
	public:
		// Default constructor for Entity
		WeaponUpgrade(IMesh* mesh, const XMFLOAT3& position, float lifeTime, float fireRateUpAmount, float damageUpAmount);
		
		// Destructor for Entity
		virtual ~WeaponUpgrade();
	public:
		// Called to update the pickup each frame
		virtual void Update(float frameTime);

		// Called when the player 'Picks up' the pickup
		virtual void OnPickup(Player* collectingPlayer);

	private:
		float mFireRate;
		float mDamage;
	};
}
