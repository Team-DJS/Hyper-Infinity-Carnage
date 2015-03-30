#include "Arena.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

IMesh* Arena::ARENA_MESH = nullptr;
IMesh* Arena::ENEMY_MESH = nullptr;

#ifdef _DEBUG
	tle::IFont* Arena::DebugHUD = nullptr;
#endif

const std::string Arena::SAVE_FILENAME = "Save.hic";

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Arena
Arena::Arena(bool loadFromFile) :
	mPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40.0f),
	mArenaModel(ARENA_MESH, D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	mCollisionBox(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(-450.0f, -450.0f), D3DXVECTOR2(450.0f, 450.0f)),
	mScore(0U),
	mPickupTimer(5.0f)
{
	// Build the scenery
	IMesh* buildingsMesh = gEngine->LoadMesh("cityScape.x");
	D3DXVECTOR3 pos;
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

	mGameMusic = gAudioManager->CreateSource("GameplayMusic", D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	mGameMusic->SetLooping(true);
	mGameMusic->Play();


	// Initialise the Mesh for the pickups
	WeaponUpgrade::mMesh = gEngine->LoadMesh("CardboardBox.x");
	HealthPack::MESH = gEngine->LoadMesh("CardboardBox.x");
	ExtraLife::MESH = gEngine->LoadMesh("CardboardBox.x");

	IMesh* particleMesh = gEngine->LoadMesh("Sphere.x");
	mArenaParticles = new ParticleEmitter(particleMesh, D3DXVECTOR3(30,40,0), 0.1f, 2.0f);
	mArenaParticles->StartEmission();


#ifdef _DEBUG
	DebugHUD = gEngine->LoadFont("Lucida Console", 12);
#endif

	// Load from the save file
	if (loadFromFile && LoadFromFile())
	{
		LoadStage(mCurrentStage);
	}
	else
	{
		// Load the first stage
		LoadStage(1);
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

	delete(mArenaParticles);

	mGameMusic->Stop();
	gAudioManager->ReleaseSource(mGameMusic);
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

	if (gEngine->KeyHeld(Mouse_LButton))
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

	hudText = "Score: " + to_string(mScore);
	DebugHUD->Draw(hudText, gEngine->GetWidth() - 200, 10, kRed);

#endif

	// Actual HUD update


	D3DXVECTOR3 enitityPos = mPlayer.GetWorldPos();
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

	// Collision

	// check if player is colliding with arena


	// check if projectiles are colliding with arena
	for (uint32_t j = 0; j < mPlayer.GetWeapon()->GetProjectiles().size(); j++)
	{
		if (!CylinderToBoxCollision(&mPlayer.GetWeapon()->GetProjectiles()[j]->GetCollisionObject(), &mCollisionBox))
		{
			mPlayer.GetWeapon()->RemoveProjectile(j);
			j--;
		}
	}

	bool hitEnemy;
	uint32_t damage = 0;
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
				damage = mPlayer.GetWeapon()->GetProjectiles()[j]->GetDamage();
				if (mEnemies[i]->TakeHealth(damage))
				{
					hitEnemy = true;
					mScore += mEnemies[i]->GetDamage();
				}
				mPlayer.GetWeapon()->RemoveProjectile(j);
				j--;
				break;
			}
		}

		// check if player is colliding with enemies
		if (CylinderToCylinderCollision(&currentEnemy, &mPlayer.GetCollisionCylinder()))
		{
			mPlayer.TakeHealth(mEnemies[i]->GetDamage());
			hitEnemy = true;
		}

		if (hitEnemy)
		{
			delete mEnemies[i];
			mEnemies.erase(mEnemies.begin() + i);
			i--;
		}
	}

	// Pickups
	mPickupTimer.Update(frameTime);
	if (mPickupTimer.IsComplete())
	{
		int pickupType = static_cast<int>(Random(0.0f, 3.0f));
		D3DXVECTOR3 position = D3DXVECTOR3(Random(mCollisionBox.GetMinOffset().x + 15, mCollisionBox.GetMaxOffset().x - 15), 7.0f,
			Random(mCollisionBox.GetMinOffset().y + 15, mCollisionBox.GetMaxOffset().y - 15));
		float lifetime = Random(5.0f, 9.2f);

		switch (pickupType)
		{
			case 0:
			{
				mPickups.push_back(new WeaponUpgrade(WeaponUpgrade::mMesh, position, 3.0f, lifetime, Random(0.01,0.1), static_cast<uint32_t>(Random(1.3f, 3.8f))));
				break;
			}
			case 1:
			{
				mPickups.push_back(new HealthPack(position, 3.0f, lifetime, 50U));
				break;
			}
			case 2:
			{
				mPickups.push_back(new ExtraLife(position, 3.0f, lifetime));
				break;
			}
			default: break;
		}
		mPickups.back()->GetModel()->Scale(15.0f);

		mPickupTimer.Reset(Random(8.6f, 12.2f));
	}

	bool deletePickup;
	// Update pickups and check to see if they collide with player or have run out of time
	for (size_t i = 0; i < mPickups.size(); i++)
	{
		deletePickup = false;
		mPickups[i]->Update(frameTime);
		if (CylinderToCylinderCollision(&mPickups[i]->GetCollisionCylinder(), &mPlayer.GetCollisionCylinder()))
		{
			mPickups[i]->OnPickup(&mPlayer);
			deletePickup = true;
		}

		if (mPickups[i]->IsLifetimeComplete() || deletePickup)
		{
			delete mPickups[i];
			mPickups.erase(mPickups.begin() + i);
			i--;
		}
	}

	// check if Players life is 0;
	if (mPlayer.GetHealth() <= 0.0f)
	{
		mPlayer.TakeLife();
		this->Clear();
		LoadStage(mCurrentStage);
	}

	// check if there are no more enemies on the field
	if (mEnemies.size() <= 0)
	{
		LoadStage(mCurrentStage + 1);
	}

	mArenaParticles->Update(frameTime);
}

