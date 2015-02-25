#include "Bomb.hpp"

using namespace HIC;

IMesh* Bomb::MESH = nullptr;

Bomb::Bomb(const XMFLOAT3& position, float radius, float lifeTime) :
	Pickup(MESH, position, radius, lifeTime)
{

}

Bomb::~Bomb()
{

}

void Bomb::OnPickup(Player* collectingPlayer)
{
	collectingPlayer->GiveBomb();
}