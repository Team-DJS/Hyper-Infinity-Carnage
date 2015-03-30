#pragma once

#include "Common.hpp"
#include "Pickup.hpp"

namespace HIC
{
	class WeaponUpgrade : public Pickup
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		
		// Default constructor for WeaponUpgrade
		WeaponUpgrade(IMesh* mesh, const D3DXVECTOR3& position, float radius, float lifeTime, float fireRateUpAmount, uint32_t damageUpAmount);

		// Destructor for WeaponUpgrade
		~WeaponUpgrade();
		
	public:
		//--------------
		// Setters
		//--------------
		
		//---------------
		// Getters
		//---------------

		//--------------
		// Other
		//--------------

		// Called when the pickup is collides with a player
		void OnPickup(Player* collectingPlayer);
	
	public:
		static IMesh* mMesh;

	private:
		float mFireRate;
		uint32_t mDamage;
	};
}
