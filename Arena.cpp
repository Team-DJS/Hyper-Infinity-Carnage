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

const uint32_t MAX_ENEMIES_ON_SCREEN = 30U;
const D3DXVECTOR3 OFF_SCREEN_POS = D3DXVECTOR3(0, 0, -800);

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Arena
Arena::Arena(bool loadFromFile) :
	mPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40.0f),
	mArenaModel(ARENA_MESH, D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	mCollisionBox(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(-450.0f, -450.0f), D3DXVECTOR2(450.0f, 450.0f)),
	mScore(0U),
	mPickupTimer(5.0f),
	mBombExplosionTimer(0.0f),
	mBombCollisionCylinder(D3DXVECTOR2(0.0f,0.0f),0.0f)
{
	// Seed random
	srand((uint32_t)(time(0)));

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
	Bomb::MESH = gEngine->LoadMesh("CardBoardBox.x");

	IMesh* particleMesh = gEngine->LoadMesh("Portal.x");
	mArenaParticles = new ExplosionEmitter(particleMesh, D3DXVECTOR3(30,40,0), 0.1f, 2.0f);
	//mArenaParticles->StartEmission();
	mArenaParticles->StopEmission();

	IMesh* bombMesh = gEngine->LoadMesh("Portal.x");
	mBombModel = bombMesh->CreateModel();
	mBombModel->RotateLocalX(90.0f);
	mBombModel->SetSkin("Fire_tlxmul2.jpg");

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
	mCurrentScore = mScore;

	CreateEnemies();
}

// Destructor for Arena
Arena::~Arena()
{
	while (!mSceneryObjects.empty())
	{
		delete mSceneryObjects.back();
		mSceneryObjects.pop_back();
	}

	while (!mEnemies.empty())
	{
		delete mEnemies.back();
		mEnemies.pop_back();
	}

	while (!mEnemyPool.empty())
	{
		delete mEnemyPool.back();
		mEnemyPool.pop_back();
	}

	delete(mArenaParticles);
	mBombModel->GetMesh()->RemoveModel(mBombModel);

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

	if (gEngine->KeyHit(Key_J))
	{
		mArenaParticles->StartEmission();
	}
	if (gEngine->KeyHit(Key_K))
	{
		mArenaParticles->StopEmission();
	}

	mBombExplosionTimer.Update(frameTime);

	if (mBombExplosionTimer.IsComplete())
	{
		mBombSwitch = false;
		mBombModel->ResetScale();
		mBombModel->SetPosition(OFF_SCREEN_POS.x, OFF_SCREEN_POS.y, OFF_SCREEN_POS.z);
	}

	if (gEngine->KeyHit(Key_Space) && mBombExplosionTimer.IsComplete() && mPlayer.GetBombs() > 0)
	{
		mBombSwitch = true;
		mBombExplosionTimer.Reset(3.0f);
		mBombCollisionCylinder.SetRadius(0.0);
		mBombCollisionCylinder.SetPosition(D3DXVECTOR2(mPlayer.GetWorldPos().x, mPlayer.GetWorldPos().z));
		mBombModel->SetPosition(mPlayer.GetWorldPos().x,7.0f, mPlayer.GetWorldPos().z);
		mPlayer.TakeBomb();
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
	hudText = "Enemy Pool: " + to_string(mEnemyPool.size());
	DebugHUD->Draw(hudText, 10, 58, kRed);
	hudText = "Bombs: " + to_string(mPlayer.GetBombs());
	DebugHUD->Draw(hudText, 10, 70, kRed);

	hudText = "Score: " + to_string(mCurrentScore);
	DebugHUD->Draw(hudText, gEngine->GetWidth() - 200, 10, kRed);

#endif

	// Actual HUD update


	D3DXVECTOR3 enitityPos = mPlayer.GetWorldPos();
	// Update the player
	mPlayer.Update(frameTime);

	// check if player is colliding with arena
	if (!CollisionDetect(&mPlayer.GetCollisionCylinder(), &mCollisionBox))
	{
		mPlayer.SetPosition(enitityPos);
	}

	// Update all the enemies
	Enemy::SetPlayerPosition(mPlayer.GetWorldPos());

	for (auto& enemy : mEnemies)
	{
		enitityPos = enemy->GetWorldPos();
		enemy->Update(frameTime);

		if (!CollisionDetect(&enemy->GetCollisionCylinder(), &mCollisionBox))
		{
			enemy->SetPosition(enitityPos);
		}
	}

	

	// Collision
	// Bomb Collision Radius increase
	if (mBombSwitch)
	{
		mBombCollisionCylinder.SetRadius(mBombCollisionCylinder.GetRadius() + 100 * frameTime);
		mBombModel->ResetScale();
		mBombModel->Scale(mBombCollisionCylinder.GetRadius() * 0.07);
	}

	// Check which enemies to do collision for
	// True is the second half, false is the first half
	uint32_t end;
	uint32_t beginning;
	mCollisionSwitch = !mCollisionSwitch;
	if (mCollisionSwitch)
	{
		beginning = mEnemies.size() / 2;
		end = mEnemies.size();
	}
	else
	{
		beginning = 0;
		end = mEnemies.size() / 2;
	}
	bool hitEnemy;
	uint32_t damage = 0;
	// Check enemy - player collision
	for (uint32_t i = beginning; i < end; i++)
	{
		hitEnemy = false;
		// get a temp cylinder so you dont need to get it again for each collision
		CollisionCylinder currentEnemy = mEnemies[i]->GetCollisionCylinder();

		// Check whether colliding with a projectile
		for (uint32_t j = 0; j < mPlayer.GetWeapon()->GetProjectiles().size(); j++)
		{
			// temp cylinder for the projectile
			CollisionCylinder currentProjectile = mPlayer.GetWeapon()->GetProjectiles()[j]->GetCollisionObject();

			// check if projectiles are colliding with arena
			if (!CollisionDetect(&currentProjectile, &mCollisionBox))
			{
				mPlayer.GetWeapon()->RemoveProjectile(j);
				j--;
			}
			else if (CollisionDetect(&currentEnemy, &currentProjectile)) // Else check if colliding with enemy
			{
				damage = mPlayer.GetWeapon()->GetProjectiles()[j]->GetDamage();
				if (mEnemies[i]->TakeHealth(damage))
				{
					hitEnemy = true;
					mCurrentScore += mEnemies[i]->GetDamage();
				}
				mPlayer.GetWeapon()->RemoveProjectile(j);
				j--;
				break;
			}
		}

		// check if player is colliding with enemies
		if (CollisionDetect(&currentEnemy, &mPlayer.GetCollisionCylinder()))
		{
			mPlayer.TakeHealth(mEnemies[i]->GetDamage());
			hitEnemy = true;
		}

		if (mBombSwitch)
		{
			if (CollisionDetect(&mBombCollisionCylinder, &currentEnemy))
			{
				hitEnemy = true;
				damage = 100;
			}
		}

		// ENEMY IS HIT
		if (hitEnemy)
		{
			mEnemies[i]->SetPosition(OFF_SCREEN_POS);
			mEnemyPool.push_back(mEnemies[i]);
			mEnemies.erase(mEnemies.begin() + i);
			i--;
		}
		
		if (mCollisionSwitch)
		{
			end = mEnemies.size();
		}
		else
		{
			end = mEnemies.size() / 2;
		}
	}

	// Pickups
	mPickupTimer.Update(frameTime);
	if (mPickupTimer.IsComplete() && mNoOfEnemies > MAX_ENEMIES_ON_SCREEN)
	{
		CreateNewPickup();
	}

	// Update pickups and check to see if they collide with player or have run out of time
	for (size_t i = 0; i < mPickups.size(); i++)
	{
		bool deletePickup = false;
		mPickups[i]->Update(frameTime);
		if (CollisionDetect(&mPickups[i]->GetCollisionCylinder(), &mPlayer.GetCollisionCylinder()))
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
		mCurrentScore = mScore;
	}

	// check if there should be an enemy spawned
	if (mEnemies.size() < MAX_ENEMIES_ON_SCREEN)
	{
		while (mEnemies.size() < MAX_ENEMIES_ON_SCREEN && mNoOfEnemies > 0)
		{
			mNoOfEnemies--;
			SpawnEnemy();
		}
	}

	// check if there are no more enemies on the field
	if (mEnemies.size() <= 0)
	{
		LoadStage(mCurrentStage + 1);

		mScore = mCurrentScore;
		// Save the game after loading the next state
		SaveToFile();
	}

	mArenaParticles->Update(frameTime);
}

// Proceeds to the next stage
void Arena::LoadStage(uint32_t stageNumber)
{
	mPlayer.GetWeapon()->Clear();
	// Get current stage and add one
	mCurrentStage = stageNumber;


	// Determne number of enemies to defeat this stage
	mNoOfEnemies = static_cast<uint32_t>((mCurrentStage + 10U) * 1.5f);

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
		mEnemyPool.push_back(mEnemies[i]);
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

void Arena::SpawnEnemy()
{
	Enemy* enemy = mEnemyPool.back();
	mEnemyPool.pop_back();

	enemy->ResetHealth();

	do
	{
		D3DXVECTOR3 newPosition;
		newPosition.x = Random(mCollisionBox.GetMinOffset().x + 15, mCollisionBox.GetMaxOffset().x - 15);
		newPosition.y = 7.0f;
		newPosition.z = Random(mCollisionBox.GetMinOffset().y + 15, mCollisionBox.GetMaxOffset().y - 15);

		enemy->SetPosition(newPosition);
	} while (CollisionDetect(&enemy->GetCollisionCylinder(), &CollisionCylinder(mPlayer.GetCollisionCylinder().GetPosition(), 150.0f)));

	mEnemies.push_back(enemy);
}

// Creates a pool of enemies (never creates more than double the max on screen)
void Arena::CreateEnemies()
{
	for (uint32_t i = 0; i < MAX_ENEMIES_ON_SCREEN * 2; i++)
	{
		mEnemyPool.push_back(new Enemy(ENEMY_MESH, OFF_SCREEN_POS, 15.0f, 10U));
	}
}

// Create a new pickup with random type
void Arena::CreateNewPickup()
{
	int pickupType = static_cast<int>(Random(0.0f, 4.0f));
	D3DXVECTOR3 position = D3DXVECTOR3(Random(mCollisionBox.GetMinOffset().x + 15, mCollisionBox.GetMaxOffset().x - 15), 7.0f,
		Random(mCollisionBox.GetMinOffset().y + 15, mCollisionBox.GetMaxOffset().y - 15));
	float lifetime = Random(5.0f, 9.2f);

	switch (pickupType)
	{
	case 0:
	{
		mPickups.push_back(new WeaponUpgrade(WeaponUpgrade::mMesh, position, 3.0f, lifetime, Random(0.01, 0.1), static_cast<uint32_t>(Random(1.3f, 3.8f))));
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
	case 3:
	{
		mPickups.push_back(new Bomb(position, 3.0f, lifetime));
		break;
	}
	default: break;
	}
	mPickups.back()->GetModel()->Scale(15.0f);

	mPickupTimer.Reset(Random(8.6f, 12.2f));
}