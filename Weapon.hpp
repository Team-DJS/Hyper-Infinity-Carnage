#pragma once

#include "Common.hpp"
#include "Projectile.hpp"
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
		Weapon(float fireRate = 0.3f, uint32_t damage = 50U, uint32_t noBarrels = 1U);

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
		vector<Projectile*> GetProjectiles();

		//--------------
		// Other
		//--------------

		// Called to update the weapon
		void Update(float frameTime, const XMFLOAT3 playerPosition, const XMFLOAT3 playerFacingVector);


		void RemoveProjectile(uint32_t i);
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

		friend std::istream& operator>>(std::istream& stream, Weapon& weapon)
		{
			stream >> weapon.mFireRate;
			stream >> weapon.mDamage;
			stream >> weapon.mNoBarrels;
			return stream;
		}
	private:
		// Shoots a projectile in a direction
		void Shoot(const XMFLOAT3& direction, const XMFLOAT3 playerFacingVector);

	private:
		float mFireRate;					//Length of time between bullet fires
		uint32_t mDamage;					//Amount of damage a bullet deals upon contact
		uint32_t mNoBarrels;				//Number of barrels the ship fires at once
		vector <Projectile*> mProjectiles;	//Vector of projectiles that this weapon has fired (clear a projectile when it hits something)
		Timer mFireTimer;
		bool mTryFire;						//Whether or not the weapon should try to fire a round this frame
	};
}