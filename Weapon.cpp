#include "Weapon.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Constructor for Weapon
Weapon::Weapon(float fireRate, uint32_t damage, uint32_t noBarrels) :
	mFireRate(fireRate),
	mDamage(damage),
	mNoBarrels(noBarrels),
	mTryFire(false)
{
}

// Destructor for Weapon
Weapon::~Weapon()
{
	while (!mProjectiles.empty())		//Deallocate Memory for and pop projectiles from vector
	{
		delete mProjectiles.back();
		mProjectiles.pop_back();
	}
}

//-----------------------------------
// Setters
//-----------------------------------

// Increases the fire rate of the weapon by the given amount - (make the bullets faster)
void Weapon::SetFireRate(float bulletsPerSecond)
{
	mFireRate = 1 / bulletsPerSecond;
}

// Increases the damage of the weapon by the given amount
void Weapon::SetDamage(uint32_t damage)
{
	mDamage = damage;
}

// Gives the weapon another barrel
void Weapon::AddBarrel()
{
	mNoBarrels++;
}

//Sets whether or not the weapon should try to fire this frame (will decide to fire based on fire rate)
void Weapon::SetFire()
{
	mTryFire = true;
}

//-----------------------------------
// Getters
//-----------------------------------

//Returns the number of bullets fired per second
float Weapon::GetFireRate()
{
	return 1 / mFireRate;
}

//Returns the damage that a new projectile will deliver upon collision
uint32_t Weapon::GetDamage()
{
	return mDamage;
}

//-----------------------------------
// Other
//-----------------------------------

// Called to update the weapon
void Weapon::Update(float frameTime)
{
	//**%**

	//Update fire timer

	//Check if trying to fire this frame
		//- Perform fire action if timer allows


	//Update all projectiles
	for (int i = 0; i < mProjectiles.size(); i++)
	{
		mProjectiles[i]->Update(frameTime);
	}

	//Unset tryToFire boolean
	mTryFire = false;
}

// Shoots a new projectile in a direction
void Weapon::Shoot(const XMFLOAT3& direction)
{
	//**%**
	//Create new projectiles moving in the provided direction (use an angle offset for additional barrels)
}