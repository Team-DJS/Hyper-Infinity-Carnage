#pragma once

#include "Entity.hpp"
#include "Weapon.hpp"

namespace HIC
{
	class Player : public Entity
	{
	public:
		// Default constructor for Player
		Player(const XMFLOAT3& position, float radius);

		// Destructor for Player
		~Player();
	public:
		// Returns the number of remaining lives the player has
		uint32_t GetLives() const;

		// Returns the number of reminaing bombs the player has
		uint32_t GetBombs() const;

		// Returns the weapon which the player has
		Weapon* GetWeapon();

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
		static const float RADIUS;
		static const uint32_t DEFAULT_LIVES;
		static const uint32_t DEFAULT_BOMBS;
	private:
		uint32_t mLives;
		uint32_t mBombs;
		Weapon* mWeapon;
	private:
		//*Movement Variables*//
		//Globals Required to allow movement to perform Correctly
		XMFLOAT2 mVelocity;
		bool mMoveForward;
		bool mMoveBackward;
		bool mMoveLeft;
		bool mMoveRight;
		static const float BASE_THRUST;
		static const float DRAG_COEF;
	public:
		//*Movement Functions*//
		//Set the movement flags for the object to react to in Update()
		void SetMoveForward();
		void SetMoveBackward();
		void SetMoveLeft();
		void SetMoveRight();

	};
}
