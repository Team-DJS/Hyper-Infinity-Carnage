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
WeaponUpgrade::WeaponUpgrade(IMesh* mesh, const XMFLOAT3& position, float radius, float lifeTime, float fireRateUpAmount, float damageUpAmount) :
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
	
}
