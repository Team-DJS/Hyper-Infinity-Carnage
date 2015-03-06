#pragma once

#include "Player.hpp"
#include "Enemy.hpp"
#include "Scenery.hpp"
#include "CollisionFunctions.hpp"

namespace HIC
{
	class Arena
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------

		// Default constructor for Arena
		Arena();

		// Destructor for Arena
		~Arena();

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
		
		// Updates all the entities inside the arena
		void Update(float frameTime);

		// Proceeds to the next stage
		void NextStage();

		// Saves the game to be loaded at a later date
		void Save();

		// Removes all entities from the arena
		void Clear();
	
		//Target the camera at the desired item
		void TargetCamera(ICamera* camera);	//Point the camera at the current focal point
	
		//Spawn a group of enemies (TO DO: Create parameters for the rules of spawning enemies)
		void SpawnEnemies();

	public:
		static IMesh* ARENA_MESH;
		static IMesh* ENEMY_MESH;

	private:
		// Pointer to the game engine

		// The player character
		Player mPlayer;

		//Vector of enemies
		std::vector<Enemy*> mEnemies;

		// Model for arena area
		Scenery mArenaModel;

		// Vector of scenery (non-interactive) items
		std::vector<Scenery*> mSceneryObjects;

		// Current stage number
		uint32_t mCurrentStage;

		CollisionAABB mCollisionBox;

	};
}