#pragma once

#include "Player.hpp"
#include "Enemy.hpp"
#include "Scenery.hpp"
#include "Pickup.hpp"
#include "WeaponUpgrade.hpp"
#include "ExtraLife.hpp"
#include "HealthPack.hpp"
#include "Bomb.hpp"
#include "CollisionFunctions.hpp"
#include "AudioManager.hpp"
#include "FountainEmitter.hpp"
#include "SpawnTunnel.hpp"

namespace HIC
{
	class Arena
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------

		// Default constructor for Arena
		Arena(bool loadFromFile);

		// Destructor for Arena
		~Arena();

	public:
		//--------------
		// Setters
		//--------------

		//---------------
		// Getters
		//---------------

		bool GetPlayerStatus()
		{
			return mPlayerStatus;
		}

		//--------------
		// Other
		//--------------

		// Updates all the entities inside the arena
		void Update(float frameTime);

		// Proceeds to the next stage
		void LoadStage(uint32_t stageNumber);

		// Loads the game from the save file
		// Returns true if the load file exist, false otherwise
		bool LoadFromFile();

		// Saves the game to be loaded at a later date
		void SaveToFile();

		// Removes all entities from the arena
		void Clear();

		// Target the camera at the desired item
		void TargetCamera(ICamera* camera);	//Point the camera at the current focal point

	private:
		// Spawn an enemy on to arena (TO DO: Create parameters for the rules of spawning enemies)
		void SpawnEnemy();

		// Create enemy pool
		void CreateEnemies();

		// Create a new pickup with random type
		void CreateNewPickup();

	public:
		static IMesh* ARENA_MESH;
		static IMesh* ENEMY_MESH;
#ifdef _DEBUG
		static tle::IFont* DebugHUD;
#endif
	private:
		static const std::string SAVE_FILENAME;
	private:
		// Pointer to the game engine

		// The player character
		Player mPlayer;
		bool mPlayerStatus;

		uint32_t mScore;
		uint32_t mCurrentScore;

		// Vector of enemies
		std::vector<Enemy*> mEnemies;

		// switch to decide which enemies to do collision on each frame
		bool mCollisionSwitch;

		// Switch for the Bomb
		bool mBombSwitch;
		Timer mBombExplosionTimer;
		IModel* mBombModel;
		AudioSource* mBombSound;

		// Enemy Spawn Models
		std::vector<SpawnTunnel*> mSpawnTunnels;

		// Vector of a pool of enemies
		std::vector<Enemy*> mEnemyPool;

		// Number of enemies to defeat this stage
		uint32_t mNoOfEnemies;

		// Vector of pickups
		std::vector<Pickup*> mPickups;
		Timer mPickupTimer;

		// Model for arena area
		Scenery mArenaModel;

		// Particle effects for the arena
		std::vector<ParticleEmitter*> mArenaParticles;

		// Vector of scenery (non-interactive) items
		std::vector<Scenery*> mSceneryObjects;

		// Current stage number
		uint32_t mCurrentStage;

		CollisionAABB mCollisionBox;
		CollisionCylinder mBombCollisionCylinder;

		AudioSource* mGameMusic;
		AudioSource* mEnemySpawnSound;
	};
}