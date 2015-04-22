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
		Player(const D3DXVECTOR3& position, float radius);

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

		// Takes a life from the player
		void TakeLife();

		// Takes a bomb from the player
		void TakeBomb();

		// Gives gives shield to the player
		void GiveShield();

		//---------------
		// Getters
		//---------------

		// Returns the number of remaining lives the player has
		uint32_t GetLives() const;

		// Returns the number of reminaing bombs the player has
		uint32_t GetBombs() const;

		// Returns the weapon which the player has
		Weapon* GetWeapon();

		// returns if the players shield is up
		bool IsShielded();

		//--------------
		// Other
		//--------------

		// Respawns the player inside the arena
		void Respawn();

		// Called to update the entity
		void Update(float frameTime);

		// Called when the arena clears, clears all the projectiles currently alive
		void Clear();

		// Operator overload of the output stream operator
		friend std::ostream& operator<<(std::ostream& stream, const Player& player)
		{
			stream << player.mLives << std::endl;
			stream << player.mBombs << std::endl;
			stream << (*player.mWeapon) << std::endl;
			return stream;
		}

		// Operator overload of the input stream operator
		friend std::istream& operator>>(std::istream& stream, Player& player)
		{
			stream >> player.mLives;
			stream >> player.mBombs;
			stream >> (*player.mWeapon);
			return stream;
		}
	public:
		static IMesh* MESH;
		static IMesh* SHIELD;

	private:
		static const float RADIUS;
		static const uint32_t DEFAULT_LIVES;
		static const uint32_t DEFAULT_BOMBS;

	private:
		uint32_t mLives;
		uint32_t mBombs;
		Weapon* mWeapon;

		bool mShield;
		Timer mShieldTimer;
		IModel* mShieldModel;

	private:
		// Movement Variables
		//Globals Required to allow movement to perform Correctly
		//Movement control states
		bool mMoveForward;
		bool mMoveBackward;
		bool mMoveLeft;
		bool mMoveRight;

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
