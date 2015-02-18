#pragma once

#include "Common.hpp"
#include "Pickup.hpp"

namespace HIC
{
	class WeaponUpgrade : public Pickup
	{
	public:
		// Default constructor for WeaponUpgrade
		WeaponUpgrade(IMesh* mesh, const XMFLOAT3& position, float lifeTime, float fireRateUpAmount, float damageUpAmount);

		// Destructor for WeaponUpgrade
		~WeaponUpgrade();
	public:
		// Called when the pickup is collides with a player
		void OnPickup(Player* collectingPlayer);
	private:
		float mFireRate;
		float mDamage;
	};
}
