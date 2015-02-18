#include "HealthPack.hpp"
using namespace HIC;

IMesh* HealthPack::MESH = nullptr;

// Default constructor for HealthPack
HealthPack::HealthPack(const XMFLOAT3& position, float lifeTime, uint32_t restoreAmount) :
	Pickup(MESH, position, lifeTime),
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
