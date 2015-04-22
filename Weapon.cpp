#include "Weapon.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

const uint32_t MAX_PROJECTILES = 30U;
float MAX_FIRE_RATE = 0.1f;

AudioSource* Weapon::SHOOT_SOUND = nullptr;

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
	for (uint32_t i = 0; i < MAX_PROJECTILES; i++)
	{
		mProjectilePool.push_back(new Projectile(D3DXVECTOR3(0, 0, -800), D3DXVECTOR2(0, 0)));
	}
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
	mFireRate -= bulletsPerSecond;
	if (mFireRate < MAX_FIRE_RATE)
	{
		mFireRate = MAX_FIRE_RATE;
	}
	mFireTimer.Reset(mFireRate * mNoBarrels);
}

// Increases the damage of the weapon by the given amount
void Weapon::SetDamage(uint32_t damage)
{
	mDamage += damage;
}

// Gives the weapon another barrel
void Weapon::AddBarrel()
{
	if (mNoBarrels < 26)
		mNoBarrels++;
	
	MAX_FIRE_RATE -= 0.01f;
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
	return mFireRate;
}

//Returns the damage that a new projectile will deliver upon collision
uint32_t Weapon::GetDamage()
{
	return mDamage;
}


deque<Projectile*> Weapon::GetProjectiles()
{
	return mProjectiles;
}

//-----------------------------------
// Other
//-----------------------------------

// Called to update the weapon
void Weapon::Update(float frameTime, const D3DXVECTOR3 playerPosition, const D3DXVECTOR3 playerFacingVector)
{
	//Update fire timer
	mFireTimer.Update(frameTime);

	//Check if trying to fire this frame
	if (mTryFire && mFireTimer.IsComplete())
	{
		Shoot(playerPosition, playerFacingVector);
	}

	//Update all projectiles
	for (size_t i = 0; i < mProjectiles.size(); i++)
	{
		mProjectiles[i]->Update(frameTime);
	}

	//Unset tryToFire boolean
	mTryFire = false;
}

void Weapon::RemoveProjectile(uint32_t i)
{
	mProjectiles[i]->SetPosOffscreen();
	mProjectilePool.push_back(mProjectiles[i]);
	mProjectiles.erase(mProjectiles.begin() + i);
}

void Weapon::Clear()
{
	for (uint32_t i = 0; i < mProjectiles.size(); i++)
	{
		RemoveProjectile(i);
		i--;
	}
	mProjectiles.clear();
}

// Shoots a new projectile in a direction
void Weapon::Shoot(const D3DXVECTOR3& playerPosition, const D3DXVECTOR3 playerFacingVector)
{
	// Create new projectiles moving in the provided direction (use an angle offset for additional barrels)
	bool spread = false;
	for (auto i = 0U; i < mNoBarrels; i++)
	{
		Projectile* projectile = nullptr;
		if (!mProjectilePool.empty())
		{
			projectile = mProjectilePool.back();
			mProjectilePool.pop_back();
		}
		else
		{
			projectile = mProjectiles.front();
			mProjectiles.pop_front();
		}
		// Set the projectile data
		projectile->SetPos(playerPosition);
		float angleX;
		if (i == 0)
		{
			angleX = 0.0f;
		}
		else if (!spread)
		{
			angleX = i * 3.0f * 0.0174532925f;
			spread = !spread;
		}
		else
		{
			angleX = -((i - 1) * 3.0f * 0.0174532925f);
			spread = !spread;
		}

		D3DXVECTOR2 newVelocity = { (playerFacingVector.x * cos(angleX)) - (playerFacingVector.z * sin(angleX)), 
									(playerFacingVector.x * sin(angleX)) + (playerFacingVector.z * cos(angleX)) };

		D3DXVec2Normalize(&newVelocity,&newVelocity);

		projectile->SetVelocity({ newVelocity.x * -8.0f,
								  newVelocity.y * -8.0f });
		projectile->SetDamage(mDamage);
		mProjectiles.push_back(projectile);
	}

	mFireTimer.Reset(mFireRate * mNoBarrels);

	// Play the shooting audio clip
	SHOOT_SOUND->Play();
}