#include "Shield.hpp"

using namespace HIC;

Shield::Shield(const D3DXVECTOR3& position, float radius, float lifeTime) :
Pickup(Player::SHIELD, position, radius, lifeTime)
{

}

Shield::~Shield()
{

}

void Shield::OnPickup(Player* collectingPlayer)
{
	collectingPlayer->GiveShield();
}