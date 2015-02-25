#pragma once

#include "Player.hpp"
#include "Enemy.hpp"
#include "Scenery.hpp"

namespace HIC
{
	class Arena
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------

		// Default constructor for Arena
		Arena(I3DEngine* engine);

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
	
	public:
		static IMesh* ARENA_MESH;
	
	private:
		// Pointer to the game engine
		I3DEngine* mEngine;

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

	};
}