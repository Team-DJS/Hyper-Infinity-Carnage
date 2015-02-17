#pragma once

#include "Entity.hpp"
#include "Weapon.hpp"

namespace HIC
{
	class Player : public Entity
	{
	public:
		// Default constructor for Player
		Player();

		// Destructor for Player
		~Player();
	public:
		// Returns the number of remaining lives the player has
		uint32_t GetLives() const;

		// Returns the number of reminaing bombs the player has
		uint32_t GetBombs() const;

		// Gives an extra life to the player
		void GiveLife();

		// Gives an extra bomb to the player
		void GiveBomb();

		// Respawns the player inside the arena
		void Respawn();

		// Called to update the entity
		void Update(float frameTime);
	public:
		static IMesh* MESH;
	private:
		static const uint32_t DEFAULT_LIVES;
		static const uint32_t DEFAULT_BOMBS;
	private:
		uint32_t mLives;
		uint32_t mBombs;
		Weapon* mWeapon;
	};
}
