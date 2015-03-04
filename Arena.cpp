#include "Arena.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

IMesh* Arena::ARENA_MESH = nullptr;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Arena
Arena::Arena() :
mPlayer(Player(XMFLOAT3(0.0f, 0.0f, 0.0f), 40.0f)),
mArenaModel(Scenery(ARENA_MESH, XMFLOAT3(0.0f, 0.0f, 0.0f)))
{
	IMesh* buildingsMesh = gEngine->LoadMesh("cityScape.x");
	XMFLOAT3 pos;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pos.x = -1170.0f + 1170.0f * j;
			pos.y = -600.0f;
			pos.z = 1170.0f * i;
			Scenery* sceneryTemp = new Scenery(buildingsMesh, pos, 200.0f);
			mSceneryObjects.push_back(sceneryTemp);
		}
	}

	IMesh* enemyMesh = gEngine->LoadMesh("Enemy.x");
	Enemy* temp = new Enemy(enemyMesh, XMFLOAT3(0.0f, 10.0f, 120.0f), 10.0f, 10.0f);
	mEnemies.push_back(temp);
}

// Destructor for Arena
Arena::~Arena()
{
	while (!mSceneryObjects.empty())
	{
		delete mSceneryObjects.back();
		mSceneryObjects.pop_back();
	}
}

//-----------------------------------
// Setters
//-----------------------------------

//-----------------------------------
// Getters
//-----------------------------------

//-----------------------------------
// Other
//-----------------------------------

// Updates all the entities inside the arena
void Arena::Update(float frameTime)
{

	// Handle player input
	if (gEngine->KeyHeld(Key_W))
	{
		mPlayer.SetMoveForward();
		//mPlayer->MoveZ(50.0f * frameTime);
	}
	if (gEngine->KeyHeld(Key_A))
	{
		mPlayer.SetMoveLeft();
		//mPlayer->MoveX(-50.0f * frameTime);
	}
	if (gEngine->KeyHeld(Key_S))
	{
		mPlayer.SetMoveBackward();
		//mPlayer->MoveZ(-50.0f * frameTime);
	}
	if (gEngine->KeyHeld(Key_D))
	{
		mPlayer.SetMoveRight();
		//mPlayer->MoveX(50.0f * frameTime);
	}

#ifdef _DEBUG
	if (gEngine->KeyHit(Key_M))
	{
		mPlayer.GetCollisionCylinder().ToggleMarkers();
		for (size_t i = 0; i < mEnemies.size(); i++)
		{
			mEnemies[i]->GetCollisionCylinder().ToggleMarkers();
		}
	}
#endif

	// Update the player
	mPlayer.Update(frameTime);

	// Update all the enemies
	for (auto& enemy : mEnemies)
	{
		enemy->LookAt(mPlayer.GetWorldPos());
		enemy->Update(frameTime);
	}

	// Do collision
	for (int i = 0; i < mEnemies.size(); i++)
	{
		//for (int j = 0; j < mEnemies.size(); j++)
		//{
		if (CylinderToCylinderCollision(&mEnemies[i]->GetCollisionCylinder(), &mPlayer.GetCollisionCylinder()))
		{
			mEnemies[i]->GetModel()->SetSkin("tiles1.jpg");
		}
		//}
	}
}

// Proceeds to the next stage
void Arena::NextStage()
{
	// Get current stage
	// Add one
	// Determne number of enemies to defeat this stage
	// Create that many enemies
}

// Saves the game to be loaded at a later date
void Arena::Save()
{
}

// Removes all entities from the arena
void Arena::Clear()
{
	for (int i = 0; i < mEnemies.size(); i++)
	{
		delete mEnemies[i];
	}
	mEnemies.clear();
}

void Arena::TargetCamera(ICamera* camera)
{
	camera->LookAt(mPlayer.GetModel());
}