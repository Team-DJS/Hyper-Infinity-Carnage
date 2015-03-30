#include "ExtraLife.hpp"
using namespace HIC;

IMesh* ExtraLife::MESH = nullptr;

// Default constructor for ExtraLife
ExtraLife::ExtraLife(const D3DXVECTOR3& position, float radius, float lifeTime) :
	Pickup(MESH, position, radius, lifeTime)
{
}

// Destructor for ExtraLife
ExtraLife::~ExtraLife()
{
}

// Called when the pickup is collides with a player
void ExtraLife::OnPickup(Player* collectingPlayer)
{
	collectingPlayer->GiveLife();
}