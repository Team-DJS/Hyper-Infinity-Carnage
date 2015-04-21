#include "Arena.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

IMesh* Arena::ARENA_MESH = nullptr;
IMesh* Arena::ENEMY_MESH = nullptr;

IFont* HUDFont = nullptr;
#ifdef _DEBUG
tle::IFont* Arena::DebugHUD = nullptr;
#endif

const std::string Arena::SAVE_FILENAME = "Save.hic";
const std::string Arena::HIGH_SCORES_FILENAME = "High_Scores.hic";

const uint32_t MAX_ENEMIES_ON_SCREEN = 30U;
const D3DXVECTOR3 OFF_SCREEN_POS = D3DXVECTOR3(0, 0, -800);

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Arena
Arena::Arena(bool loadFromFile, string name) :
	mPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40.0f),
	mArenaModel(ARENA_MESH, D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	mCollisionBox(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(-450.0f, -450.0f), D3DXVECTOR2(450.0f, 450.0f)),
	mScore(0U),
	mPickupTimer(15.0f),
	mBombExplosionTimer(0.0f),
	mBombCollisionCylinder(D3DXVECTOR2(0.0f, 0.0f), 0.0f),
	mCollisionSwitch(false),
	mBombSwitch(false),
	mPlayerStatus(true),
	mPlayerName(name),
	mMultiplier(1U),
	mKillCount(0U)
{
	// Seed random
	srand((uint32_t)(time(0)));

	// Build the scenery
	IMesh* skyboxMesh = gEngine->LoadMesh("Skybox.x");
	mSceneryObjects.push_back(new Scenery(skyboxMesh, D3DXVECTOR3(0, -2400.0f, 0), 2.0f));

	IMesh* buildingsMesh = gEngine->LoadMesh("cityScape.x");
	//D3DXVECTOR3 pos;
	//for (int i = 0; i < 3; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		pos.x = -1170.0f + 1170.0f * j;
	//		pos.y = -700.0f;
	//		pos.z = 1170.0f * i;
			Scenery* sceneryTemp = new Scenery(buildingsMesh, D3DXVECTOR3(0,-1805,-50), 800.0f);
			mSceneryObjects.push_back(sceneryTemp);
	//	}
	//}

	// Create the gameplay music audio source
	mGameMusic = gAudioManager->CreateSource("GameplayMusic", { 0.0f, 0.0f, 0.0f });
	mGameMusic->SetLooping(true);
	mGameMusic->Play();

	// Load and create the bomb audio source
	gAudioManager->LoadAudio("BombExplosion", "Media\\Audio\\BombExplosion.wav");
	mBombSound = gAudioManager->CreateSource("BombExplosion", { 0.0f, 0.0f, 0.0f });
	mBombSound->SetLooping(false);

	// Load and create the enemy spawn audio source
	gAudioManager->LoadAudio("EnemySpawn", "Media\\Audio\\EnemySpawn.wav");
	mEnemySpawnSound = gAudioManager->CreateSource("EnemySpawn", { 0.0f, 0.0f, 0.0f });
	mEnemySpawnSound->SetLooping(false);


	// Initialise the Mesh for the pickups
	WeaponUpgrade::mMesh = gEngine->LoadMesh("WeaponPickup.x");
	HealthPack::MESH = gEngine->LoadMesh("HealthPickup.x");
	ExtraLife::MESH = gEngine->LoadMesh("LifePickup.x");
	Bomb::MESH = gEngine->LoadMesh("BombPickup.x");

	IMesh* particleMesh = gEngine->LoadMesh("quad_multiplicative.x");
	const float arenaEdge = mCollisionBox.GetMaxOffset().x;
	const float emissionRate = 0.0125f;
	const float lifetime = 2.0f;
	mArenaParticles.push_back(new FountainEmitter(particleMesh, D3DXVECTOR3(arenaEdge,  0,  arenaEdge), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(particleMesh, D3DXVECTOR3(arenaEdge,  0,			0), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(particleMesh, D3DXVECTOR3(arenaEdge,  0, -arenaEdge), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(particleMesh, D3DXVECTOR3(		0,  0,  arenaEdge), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(particleMesh, D3DXVECTOR3(		0,  0, -arenaEdge), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(particleMesh, D3DXVECTOR3(-arenaEdge, 0,  arenaEdge), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(particleMesh, D3DXVECTOR3(-arenaEdge, 0,			0), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(particleMesh, D3DXVECTOR3(-arenaEdge, 0, -arenaEdge), emissionRate, lifetime));

	for (auto iter : mArenaParticles)
	{
		//iter->StartEmission();
		iter->StopEmission();
	}

	SpawnTunnel::SPAWN_TUNNEL_MESH = gEngine->LoadMesh("BombPortal.x");
	//for (int i = 0; i < MAX_ENEMIES_ON_SCREEN * 2; i++)
	//{
	//	mSpawnTunnels.push_back(new SpawnTunnel(OFF_SCREEN_POS));
	//}


	// Load the bomb explosion model
	IMesh* bombMesh = gEngine->LoadMesh("quad_additive.x");
	mBombModel = bombMesh->CreateModel(0.0f, 1.0f, 0.0f);
	mBombModel->RotateLocalX(90.0f);
	mBombSwitch = true;


	// Load the HUD font
	HUDFont = gEngine->LoadFont("Berlin Sans FB Demi", 44);
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

	while (!mArenaParticles.empty())
	{
		delete mArenaParticles.back();
		mArenaParticles.pop_back();
	}
	mBombModel->GetMesh()->RemoveModel(mBombModel);

	while (!mSpawnTunnels.empty())
	{
		delete mSpawnTunnels.back();
		mSpawnTunnels.pop_back();
	}


	mGameMusic->Stop();
	gAudioManager->ReleaseSource(mGameMusic);
	gAudioManager->ReleaseSource(mBombSound);
	gAudioManager->ReleaseSource(mEnemySpawnSound);
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

	if (gEngine->KeyHit(Key_T))
	{
		CreateNewPickup();
	}

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

	mBombExplosionTimer.Update(frameTime);

	if (mBombSwitch && mBombExplosionTimer.IsComplete())
	{
		mBombSwitch = false;
		mBombModel->ResetScale();
		mBombModel->SetPosition(OFF_SCREEN_POS.x, OFF_SCREEN_POS.y, OFF_SCREEN_POS.z);
		mBombSound->Stop();
	}

	if (gEngine->KeyHit(Key_Space) && mBombExplosionTimer.IsComplete() && mPlayer.GetBombs() > 0)
	{
		mBombSwitch = true;
		mBombExplosionTimer.Reset(0.9f);
		mBombCollisionCylinder.SetRadius(0.0);
		mBombCollisionCylinder.SetPosition(D3DXVECTOR2(mPlayer.GetWorldPos().x, mPlayer.GetWorldPos().z));
		mBombModel->SetPosition(mPlayer.GetWorldPos().x, 7.0f, mPlayer.GetWorldPos().z);
		mPlayer.TakeBomb();
		mBombSound->Play();
	}

	// InGame HUD update
	int screenWidth = gEngine->GetWidth();
	int screenHeight = gEngine->GetHeight();
	string hudText = to_string(mCurrentScore);  // Score ( Top Middle )
	HUDFont->Draw(hudText, screenWidth / 2 + 138, 6, kGreen, kRight);
	hudText = "x " + to_string(mMultiplier);
	HUDFont->Draw(hudText, screenWidth / 2 + 152, 25, kGreen, kRight);
	hudText = to_string(mPlayer.GetHealth());	// Health ( Top Left )
	HUDFont->Draw(hudText, 245, 6, kGreen, kRight);
	hudText = to_string(mCurrentStage);			// Stage ( Top Right )
	HUDFont->Draw(hudText, screenWidth - 20, 6, kGreen, kRight);
	hudText = to_string(mPlayer.GetLives());	// Lives ( Bottom Left )
	HUDFont->Draw(hudText, 235, screenHeight - 50, kGreen, kRight);
	hudText = to_string(mPlayer.GetBombs());	// Bombs ( Bottom Right )
	HUDFont->Draw(hudText, screenWidth - 20, screenHeight - 50, kGreen, kRight);


#ifdef _DEBUG
	if (gEngine->KeyHit(Key_M))
	{
		mPlayer.GetCollisionCylinder()->ToggleMarkers();
		for (size_t i = 0; i < mEnemies.size(); i++)
		{
			mEnemies[i]->GetCollisionCylinder()->ToggleMarkers();
		}
		mCollisionBox.ToggleMarkers();
	}

	// turn on and off the particles for the arena
	if (gEngine->KeyHit(Key_J))
	{
		for (auto iter : mArenaParticles)
		{
			iter->StartEmission();
		}
	}
	if (gEngine->KeyHit(Key_K))
	{
		for (auto iter : mArenaParticles)
		{
			iter->StopEmission();
		}
	}

	// Update all arena particle emitter
	for (auto iter : mArenaParticles)
	{
		iter->Update(frameTime);
	}

	//// Debug HUD
	//hudText = "Stage: " + to_string(mCurrentStage);
	//DebugHUD->Draw(hudText, 10, 10, kRed);
	//hudText = "Lives: " + to_string(mPlayer.GetLives());
	//DebugHUD->Draw(hudText, 10, 22, kRed);
	//hudText = "Health: " + to_string(mPlayer.GetHealth());
	//DebugHUD->Draw(hudText, 10, 34, kRed);
	//hudText = "Enemies: " + to_string(mNoOfEnemies + mEnemies.size());
	//DebugHUD->Draw(hudText, 10, 46, kRed);
	//hudText = "Enemy Pool: " + to_string(mEnemyPool.size());
	//DebugHUD->Draw(hudText, 10, 58, kRed);
	//hudText = "Bombs: " + to_string(mPlayer.GetBombs());
	//DebugHUD->Draw(hudText, 10, 70, kRed);
	//
	//hudText = "Score: " + to_string(mCurrentScore);
	//DebugHUD->Draw(hudText, gEngine->GetWidth() - 200, 10, kRed);
	//
	gEngine->SetWindowCaption(to_string(1 / frameTime));

#endif


	D3DXVECTOR3 enitityPos = mPlayer.GetWorldPos();
	// Update the player
	mPlayer.Update(frameTime);

	// check if player is colliding with arena
	if (!CollisionDetect(mPlayer.GetCollisionCylinder(), &mCollisionBox))
	{
		mPlayer.SetPosition(enitityPos);
	}

	// Update all the enemies
	Enemy::SetPlayerPosition(mPlayer.GetWorldPos());

	for (auto& enemy : mEnemies)
	{
		enitityPos = enemy->GetWorldPos();
		enemy->Update(frameTime);

		if (!CollisionDetect(enemy->GetCollisionCylinder(), &mCollisionBox))
		{
			enemy->SetPosition(enitityPos);
		}
	}



	// Collision
	// Bomb Collision Radius increase
	if (mBombSwitch)
	{
		mBombCollisionCylinder.SetRadius(mBombCollisionCylinder.GetRadius() + 300 * frameTime);
		mBombModel->ResetScale();
		mBombModel->Scale(mBombCollisionCylinder.GetRadius() * 0.1f);

		//mBombPhase++;
		//if (mBombPhase > 9)
		//	mBombModel->SetSkin("PlasmaRing_00" + to_string(mBombPhase) + "_tlxadd.tga");
		//else
		//	mBombModel->SetSkin("PlasmaRing_000" + to_string(mBombPhase) + "_tlxadd.tga");
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
		CollisionCylinder* currentEnemy = mEnemies[i]->GetCollisionCylinder();

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
			else if (CollisionDetect(currentEnemy, &currentProjectile)) // Else check if colliding with enemy
			{
				damage = mPlayer.GetWeapon()->GetProjectiles()[j]->GetDamage();
				if (mEnemies[i]->TakeHealth(damage))
				{
					hitEnemy = true;
					mCurrentScore += mEnemies[i]->GetDamage() * mMultiplier;
					mKillCount++;
				}
				mPlayer.GetWeapon()->RemoveProjectile(j);
				j--;
				break;
			}
		}

		// check if player is colliding with enemies
		if (CollisionDetect(currentEnemy, mPlayer.GetCollisionCylinder()))
		{
			mPlayer.TakeHealth(mEnemies[i]->GetDamage());
			mMultiplier = 1;
			mKillCount = 0;
			hitEnemy = true;
		}

		if (mBombSwitch)
		{
			if (CollisionDetect(&mBombCollisionCylinder, currentEnemy))
			{
				hitEnemy = true;
				damage = 100;
				mCurrentScore += mEnemies[i]->GetDamage() / 2;
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

	if (mKillCount > 5)
	{
		mMultiplier++;
		mKillCount = 0;
	}

	// Pickups
	mPickupTimer.Update(frameTime);
	if (mPickupTimer.IsComplete() && mNoOfEnemies > 10)
	{
		CreateNewPickup();
	}

	// Update pickups and check to see if they collide with player or have run out of time
	for (size_t i = 0; i < mPickups.size(); i++)
	{
		bool deletePickup = false;
		mPickups[i]->Update(frameTime);
		if (CollisionDetect(mPickups[i]->GetCollisionCylinder(), mPlayer.GetCollisionCylinder()))
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
		if (mPlayer.GetLives() == 0)
		{
			mPlayerStatus = false;
		}
		else
		{
			mPlayer.TakeLife();
			mMultiplier = 1;
		}
		this->Clear();
		LoadStage(mCurrentStage);
		mCurrentScore = mScore;
	}

	// check if there should be an enemy spawned
	if (mEnemies.size() < MAX_ENEMIES_ON_SCREEN && mNoOfEnemies > 0)
	{
		//while (mEnemies.size() < MAX_ENEMIES_ON_SCREEN && mNoOfEnemies > 0)
		//{
		mNoOfEnemies--;
		SpawnEnemy();
		//}
	}

	// check if there are no more enemies on the field
	if (mEnemies.size() <= 0)
	{
		LoadStage(mCurrentStage + 1);

		mScore = mCurrentScore;
		// Save the game after loading the next state
		SaveToFile();
		SaveHighScores();
	}

	for (uint32_t i = 0; i < mSpawnTunnels.size(); i++)
	{
		if (mSpawnTunnels[i]->Update(frameTime))
		{
			delete mSpawnTunnels[i];
			mSpawnTunnels.erase(mSpawnTunnels.begin() + i);
			i--;
		}
	}
}

// Proceeds to the next stage
void Arena::LoadStage(uint32_t stageNumber)
{
	mPlayer.GetWeapon()->Clear();
	// Get current stage and add one
	mCurrentStage = stageNumber;


	// Determne number of enemies to defeat this stage
	mNoOfEnemies = static_cast<uint32_t>((mCurrentStage + 30U) * 1.5f);

	mBombExplosionTimer.Update(20.0f);
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
	file >> mPlayerName;

	// Read the player data
	file >> mPlayer;

	return true;
}

void Arena::SaveHighScores()
{

	// used for sorting the highscore list
	struct HighScore
	{
		string name;
		string score;
	};

	std::fstream highScoreFile(HIGH_SCORES_FILENAME);	

	if (!highScoreFile.is_open())
	{
		throw std::runtime_error("Failed to open file: " + HIGH_SCORES_FILENAME);
	}

	std::list<HighScore> HighScores;
	bool isNewName = true;
	for (int i = 0; i < 10; i++)
	{
		HighScores.push_back(HighScore());
		string name;
		highScoreFile >> name;
		HighScores.back().name = name;
		if (name != mPlayerName)
		{
			highScoreFile >> HighScores.back().score;
		}
		else
		{
			HighScores.back().name = name;
			string score;
			highScoreFile >> score;
			if (std::stoi(score) > mCurrentScore)
			{
				HighScores.back().score = score;
			}
			else
			{
				HighScores.back().score = to_string(mCurrentScore);
			}
			isNewName = false;
		}
	}

	if (isNewName)
	{
		HighScores.push_back(HighScore());
		HighScores.back().name = mPlayerName;
		HighScores.back().score = to_string(mCurrentScore);
	}
	typedef std::pair<int, int> ipair;
	HighScores.sort([](HighScore const& first, HighScore const& second){return (std::stoi(first.score) > std::stoi(second.score)); });

	std::ofstream file(HIGH_SCORES_FILENAME);

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + HIGH_SCORES_FILENAME);
	}

	for (auto it : HighScores)
	{
		file << it.name << " " << it.score << std::endl;
	}
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
	file << mPlayerName << std::endl;

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

	mBombExplosionTimer.Update(20.0f);
}

void Arena::TargetCamera(ICamera* camera)
{
	camera->LookAt(mPlayer.GetModel());
}

void Arena::SpawnEnemy()
{
	// Play the enemy spawn sound
	mEnemySpawnSound->Play();

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
	} while (CollisionDetect(enemy->GetCollisionCylinder(), &CollisionCylinder(mPlayer.GetCollisionCylinder()->GetPosition(), 150.0f)));

	mSpawnTunnels.push_back(new SpawnTunnel(enemy->GetWorldPos()));
	mSpawnTunnels.back()->SetPosition(enemy->GetWorldPos());

	mEnemies.push_back(enemy);
}

// Creates a pool of enemies (never creates more than double the max on screen)
void Arena::CreateEnemies()
{
	for (uint32_t i = 0; i < MAX_ENEMIES_ON_SCREEN + 3; i++)
	{
		mEnemyPool.push_back(new Enemy(ENEMY_MESH, OFF_SCREEN_POS, 15.0f, 10U));
	}
}

// Create a new pickup with random type
void Arena::CreateNewPickup()
{
	D3DXVECTOR3 position = D3DXVECTOR3(Random(mCollisionBox.GetMinOffset().x + 15, mCollisionBox.GetMaxOffset().x - 15), 10.0f,
									   Random(mCollisionBox.GetMinOffset().y + 15, mCollisionBox.GetMaxOffset().y - 15));
	float lifetime = Random(10.0f, 25.0f);

	float pickupSeed = Random(0.0f, 100.0f);

	if (pickupSeed <= 40.0f)
	{
		//health
		mPickups.push_back(new HealthPack(position, 15.0f, lifetime, 50U));
	}
	else if (pickupSeed <= 70)
	{
		//bomb
		mPickups.push_back(new Bomb(position, 15.0f, lifetime));
	}
	else if (pickupSeed <= 90)
	{
		//weapon upgrade
		mPickups.push_back(new WeaponUpgrade(WeaponUpgrade::mMesh, position, 15.0f, lifetime, 0.02f, static_cast<uint32_t>(Random(1.0f, 10.0f))));
	}
	else
	{
		//life
		mPickups.push_back(new ExtraLife(position, 15.0f, lifetime));
	}

	mPickups.back()->GetModel()->Scale(15.0f);

	mPickupTimer.Reset(Random(8.6f, 12.2f));
}