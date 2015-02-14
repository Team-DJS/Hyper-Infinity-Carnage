#include "Scenery.hpp"
using namespace HIC;

// Default constructor for Scenery
Scenery::Scenery(IMesh* mesh, const XMFLOAT3& position) :
	mModel(mesh->CreateModel(position.x, position.y, position.z))
{
}

Scenery::~Scenery()
{
	mModel->GetMesh()->RemoveModel(mModel);
}