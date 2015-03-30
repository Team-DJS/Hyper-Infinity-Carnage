#pragma once

#include "Common.hpp"
#include "Pickup.hpp"

namespace HIC
{
	class HealthPack : public Pickup
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		//--------------
		// Setters
		//--------------
		//---------------
		// Getters
		//---------------
		//--------------
		// Other
		//--------------
		// Default constructor for HealthPack
		HealthPack(const D3DXVECTOR3& position, float radius, float lifeTime, uint32_t restoreAmount);

		// Destructor for HealthPack
		~HealthPack();
	public:
		// Called when the pickup is collides with a player
		void OnPickup(Player* collectingPlayer);
	public:
		static IMesh* MESH;
	private:
		uint32_t mHealth;
	};
}
