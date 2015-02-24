#include "Scenery.hpp"
using namespace HIC;

// Default constructor for Scenery
Scenery::Scenery(IMesh* mesh, const XMFLOAT3& position, float scale) :
	mModel(mesh->CreateModel(position.x, position.y, position.z)),
	mScale(scale)	
{
	mModel->Scale(mScale);
}

// Destructor for Scenery
Scenery::~Scenery()
{
	mModel->GetMesh()->RemoveModel(mModel);
}

void Scenery::LookAt(ISceneNode* at)
{
	mModel->LookAt(at);
	mModel->Scale(mScale);
}

IModel* Scenery::GetModel()
{
	return mModel;
}