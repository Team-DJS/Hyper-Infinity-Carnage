#include "Scenery.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Scenery
Scenery::Scenery(IMesh* mesh, const D3DXVECTOR3& position, float scale) :
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

//-----------------------------------
// Setters
//-----------------------------------

//-----------------------------------
// Getters
//-----------------------------------

IModel* Scenery::GetModel()
{
	return mModel;
}

//-----------------------------------
// Other
//-----------------------------------

void Scenery::LookAt(ISceneNode* at)
{
	mModel->LookAt(at);
	mModel->Scale(mScale);
}