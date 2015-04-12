#include "SpawnTunnel.hpp"

using namespace HIC;

IMesh* SpawnTunnel::SPAWN_TUNNEL_MESH = nullptr;

SpawnTunnel::SpawnTunnel(D3DXVECTOR3 pos) :
	mTimer(0.5f)
{
	mModel = SPAWN_TUNNEL_MESH->CreateModel(pos.x,pos.y,pos.z);
	mModel->SetSkin("SpawnTunnel_tlxadd.tga");
	mModel->ScaleY(30.0f);
}


SpawnTunnel::~SpawnTunnel()
{
	mModel->GetMesh()->RemoveModel(mModel);
}

void SpawnTunnel::SetPosition(D3DXVECTOR3 pos)
{
	mModel->SetPosition(pos.x, pos.y, pos.z);
	mSize = 1.0f;
	mTimer.Reset();
}

bool SpawnTunnel::Update(float frameTime)
{
	mTimer.Update(frameTime);
	mSize -= frameTime;
	mModel->ResetScale();
	mModel->ScaleY(30.0f);
	mModel->ScaleX(mSize);
	if (mTimer.IsComplete())
		return true;
	else 
		return false;
}