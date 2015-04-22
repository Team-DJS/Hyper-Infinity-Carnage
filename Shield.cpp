#include "Shield.hpp"

using namespace HIC;

IMesh* Shield::MESH = nullptr;

Shield::Shield(const D3DXVECTOR3& position, float radius, float lifeTime) :
Pickup(MESH, position, radius, lifeTime)
{
	
}

Shield::~Shield()
{

}

void Shield::OnPickup(Player* collectingPlayer)
{
	collectingPlayer->GiveShield();
}