// Proceeds to the next stage
void Arena::LoadStage(uint32_t stageNumber)
{
	// Save the game before loading the next state
	SaveToFile();

	mPlayer.GetWeapon()->Clear();
	// Get current stage and add one
	mCurrentStage = stageNumber;


	// Determne number of enemies to defeat this stage
	uint32_t noOfEnemies = static_cast<uint32_t>((mCurrentStage + 10U) * 1.5f);

	// Create that many enemies
	SpawnEnemies(noOfEnemies);

	//this->Clear();
}

// Loads the game from the save file
// Returns true if the load file exist, false otherwise
bool Arena::LoadFromFile()
{
	// Open the save file
	std::ifstream file(SAVE_FILENAME);

	if (!file.is_open())
	{
		return false;
	}

	// Read the game data
	file >> mCurrentStage;
	file >> mScore;

	// Read the player data
	file >> mPlayer;

	return true;
}

// Saves the game to be loaded at a later date
void Arena::SaveToFile()
{
	// Open the save file
	std::ofstream file(SAVE_FILENAME);

	// Ensure the file opened successfully
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + SAVE_FILENAME);
	}

	// Save the game data
	file << mCurrentStage << std::endl;
	file << mScore << std::endl;

	// Save the player data
	file << mPlayer << std::endl;
}

// Removes all entities from the arena
void Arena::Clear()
{
	for (uint32_t i = 0; i < mEnemies.size(); i++)
	{
		delete mEnemies[i];
	}
	
	mEnemies.clear();

	for (auto remove : mPickups)
	{
		delete remove;
	}
	mPickups.clear();

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
	for (uint32_t i = 0; i < noOfEnemies; i++)
	{
		mEnemies.push_back(new Enemy(ENEMY_MESH, D3DXVECTOR3(Random(mCollisionBox.GetMinOffset().x + 15, mCollisionBox.GetMaxOffset().x - 15), 7.0f, 
									 Random(mCollisionBox.GetMinOffset().y + 15, mCollisionBox.GetMaxOffset().y - 15)), 15.0f, 10U));
	}
}