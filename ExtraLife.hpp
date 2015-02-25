#pragma once

#include "Common.hpp"
#include "Pickup.hpp"

namespace HIC
{
	class ExtraLife : public Pickup
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		// Default constructor for ExtraLife
		ExtraLife(const XMFLOAT3& position, float radius, float lifeTime);

		// Destructor for ExtraLife
		~ExtraLife();
	public:
		//--------------
		// Setters
		//--------------

		//--------------
		// Getters
		//--------------
		
		//--------------
		// Other
		//--------------
		
		// Called when the pickup is collides with a player
		void OnPickup(Player* collectingPlayer);
	
	public:
		static IMesh* MESH;
	
	};
}
