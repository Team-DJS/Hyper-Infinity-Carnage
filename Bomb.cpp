#include "Bomb.hpp"

using namespace HIC;

IMesh* Bomb::MESH = nullptr;

Bomb::Bomb(const XMFLOAT3& position, float lifeTime) :
Pickup(MESH, position, lifeTime)
{

}

Bomb::~Bomb()
{

}

void Bomb::OnPickup(Player* collectingPlayer)
{
	collectingPlayer->GiveBomb();
}