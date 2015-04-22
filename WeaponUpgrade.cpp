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
WeaponUpgrade::WeaponUpgrade(IMesh* mesh, const D3DXVECTOR3& position, float radius, float lifeTime) :
	Pickup(mesh, position, radius, lifeTime)
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
	float seed = Random(0.0f, 100.0f);

	if (seed <= 40.0f)
	{
		collectingPlayer->GetWeapon()->SetFireRate(collectingPlayer->GetWeapon()->GetFireRate() + 1.0f);
	}
	else if (seed <= 70.0f)
	{
		collectingPlayer->GetWeapon()->SetDamage(25.0f);
	}
	else
	{
		collectingPlayer->GetWeapon()->AddBarrel();
	}
	
}
