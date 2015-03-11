#pragma once

#include "Entity.hpp"

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
		uint32_t GetRemainingLives() const;

		// Called to update the entity
		void OnUpdate(I3DEngine* engine, float frameTime);
	public:
		static IMesh* MESH;
		static uint32_t DEFAULT_LIVES;
	private:
		uint32_t mLives;
	};
}
