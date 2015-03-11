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
	mTryFire(false),
	mFireTimer(fireRate)
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


vector<Projectile*> Weapon::GetProjectiles()
{
	return mProjectiles;
}

//-----------------------------------
// Other
//-----------------------------------

// Called to update the weapon
void Weapon::Update(float frameTime, XMFLOAT3 playerPosition, XMFLOAT3 playerFacingVector)
{
	//**%**

	//Update fire timer
	mFireTimer.Update(frameTime);

	//Check if trying to fire this frame
	if (mTryFire)
	{		//- Perform fire action if timer allows
		if (mFireTimer.IsComplete())
		{
			Projectile* newProjectile = new Projectile(playerPosition, XMFLOAT2(-playerFacingVector.x * 8, -playerFacingVector.z * 8), mDamage);
			mProjectiles.push_back(newProjectile);
			mFireTimer.Reset();
		}
	}


	//Update all projectiles
	for (size_t i = 0; i < mProjectiles.size(); i++)
	{
		mProjectiles[i]->Update(frameTime);
	}

	//Unset tryToFire boolean
	mTryFire = false;
}

void Weapon::Clear()
{
	for (uint32_t i = 0; i < mProjectiles.size(); i++)
	{
		delete mProjectiles[i];
	}
	mProjectiles.clear();
}

// Shoots a new projectile in a direction
void Weapon::Shoot(const XMFLOAT3& direction)
{
	//**%**
	//Create new projectiles moving in the provided direction (use an angle offset for additional barrels)
}