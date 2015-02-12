#pragma once

#include "Entity\Player.hpp"
#include "Entity\Enemy.hpp"

namespace HIC
{
	class Arena
	{
	public:
		// Default constructor for Arena
		Arena(I3DEngine* engine);

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
		I3DEngine* mEngine;
		Player* mPlayer;
		std::vector<Enemy*> mEnemies;
		uint32_t mCurrentStage;
	};
}
