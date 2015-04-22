#pragma once
#include "Pickup.hpp"
namespace HIC
{
	class Shield : public Pickup
	{
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		// Default constructor for Entity
		Shield(const D3DXVECTOR3& position, float radius, float lifeTime);

		// Destructor for Entity
		~Shield();

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
