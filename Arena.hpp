#pragma once

#include "Player.hpp"
#include "Enemy.hpp"
#include "Scenery.hpp"

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
		Player mPlayer;
		std::vector<Enemy*> mEnemies;
		Scenery mArenaModel;
		std::vector<Scenery*> mSceneryObjects;
		uint32_t mCurrentStage;
	public:
		static IMesh* ARENA_MESH;

	public:
		void TargetCamera(ICamera* camera);	//Point the camera at the current focal point - done this way as camera is global
	};
}
