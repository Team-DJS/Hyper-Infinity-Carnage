#pragma once

#include "Common.hpp"
#include "Projectile.hpp"

namespace HIC
{
	class Weapon
	{
	public:
		// Default constructor for Weapon
		Weapon();

		// Destructor for Weapon
		~Weapon();
	public:
		// Increases the fire rate of the weapon by the given amount
		void IncreaseFireRate(float rateOfFire);

		// Increases the damage of the weapon by the given amount
		void IncreaseDamage(float damage);

		// Gives the weapon another barrel
		void AddBarrel();

		// Shoots a projectile in a direction
		void Shoot(const XMFLOAT3& direction);

		// Called to update the weapon
		void Update(float frameTime);
	private:
		float mFireRate;
		uint32_t mDamage;
		uint32_t mNoBarrels;
		vector <Projectile*> mProjectiles;
	};
}