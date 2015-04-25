#pragma once

#include "Entity.hpp"

namespace HIC
{
	class Enemy : public Entity
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		// Default constructor for Enemy
		Enemy(IMesh* mesh, const D3DXVECTOR3& position, float radius, uint32_t damage);

		// Destructor for Enemy
		~Enemy();
	public:
		//--------------
		// Setters
		//--------------
		static void SetPlayerPosition(D3DXVECTOR3 playerPos);

		void SetDamage(uint32_t damage);

		// Sets the enemy to be a boss
		void SetBoss(bool boss);

		//---------------
		// Getters
		//---------------

		// Returns whether the enemy is a boss
		bool GetBoss();

		// Returns the damage the enemy deals
		uint32_t GetDamage() const;

		//--------------
		// Other
		//--------------

		// Called to update the entity
		void Update(float frameTime);
	private:
		uint32_t mDamage;

		bool mBoss;

		static D3DXVECTOR3 PLAYER_POS;
	};
}
