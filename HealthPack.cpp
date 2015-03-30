#include "HealthPack.hpp"
using namespace HIC;

IMesh* HealthPack::MESH = nullptr;

// Default constructor for HealthPack
HealthPack::HealthPack(const D3DXVECTOR3& position, float radius, float lifeTime, uint32_t restoreAmount) :
	Pickup(MESH, position, radius, lifeTime ),
	mHealth(restoreAmount)
{
}

// Destructor for HealthPack
HealthPack::~HealthPack()
{
}

// Called when the pickup is collides with a player
void HealthPack::OnPickup(Player* collectingPlayer)
{
	collectingPlayer->GiveHealth(mHealth);
}
