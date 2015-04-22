#pragma once

#include "Common.hpp"
#include "Projectile.hpp"
#include "AudioSource.hpp"
#include "Timer.hpp"

namespace HIC
{
	class Weapon
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------

		// Constructor for Weapon
		Weapon(float fireRate = 0.5f, uint32_t damage = 50U, uint32_t noBarrels = 1U);

		// Destructor for Weapon
		~Weapon();

	public:
		//--------------
		// Setters
		//--------------

		// Sets the fire rate to X bullets per second (1/fireRate)
		void SetFireRate(float bulletsPerSecond);

		// Sets the damage of the weapon to the given amount
		void SetDamage(uint32_t damage);

		// Gives the weapon another barrel
		void AddBarrel();

		//Sets whether or not the weapon should try to fire this frame (will decide to fire based on fire rate)
		void SetFire();

		//---------------
		// Getters
		//---------------

		//Returns the number of bullets fired per second
		float GetFireRate();

		//Returns the damage that a new projectile will deliver
		uint32_t GetDamage();

		//Returns the projectiles for collision
		deque<Projectile*>& GetProjectiles();

		//--------------
		// Other
		//--------------

		// Called to update the weapon
		void Update(float frameTime, const D3DXVECTOR3 playerPosition, const D3DXVECTOR3 playerFacingVector);

		// Removes a projectile from the arena
		void RemoveProjectile(std::deque<Projectile*>::iterator& iter);

		// Called to clear all projectiles currently alive
		void Clear();

		// Operator overload of the output stream operator
		friend std::ostream& operator<<(std::ostream& stream, const Weapon& weapon)
		{
			stream << weapon.mFireRate << std::endl;
			stream << weapon.mDamage << std::endl;
			stream << weapon.mNoBarrels << std::endl;
			return stream;
		}

		// Operator overload of the input stream operator
		friend std::istream& operator>>(std::istream& stream, Weapon& weapon)
		{
			stream >> weapon.mFireRate;
			stream >> weapon.mDamage;
			stream >> weapon.mNoBarrels;

			// Reset the timer
			weapon.mFireTimer.Reset(weapon.mFireRate);

			return stream;
		}
	public:
		static AudioSource* SHOOT_SOUND;
	private:
		// Shoots a projectile in a direction
		void Shoot(const D3DXVECTOR3& direction, const D3DXVECTOR3 playerFacingVector);
	private:
		float mFireRate;						// Length of time between bullet fires
		float mMaxFireRate;						// The maximum fire rate with the current number of barrels
		uint32_t mDamage;						// Amount of damage a bullet deals upon contact
		uint32_t mNoBarrels;					// Number of barrels the ship fires at once
		deque <Projectile*> mProjectiles;		// Vector of projectiles that this weapon has fired
		vector <Projectile*> mProjectilePool;	// Vector of projectiles that the weapon can fire
		Timer mFireTimer;						// The timer used to determine when the next projectile can be shot
		bool mTryFire;							//Whether or not the weapon should try to fire a round this frame
	};
}