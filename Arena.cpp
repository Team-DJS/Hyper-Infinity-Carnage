#include "Arena.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

IMesh* Arena::ARENA_MESH = nullptr;
IMesh* Arena::ENEMY_MESH = nullptr;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Arena
Arena::Arena() :
	mPlayer(Player(XMFLOAT3(0.0f, 20.0f, 0.0f))),
	mArenaModel(Scenery(ARENA_MESH, XMFLOAT3(0.0f, 0.0f, 0.0f))),
	mCollisionBox(CollisionAABB(XMFLOAT2(0.0f, 0.0f), XMFLOAT2(-450.0f, -450.0f), XMFLOAT2(450.0f, 450.0f)))
{
	gPhysicsManager->CreateRigidBody(mArenaModel.GetModel(), 1000);

	IMesh* buildingsMesh = gEngine->LoadMesh("cityScape.x");
	XMFLOAT3 pos;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pos.x = -1170.0f + 1170.0f * j;
			pos.y = -700.0f;
			pos.z = 1170.0f * i;
			Scenery* sceneryTemp = new Scenery(buildingsMesh, pos, 200.0f);
			mSceneryObjects.push_back(sceneryTemp);
		}
	}

	gAudioManager->LoadAudio("GameplayMusic", "Media\\GameplayTheme.wav");
	mGameMusic = gAudioManager->CreateSource("GameplayMusic", XMFLOAT3(0.0f, 0.0f, 0.0f));
	mGameMusic->Play();

	SpawnEnemies();
}

// Destructor for Arena
Arena::~Arena()
{
	while (!mSceneryObjects.empty())
	{
		delete mSceneryObjects.back();
		mSceneryObjects.pop_back();
	}
	mGameMusic->Stop();
	gAudioManager->ReleaseSource(mGameMusic);
	gAudioManager->ReleaseAudio("GameplayMusic");
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
	}
	if (gEngine->KeyHeld(Key_A))
	{
		mPlayer.SetMoveLeft();
	}
	if (gEngine->KeyHeld(Key_S))
	{
		mPlayer.SetMoveBackward();
	}
	if (gEngine->KeyHeld(Key_D))
	{
		mPlayer.SetMoveRight();
	}

	if (gEngine->KeyHit(Mouse_LButton))
	{
		mPlayer.SetTryFire();
	}

	//mPlayer.SetPhysicsBodyToModel();
	for (uint32_t i = 0; i < mEnemies.size(); i++)
	{
		//mEnemies[i]->SetPhysicsBodyToModel();
	}
	//Update the physics world
	gPhysicsManager->Update(frameTime);

	// Update the player
	mPlayer.Update(frameTime);

	// Update all the enemies
	for (auto& enemy : mEnemies)
	{
		enemy->LookAt(mPlayer.GetWorldPos());
		enemy->Update(frameTime);

	}

	if (mPlayer.GetHealth() <= 0.0f)
	{
		this->Clear();
	}

	if (mEnemies.size() <= 0)
	{
		NextStage();
	}
}

// Proceeds to the next stage
void Arena::NextStage()
{
	// Get current stage
	// Add one
	// Determne number of enemies to defeat this stage
	// Create that many enemies

	this->Clear();
}

// Saves the game to be loaded at a later date
void Arena::Save()
{
}

// Removes all entities from the arena
void Arena::Clear()
{
	for (uint32_t i = 0; i < mEnemies.size(); i++)
	{
		delete mEnemies[i];
	}
	mEnemies.clear();
	mPlayer.Clear();
	mPlayer.Respawn();
	SpawnEnemies();

}

void Arena::TargetCamera(ICamera* camera)
{
	camera->LookAt(mPlayer.GetModel());
}

void Arena::SpawnEnemies()
{
	srand((uint32_t)(time(0)));
	for (uint32_t i = 0; i < 20; i++)
	{
		mEnemies.push_back(new Enemy(ENEMY_MESH, XMFLOAT3(Random(mCollisionBox.GetMinOffset().x + 15, mCollisionBox.GetMaxOffset().x - 15), 7.0f, Random(mCollisionBox.GetMinOffset().y + 15, mCollisionBox.GetMaxOffset().y - 15 )), 10, 250.0f));
	}

}