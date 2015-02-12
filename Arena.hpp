#pragma once

#include "Entity\Player.hpp"

namespace HIC
{
	class Arena
	{
	public:
		// Default constructor for Arena
		Arena();

		// Destructor for Arena
		~Arena();
	public:
		// Updates all the entities inside the arena
		void Update(float frameTime);

		// Proceeds to the next stage
		void NextStage();

		// Saves the game to be loaded at a later date
		void Save();

		// Removes all entities from the arena
		void Clear();
	private:
		Player* mPlayer;
		uint32_t mCurrentStage;
	};
}
