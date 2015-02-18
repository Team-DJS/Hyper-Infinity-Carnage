#include "ExtraLife.hpp"

using namespace HIC;

IMesh* ExtraLife::MESH = nullptr;

ExtraLife::ExtraLife(const XMFLOAT3& position, float lifeTime) :
	Pickup(MESH, position, lifeTime)
{

}

ExtraLife::~ExtraLife()
{

}

void ExtraLife::OnPickup(Player* collectingPlayer)
{

}