#pragma once

#include "Common.hpp"
#include "Pickup.hpp"

namespace HIC
{
	class Bomb : public Pickup
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		// Default constructor for Entity
		Bomb(const D3DXVECTOR3& position, float radius, float lifeTime);

		// Destructor for Entity
		~Bomb();

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
		static IMesh* MESH;

	};
}
