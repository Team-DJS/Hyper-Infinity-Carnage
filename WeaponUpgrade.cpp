#include "WeaponUpgrade.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

IMesh* WeaponUpgrade::mMesh = nullptr;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Constructor for WeaponUpgrade
WeaponUpgrade::WeaponUpgrade(IMesh* mesh, const D3DXVECTOR3& position, float radius, float lifeTime, float fireRateUpAmount, uint32_t damageUpAmount) :
	Pickup(mesh, position, radius, lifeTime),
	mFireRate(fireRateUpAmount),
	mDamage(damageUpAmount)
{
}

// Destructor for WeaponUpgrade
WeaponUpgrade::~WeaponUpgrade()
{

}

//-----------------------------------
// Setters
//-----------------------------------

//-----------------------------------
// Getters
//-----------------------------------

//-----------------------------------
// Other
//-----------------------------------

// Called when the pickup is collides with a player
void WeaponUpgrade::OnPickup(Player* collectingPlayer)
{
	if (Random(0.0f, 100.0f) > 87.0f)
	{
		collectingPlayer->GetWeapon()->AddBarrel();
		if (mFireRate > 0.0f)
			collectingPlayer->GetWeapon()->SetFireRate(mFireRate);
	}
	else
	{
	if (mFireRate > 0.0f)
		collectingPlayer->GetWeapon()->SetFireRate(mFireRate);
	}

	if (mDamage > 0U)
		collectingPlayer->GetWeapon()->SetDamage(mDamage);


}
