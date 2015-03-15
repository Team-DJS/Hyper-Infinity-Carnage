#include "Arena.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

IMesh* Arena::ARENA_MESH = nullptr;
IMesh* Arena::ENEMY_MESH = nullptr;

#ifdef _DEBUG
	IFont* Arena::DebugHUD = nullptr;
#endif

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Arena
Arena::Arena() :
	mPlayer(Player(XMFLOAT3(0.0f, 0.0f, 0.0f), 40.0f)),
	mArenaModel(Scenery(ARENA_MESH, XMFLOAT3(0.0f, 0.0f, 0.0f))),
	mCollisionBox(CollisionAABB(XMFLOAT2(0.0f, 0.0f), XMFLOAT2(-450.0f, -450.0f), XMFLOAT2(450.0f, 450.0f)))
{
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
	mGameMusic->SetLooping(true);
	mGameMusic->Play();

#ifdef _DEBUG
	DebugHUD = gEngine->LoadFont("Lucida Console", 12);
#endif

	// Load the first stage
	LoadStage(1);
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

	if (gEngine->KeyHit(Mouse_LButton))
	{
		mPlayer.SetTryFire();
	}

#ifdef _DEBUG
	if (gEngine->KeyHit(Key_M))
	{
		mPlayer.GetCollisionCylinder().ToggleMarkers();
		for (size_t i = 0; i < mEnemies.size(); i++)
		{
			mEnemies[i]->GetCollisionCylinder().ToggleMarkers();
		}
		mCollisionBox.ToggleMarkers();
	}

	// Debug HUD
	string hudText = "Stage: " + to_string(mCurrentStage);
	DebugHUD->Draw(hudText, 10, 10, kRed);
	hudText = "Lives: " + to_string(mPlayer.GetLives());
	DebugHUD->Draw(hudText, 10, 22, kRed);
	hudText = "Health: " + to_string(mPlayer.GetHealth());
	DebugHUD->Draw(hudText, 10, 34, kRed);
	hudText = "Enemies: " + to_string(mEnemies.size());
	DebugHUD->Draw(hudText, 10, 46, kRed);
	hudText = "Projectiles: " + to_string(mPlayer.GetWeapon()->GetProjectiles().size());
	DebugHUD->Draw(hudText, 10, 58, kRed);



#endif
	XMFLOAT3 enitityPos = mPlayer.GetWorldPos();
	// Update the player
	mPlayer.Update(frameTime);

	if (!CylinderToBoxCollision(&mPlayer.GetCollisionCylinder(), &mCollisionBox))
	{
		mPlayer.SetPosition(enitityPos);
	}


	// Update all the enemies
	for (auto& enemy : mEnemies)
	{
		enitityPos = enemy->GetWorldPos();
		enemy->LookAt(mPlayer.GetWorldPos());
		enemy->Update(frameTime);

		if (!CylinderToBoxCollision(&enemy->GetCollisionCylinder(), &mCollisionBox))
		{
			enemy->SetPosition(enitityPos);
		}
	}

	bool hitEnemy;
	// Check enemy - player collision
	for (uint32_t i = 0; i < mEnemies.size(); i++)
	{
		hitEnemy = false;
		// get a temp cylinder so you dont need to get it again for each collision
		CollisionCylinder currentEnemy = mEnemies[i]->GetCollisionCylinder();

		// Check whether colliding with a projectile
		for (uint32_t j = 0; j < mPlayer.GetWeapon()->GetProjectiles().size(); j++)
		{
			if (CylinderToCylinderCollision(&currentEnemy, &mPlayer.GetWeapon()->GetProjectiles()[j]->GetCollisionObject()))
			{
				hitEnemy = true;
				break;
			}
		}

		if (CylinderToCylinderCollision(&currentEnemy, &mPlayer.GetCollisionCylinder()))
		{
			mPlayer.TakeHealth(mEnemies[i]->GetDamage());
			hitEnemy = true;
		}
		//}

		if (hitEnemy)
		{
			delete mEnemies[i];
			mEnemies.erase(mEnemies.begin() + i);
			i--;
		}
	}

	if (mPlayer.GetHealth() <= 0.0f)
	{
		this->Clear();
		LoadStage(mCurrentStage);
	}

	if (mEnemies.size() == 0)
	{
		LoadStage(mCurrentStage + 1);
	}
}

// Proceeds to the next stage
void Arena::LoadStage(uint32_t stageNumber)
{
	mPlayer.GetWeapon()->Clear();
	// Get current stage and add one
	mCurrentStage = stageNumber;

	// Determne number of enemies to defeat this stage
	uint32_t noOfEnemies = (mCurrentStage + 10) * 1.5;

	// Create that many enemies
	SpawnEnemies(noOfEnemies);

	//this->Clear();
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

}

void Arena::TargetCamera(ICamera* camera)
{
	camera->LookAt(mPlayer.GetModel());
}

void Arena::SpawnEnemies(uint32_t noOfEnemies)
{
	srand((uint32_t)(time(0)));
	for (int i = 0; i < noOfEnemies; i++)
	{
		mEnemies.push_back(new Enemy(ENEMY_MESH, XMFLOAT3(Random(mCollisionBox.GetMinOffset().x + 15, mCollisionBox.GetMaxOffset().x - 15), 7.0f, Random(mCollisionBox.GetMinOffset().y + 15, mCollisionBox.GetMaxOffset().y - 15 )), 15.0f, 10));
	}
}