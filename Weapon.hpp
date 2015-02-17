#pragma once

#include "Common.hpp"

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
		// Increases the damage of the weapon by the given amount
		void IncreaseDamage(float);

		// Increases the fire rate of the weapon by the given amount
		void IncreaseFireRate(float);

		// Gives the weapon another barrel
		void AddBarrel();

		// Shoots a projectile in a direction
		void Shoot(XMFLOAT3);

		// Called to update the weapon
		void Update();

	private:
		uint32_t mDamage;
		float mFireRate;
		uint32_t mNoBarrels;

	};
}