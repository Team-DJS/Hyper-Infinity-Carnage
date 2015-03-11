#pragma once

#include "Entity.hpp"
#include "Weapon.hpp"

namespace HIC
{
	class Player : public Entity
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------
		
		// Default constructor for Player
		Player(const XMFLOAT3& position, float radius);

		// Destructor for Player
		~Player();
		
	public:
		//--------------
		// Setters
		//--------------
		
		// Gives an extra life to the player
		void GiveLife();

		// Gives an extra bomb to the player
		void GiveBomb();

		//---------------
		// Getters
		//---------------
		
		// Returns the number of remaining lives the player has
		uint32_t GetLives() const;

		// Returns the number of reminaing bombs the player has
		uint32_t GetBombs() const;

		// Returns the weapon which the player has
		Weapon* GetWeapon();
		
		//--------------
		// Other
		//--------------

		// Respawns the player inside the arena
		void Respawn();

		// Called to update the entity
		void Update(float frameTime);

		// Called when the arena clears, clears all the projectiles currently alive
		void Clear();
	
	public:
		static IMesh* MESH;
	
	private:
		static const float RADIUS;
		static const uint32_t DEFAULT_LIVES;
		static const uint32_t DEFAULT_BOMBS;
	
	private:
		uint32_t mLives;
		uint32_t mBombs;
		Weapon* mWeapon;
	
	private:
		// Movement Variables
		//Globals Required to allow movement to perform Correctly
		XMFLOAT2 mVelocity; // Horizontal (x) and vertical (y) velocities
		//Movement control states
		bool mMoveForward;	
		bool mMoveBackward;
		bool mMoveLeft;
		bool mMoveRight;

		//Constant values
		static const float BASE_THRUST;
		static const float DRAG_COEF;
		static const float TURN_SPEED;

	public:
		//---------------------
		// Movement / Setters
		//---------------------

		//Set the movement flags for the object to react to in Update()
		void SetMoveForward();
		void SetMoveBackward();
		void SetMoveLeft();
		void SetMoveRight();

		//Set the firing flag for the object to react to in Update()
		void SetTryFire();

	};
}
