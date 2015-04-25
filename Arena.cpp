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
mPickupTimer(10.0f),
mBombExplosionTimer(0.0f),
mBombCollisionCylinder(D3DXVECTOR2(0.0f, 0.0f), 0.0f),
mBombSwitch(false),
mPlayerStatus(true),
mPlayerName(name),
mMultiplier(1U),
mKillCount(0U),
mNoPickupsThisRound(0)
{
	// Seed random
	srand((uint32_t)(time(0)));

	// Build the scenery
	IMesh* skyboxMesh = gEngine->LoadMesh("Skybox.x");
	mStaticScenery.push_back(new Scenery(skyboxMesh, D3DXVECTOR3(0, -1700.0f, 0), 2.1f));
	mStaticScenery.back()->GetModel()->ScaleY(0.8);

	IMesh* buildingsMesh = gEngine->LoadMesh("cityScape.x");
	mStaticScenery.push_back(new Scenery(buildingsMesh, D3DXVECTOR3(0, -1805, -50), 800.0f));

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

	gAudioManager->LoadAudio("EnemyDestroyed", "Media\\Audio\\EnemyDestroyed.wav");
	mEnemyDestroyedSound = gAudioManager->CreateSource("EnemyDestroyed", { 0.0f, 0.0f, 0.0f });
	mEnemyDestroyedSound->SetLooping(false);

	// Initialise the Mesh for the pickups
	WeaponUpgrade::mMesh = gEngine->LoadMesh("WeaponPickup.x");
	HealthPack::MESH = gEngine->LoadMesh("HealthPickup.x");
	ExtraLife::MESH = gEngine->LoadMesh("LifePickup.x");
	Bomb::MESH = gEngine->LoadMesh("BombPickup.x");

	mExplosionMesh = gEngine->LoadMesh("ExplosionQuad.x");
	const float arenaEdge = mCollisionBox.GetMaxOffset().x;
	const float emissionRate = 0.0125f;
	const float lifetime = 2.0f;
	mArenaParticles.push_back(new FountainEmitter(mExplosionMesh, D3DXVECTOR3(arenaEdge, 0, arenaEdge), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(mExplosionMesh, D3DXVECTOR3(arenaEdge, 0, 0), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(mExplosionMesh, D3DXVECTOR3(arenaEdge, 0, -arenaEdge), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(mExplosionMesh, D3DXVECTOR3(0, 0, arenaEdge), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(mExplosionMesh, D3DXVECTOR3(-arenaEdge, 0, arenaEdge), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(mExplosionMesh, D3DXVECTOR3(-arenaEdge, 0, 0), emissionRate, lifetime));
	mArenaParticles.push_back(new FountainEmitter(mExplosionMesh, D3DXVECTOR3(-arenaEdge, 0, -arenaEdge), emissionRate, lifetime));

	for (auto iter : mArenaParticles)
	{
		iter->StartEmission();
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
	while (!mStaticScenery.empty())
	{
		delete mStaticScenery.back();
		mStaticScenery.pop_back();
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
	// Slow-motion while bomb is detonating
	if (mBombSwitch)
	{
		frameTime *= 0.25f;
	}

#ifdef _DEBUG
	if (gEngine->KeyHit(Key_T))
	{
		CreateNewPickup();
	}

	if (gEngine->KeyHit(Key_Y))
	{
		mPlayer.SetHealth(10000000U);
	}
#endif

	// Handle player input
	{
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

		if (gEngine->KeyHeld(Mouse_LButton))
		{
			mPlayer.SetTryFire();
		}
	}

	// Update the bomb
	{
		mBombExplosionTimer.Update(frameTime);
		if (mBombExplosionTimer.IsComplete())
		{
			if (mBombSwitch)
			{
				mBombSwitch = false;
				mBombModel->ResetScale();
				mBombModel->SetPosition(OFF_SCREEN_POS.x, OFF_SCREEN_POS.y, OFF_SCREEN_POS.z);
				mBombSound->Stop();
			}
			else if (mPlayer.GetBombs() > 0 && gEngine->KeyHit(Key_Space))
			{
				mBombSwitch = true;
				mBombExplosionTimer.Reset(0.9f);
				mBombCollisionCylinder.SetRadius(0.0);
				mBombCollisionCylinder.SetPosition(D3DXVECTOR2(mPlayer.GetWorldPos().x, mPlayer.GetWorldPos().z));
				mBombModel->SetPosition(mPlayer.GetWorldPos().x, 7.0f, mPlayer.GetWorldPos().z);
				mPlayer.TakeBomb();
				mBombSound->Play();
			}
		}
	}


	// Display the HUD
	{
		static const auto screenWidth = gEngine->GetWidth();
		static const auto screenHeight = gEngine->GetHeight();

		// Draw the score
		auto hudText = to_string(mCurrentScore);
		HUDFont->Draw(hudText, screenWidth / 2 + 138, 6, kGreen, kRight);

		// Draw the multiplier
		hudText = "x " + to_string(mMultiplier);
		HUDFont->Draw(hudText, screenWidth / 2 + 165, 35, 0xFFFF0000, kRight);

		// Draw current health
		hudText = to_string(mPlayer.GetHealth());
		HUDFont->Draw(hudText, 245, 6, kGreen, kRight);

		// Draw the current stage
		hudText = to_string(mCurrentStage);
		HUDFont->Draw(hudText, screenWidth - 20, 6, kGreen, kRight);

		// Draw remaining lives
		hudText = to_string(mPlayer.GetLives());
		HUDFont->Draw(hudText, 235, screenHeight - 50, kGreen, kRight);

		// Draw remaining bombs
		hudText = to_string(mPlayer.GetBombs());
		HUDFont->Draw(hudText, screenWidth - 20, screenHeight - 50, kGreen, kRight);
	}


#ifdef _DEBUG
	// Toggle collision box markers
	if (gEngine->KeyHit(Key_M))
	{
		mPlayer.GetCollisionCylinder()->ToggleMarkers();
		for (size_t i = 0; i < mEnemies.size(); i++)
		{
			mEnemies[i]->GetCollisionCylinder()->ToggleMarkers();
		}
		for (auto enemy : mEnemyPool)
		{
			enemy->GetCollisionCylinder()->ToggleMarkers();
		}
		mCollisionBox.ToggleMarkers();
	}

	// Toggle the arena particle
	if (gEngine->KeyHit(Key_J))
	{
		for (auto iter : mArenaParticles)
		{
			iter->StartEmission();
		}
	}
	else if (gEngine->KeyHit(Key_K))
	{
		for (auto iter : mArenaParticles)
		{
			iter->StopEmission();
		}
	}

	// Display the FPS on the window title
	gEngine->SetWindowCaption(to_string(1.0f / frameTime));

#endif

	// Update all arena particle emitters
	{
		for (auto iter = mArenaParticles.begin(); iter != mArenaParticles.end();)
		{
			ParticleEmitter* emitter = *iter;
			emitter->Update(frameTime);

			// Delete the emitter if it is an explosion and is finished
			if (!emitter->IsEmitting() && dynamic_cast<ExplosionEmitter*>(emitter) != nullptr)
			{
				delete(emitter);
				iter = mArenaParticles.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	// Update the player
	{
		mPlayer.Update(frameTime);

		// Check if player is colliding with arena

		if (!CollisionDetect(mPlayer.GetCollisionCylinder(), &mCollisionBox))
		{
			mPlayer.CollisionResolution(mCollisionBox);
		}
	}

	// Update all the projectiles
	{
		auto playerWeapon = mPlayer.GetWeapon();
		auto& weaponProjectiles = playerWeapon->GetProjectiles();
		for (auto iter = weaponProjectiles.begin(); iter != weaponProjectiles.end();)
		{
			auto projectile = (*iter);
			auto projectileCollisionVolume = projectile->GetCollisionObject();

			// Check if the projectile collides with the arena
			if (!CollisionDetect(&projectileCollisionVolume, &mCollisionBox))
			{
				playerWeapon->RemoveProjectile(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	// Update all the enemies
	{
		const auto playerPosition = mPlayer.GetWorldPos();
		Enemy::SetPlayerPosition(playerPosition);

		auto playerWeapon = mPlayer.GetWeapon();
		auto& weaponProjectiles = playerWeapon->GetProjectiles();

		for (auto i = 0U; i < mEnemies.size(); ++i)
		{
			auto enemy = mEnemies[i];
			auto entityPosition = enemy->GetWorldPos();
			enemy->Update(frameTime);

			auto enemyHit = false;
			auto enemyCollisionVolume = enemy->GetCollisionCylinder();

			// Check for collision with the arena boundary
			if (!CollisionDetect(enemyCollisionVolume, &mCollisionBox))
			{
				enemy->CollisionResolution(mCollisionBox);
				//enemy->SetPosition(entityPosition);
			}

			// Check for collision with all other enemies
			for (auto j = i + 1; j < mEnemies.size(); ++j)
			{
				auto otherEnemy = mEnemies[j];
				auto otherEnemyCollisionVolume = otherEnemy->GetCollisionCylinder();
				if (CollisionDetect(enemyCollisionVolume, otherEnemyCollisionVolume))
				{
					enemy->CollisionResolution(*otherEnemyCollisionVolume);
					otherEnemy->CollisionResolution(*enemyCollisionVolume);
				}
			}

			// Check if this enemy has collided with any projectiles
			for (auto iter = weaponProjectiles.begin(); iter != weaponProjectiles.end();)
			{
				auto projectile = (*iter);
				auto projectileCollisionVolume = projectile->GetCollisionObject();

				// Check if the projectile collides with the enemy
				if (CollisionDetect(enemyCollisionVolume, &projectileCollisionVolume))
				{
					auto damage = projectile->GetDamage();
					if (enemy->TakeHealth(damage))
					{
						enemyHit = true;
						mCurrentScore += enemy->GetDamage() * mMultiplier;
						mKillCount++;
					}

					playerWeapon->RemoveProjectile(iter);
					break;
				}
				else
				{
					++iter;
				}
			}

			// Check for collision with the player
			if (CollisionDetect(enemyCollisionVolume, mPlayer.GetCollisionCylinder()))
			{
				if (!mPlayer.IsShielded())
				{
					if (!enemy->GetBoss())
					{
						mPlayer.TakeHealth(enemy->GetDamage());
					}
					else
					{
						mPlayer.TakeHealth(mPlayer.GetHealth() - 1U);
					}
					mMultiplier = 1U;
					mKillCount = 0U;
				}
				enemyHit = true;
			}

			// Check for collision with the bomb
			if (mBombSwitch && CollisionDetect(&mBombCollisionCylinder, enemyCollisionVolume))
			{
				enemyHit = true;
				mCurrentScore += enemy->GetDamage() / 2;
			}

			// If the enemy was hit then create an explosion and play the audio source
			if (enemyHit)
			{
				mEnemyDestroyedSound->Play();

				ParticleEmitter* explosion = new ExplosionEmitter(mExplosionMesh, entityPosition, 1.0f);
				explosion->StartEmission();
				mArenaParticles.push_back(explosion);

				enemy->SetPosition(OFF_SCREEN_POS);

				mEnemyPool.push_back(enemy);
				mEnemies.erase(mEnemies.begin() + i--);
			}
		}
	}

	// Update the bomb
	{
		if (mBombSwitch)
		{
			// Increase the collision volume radius
			mBombCollisionCylinder.SetRadius(mBombCollisionCylinder.GetRadius() + (300.0f * frameTime));

			// Scale the bomb model
			mBombModel->ResetScale();
			mBombModel->Scale(mBombCollisionCylinder.GetRadius() * 0.1f);
		}
	}

	if (mKillCount > 5)
	{
		if (mMultiplier < 99)
		{
			mMultiplier++;
			mKillCount = 0;
		}
	}

	// Pickups
	mPickupTimer.Update(frameTime);
	if (mPickupTimer.IsComplete() && mNoPickupsThisRound < 3)
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
			mPlayer.GiveShield();
		}
		this->Clear();
		LoadStage(mCurrentStage);
		mCurrentScore = mScore;
	}

	// check if there should be an enemy spawned
	if (mEnemies.size() < MAX_ENEMIES_ON_SCREEN && mNoOfEnemies > 0)
	{
		mNoOfEnemies--;
		SpawnEnemy(false);
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
	if (mCurrentStage % 10 == 0)
	{
		mNoOfEnemies = mCurrentStage;
		CreateEnemies();
		SpawnEnemy(true);
	}
	else
	{
		mNoOfEnemies = static_cast<uint32_t>((mCurrentStage + 30U) * 1.5f);
	}

	mNoPickupsThisRound = 0;

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
			if (static_cast<unsigned int>(std::stoi(score)) > mCurrentScore)
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
		mEnemies[i]->SetPosition(OFF_SCREEN_POS);
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

void Arena::SpawnEnemy(bool boss)
{
	// Play the enemy spawn sound
	mEnemySpawnSound->Play();

	Enemy* enemy = mEnemyPool.back();
	mEnemyPool.pop_back();

	if (boss)
	{
		enemy->SetHealth(static_cast<uint32_t>(5000 + mCurrentStage * 15));
		enemy->GetModel()->Scale(5.0f);
		enemy->GetCollisionCylinder()->SetRadius(50.0f);
		enemy->SetBoss(true);
	}
	else
	{
		enemy->SetHealth(static_cast<uint32_t>(100 + mCurrentStage * 3));
		enemy->GetModel()->ResetScale();
		enemy->GetCollisionCylinder()->SetRadius(15.0f);
		enemy->SetBoss(false);
	}

	enemy->SetDamage(10U + mCurrentStage / 2U );

	do
	{
		D3DXVECTOR3 newPosition;
		newPosition.x = Random(mCollisionBox.GetMinOffset().x + 15, mCollisionBox.GetMaxOffset().x - 15);
		newPosition.y = 7.0f;
		newPosition.z = Random(mCollisionBox.GetMinOffset().y + 15, mCollisionBox.GetMaxOffset().y - 15);

		enemy->SetPosition(newPosition);
		enemy->SetPosition(newPosition);	//Additional line to prevent issues with collsion detection the frame after spawning
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
	float lifetime = Random(8.0f, 15.0f);

	float pickupSeed = Random(0.0f, 100.0f);

	float healthChance;
	float bombChance;
	float weaponUpgradeChance;
	float shieldChance;
	//float extra life Chance;	//Dont need this, just using else, keeping for completeness

	if (mCurrentStage <= 10)
	{
		healthChance = 10.0f;
		bombChance = 20.0f;
		weaponUpgradeChance = 85.0f;
		shieldChance = 95.0f;
		//Extra life chance = 100.0f;
	}
	else
	{
		healthChance = 40.0f;
		bombChance = 65.0f;
		weaponUpgradeChance = 75.0f;
		shieldChance = 95.0f;
		//Extra life chance = 100.0f;

	}

	if (pickupSeed <= healthChance)
	{
		//health
		mPickups.push_back(new HealthPack(position, 15.0f, lifetime, 50U));
	}
	else if (pickupSeed <= bombChance)
	{
		//bomb
		mPickups.push_back(new Bomb(position, 15.0f, lifetime));
	}
	else if (pickupSeed <= weaponUpgradeChance)
	{
		//weapon upgrade
		mPickups.push_back(new WeaponUpgrade(WeaponUpgrade::mMesh, position, 15.0f, lifetime));
	}
	else if (pickupSeed <= shieldChance)
	{
		position.y += 15.0f;
		mPickups.push_back(new Shield(position, 15.0f, lifetime));
		mPickups.back()->GetModel()->Scale(0.1f);
	}
	else
	{
		mPickups.push_back(new ExtraLife(position, 15.0f, lifetime));
	}

	mNoPickupsThisRound++;

	mPickups.back()->GetModel()->Scale(15.0f);

	mPickupTimer.Reset(Random(8.6f, 12.2f));
}