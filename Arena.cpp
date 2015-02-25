#include "Arena.hpp"
using namespace HIC;

IMesh* Arena::ARENA_MESH = nullptr;

// Default constructor for Arena
Arena::Arena(I3DEngine* engine) :
	mEngine(engine),
	mPlayer(Player(XMFLOAT3(0.0f, 0.0f, 0.0f), 20.0f)),
	mArenaModel(Scenery(ARENA_MESH, XMFLOAT3(0.0f, 0.0f, 0.0f)))
{
	IMesh* temp = engine->LoadMesh("cityScape.x");
	XMFLOAT3 pos;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pos.x = -1170.0f + 1170.0 * j;
			pos.y = -600.0f;
			pos.z = 1170.0f * i;
			Scenery* sceneryTemp = new Scenery(temp, pos, 200.0f);
			mSceneryObjects.push_back(sceneryTemp);
		}		
	}
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

// Updates all the entities inside the arena
void Arena::Update(float frameTime)
{
	
	// Handle player input
	if (mEngine->KeyHeld(Key_W))
	{
		mPlayer.SetMoveForward();
		//mPlayer->MoveZ(50.0f * frameTime);
	}
	if (mEngine->KeyHeld(Key_A))
	{
		mPlayer.SetMoveLeft();
		//mPlayer->MoveX(-50.0f * frameTime);
	}
	if (mEngine->KeyHeld(Key_S))
	{
		mPlayer.SetMoveBackward();
		//mPlayer->MoveZ(-50.0f * frameTime);
	}
	if (mEngine->KeyHeld(Key_D))
	{
		mPlayer.SetMoveRight();
		//mPlayer->MoveX(50.0f * frameTime);
	}

#ifdef _DEBUG
	if (mEngine->KeyHit(Key_M))
	{
		mPlayer.GetCollisionCylinder().ToggleMarkers();
	}
#endif

	// Update the player
	mPlayer.Update(frameTime);

	// Update all the enemies
	for (auto& enemy : mEnemies)
	{
		enemy->Update(frameTime);
	}
}

// Proceeds to the next stage
void Arena::NextStage()
{
}

// Saves the game to be loaded at a later date
void Arena::Save()
{
}

// Removes all entities from the arena
void Arena::Clear()
{
}

void Arena::TargetCamera(ICamera* camera)
{
	camera->LookAt(mPlayer.GetModel() );
}