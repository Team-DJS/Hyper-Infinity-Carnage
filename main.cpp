#include "Arena.hpp"
#include "Button.hpp"
#include "AudioManager.hpp"

using namespace HIC;

// Global Engine
I3DEngine* gEngine = nullptr;

// Global Camera
ICamera* gCamera = nullptr;
ICamera* gGameCamera = nullptr;
#ifdef _DEBUG
ICamera* gDebugCamera = nullptr;
#endif

// The game arena
Arena* gArena = nullptr;

//------------------------
// Front End Data
//------------------------

ISprite* gTitleCard = nullptr;
Button* gNewGameButton = nullptr;
Button* gContinueButton = nullptr;
Button* gViewHiScoreButton = nullptr;
Button* gQuitGameButton = nullptr;
IModel* gFrontEndPlayer = nullptr;

const float MENU_BUTTON_WIDTH = 192U;
const float MENU_BUTTON_HEIGHT = 64U;
const float TITLE_CARD_WIDTH = 800U;
const float TITLE_CARD_HEIGHT = 256;

//------------------------
// Loading screen data
//------------------------

ISprite* gLoadingScreen = nullptr;

//------------------------
// Pause Screen data
//------------------------

ISprite* gPauseBackground = nullptr;

//------------------------
// InGame HUD Data
//------------------------

ISprite* gHUDTopBar = nullptr;

const float HUD_TOP_BAR_WIDTH = 1280U;

//-------------------------------------------
// Program
//-------------------------------------------

// One off setup for the entire program.
// Returns true on success, false on failure
bool ProgramSetup()
{
	// Initialise the TL-Engine
	gEngine = New3DEngine(kTLX);
	gEngine->StartWindowed();
	//gEngine->StartFullscreen();
	gEngine->SetWindowCaption("Hyper Infinity Carnage");

	// Add default folder for meshes and other media
	gEngine->AddMediaFolder(".\\Media");

	// Initialise the camera
	gGameCamera = gEngine->CreateCamera(kManual, 0.0f, 500.0f, -700.0f);
#ifdef _DEBUG
	gDebugCamera = gEngine->CreateCamera(kFPS, 0.0f, 500.0f, -700.0f);
	gDebugCamera->SetMovementSpeed(200.0f);
	gDebugCamera->SetRotationSpeed(20.0f);
#endif
	gCamera = gGameCamera;

	// Initialise the AudioManager
	gAudioManager = new AudioManager();

	// Load the gameplay audio
	gAudioManager->LoadAudio("GameplayMusic", "Media\\Audio\\GameplayTheme.wav");

	// Load the button over sound
	gAudioManager->LoadAudio("ButtonOver", "Media\\Audio\\ButtonOver.wav");
	Button::BUTTON_OVER_SOUND = gAudioManager->CreateSource("ButtonOver", { 0.0f, 0.0f, 0.0f });
	Button::BUTTON_OVER_SOUND->SetLooping(false);
	gAudioManager->LoadAudio("ButtonClick", "Media\\Audio\\ButtonClick.wav");
	Button::BUTTON_CLICK_SOUND = gAudioManager->CreateSource("ButtonClick", { 0.0f, 0.0f, 0.0f });
	Button::BUTTON_CLICK_SOUND->SetLooping(false);

	// Load the weapon shoot sound
	gAudioManager->LoadAudio("WeaponShoot", "Media\\Audio\\WeaponShoot.wav");
	Weapon::SHOOT_SOUND = gAudioManager->CreateSource("WeaponShoot", { 0.0f, 0.0f, 0.0f });
	Weapon::SHOOT_SOUND->SetLooping(false);

	Player::MESH = gEngine->LoadMesh("Player.x");

	return true;
}

// One off shutdown of the entire program
// Returns true on success, false on failure
bool ProgramShutdown()
{
	// Remove player mesh
	gEngine->RemoveMesh(Player::MESH);
	Player::MESH = nullptr;

	// Release the button over source
	gAudioManager->ReleaseSource(Button::BUTTON_OVER_SOUND);
	Button::BUTTON_OVER_SOUND = nullptr;
	gAudioManager->ReleaseSource(Button::BUTTON_CLICK_SOUND);
	Button::BUTTON_CLICK_SOUND = nullptr;

	// Release any loaded audio
	gAudioManager->ReleaseAudio("ButtonOver");
	gAudioManager->ReleaseAudio("WeaponShoot");
	gAudioManager->ReleaseAudio("GameplayMusic");
	// Release the audio manager
	SafeRelease(gAudioManager);

	// Remove the cameras
	gEngine->RemoveCamera(gGameCamera);
	gGameCamera = nullptr;

#ifdef _DEBUG
	gEngine->RemoveCamera(gDebugCamera);
	gDebugCamera = nullptr;
#endif

	// Shutdown the TL-Engine
	gEngine->Delete();
	gEngine = nullptr;



	return true;
}

//-------------------------------------------
// Front End
//-------------------------------------------

// Setups the front end menu
// Returns true on success, false on failure
bool FrontEndSetup()
{
	// Load the menu button sprites
	uint32_t halfScreenWidth = gEngine->GetWidth() / 2;

	// TODO: Check if save file exists

	//Create menu buttons
	gTitleCard = gEngine->CreateSprite("Title_Card.png", halfScreenWidth - (TITLE_CARD_WIDTH / 2), 20.0f, 0.0f);

	gNewGameButton = new Button("New_Game_Button.png", D3DXVECTOR2((float)halfScreenWidth - TITLE_CARD_WIDTH / 2, 350.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	gContinueButton = new Button("Continue_Button.png", D3DXVECTOR2((float)halfScreenWidth - TITLE_CARD_WIDTH / 2, 425.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	gViewHiScoreButton = new Button("View_Hi_Score_Button.png", D3DXVECTOR2((float)halfScreenWidth - TITLE_CARD_WIDTH / 2, 500.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	gQuitGameButton = new Button("Quit_Game_Button.png", D3DXVECTOR2((float)halfScreenWidth - TITLE_CARD_WIDTH / 2, 575.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);

	// Load player model for front end display
	gFrontEndPlayer = Player::MESH->CreateModel(200, 0, 0);
	gFrontEndPlayer->Scale(5.0f);

	gCamera->LookAt(0, 150, 0);

	return true;
}

// Updates the front end menu
// Returns true on success, false on failure
bool FrontEndUpdate(float frameTime, bool& quitProgram, bool& playNewGame, bool& playSavedGame)
{
	gFrontEndPlayer->RotateLocalY(30 * frameTime);
	// Draw the scene
	gEngine->DrawScene(gCamera);

	//-----------------------------
	// State checks
	//-----------------------------

	quitProgram = false;
	playNewGame = false;
	playSavedGame = false;

	if (gEngine->KeyHit(Key_P))
	{
		playNewGame = true;
		return true;
	}

	bool isMouseDown = gEngine->KeyHit(Mouse_LButton);

	//Click new game
	if (gNewGameButton->MouseIsOver() && isMouseDown)
	{
		Button::BUTTON_CLICK_SOUND->Play();
		playNewGame = true;
		return true;
	}
	//Click continue
	else if (gContinueButton->MouseIsOver() && isMouseDown)
	{
		Button::BUTTON_CLICK_SOUND->Play();
		playSavedGame = true;
		return true;
	}
	//Click view high scores
	else if (gViewHiScoreButton->MouseIsOver() && isMouseDown)
	{
		Button::BUTTON_CLICK_SOUND->Play();
		//TODO: Load high scores
	}
	//Click quit game
	else if (gQuitGameButton->MouseIsOver() && isMouseDown)
	{
		Button::BUTTON_CLICK_SOUND->Play();
		quitProgram = true;
		return true;
	}

	// Exit the program if the exit key/button is pressed
	if (gEngine->KeyHit(Key_Q) || !gEngine->IsRunning())
	{
		quitProgram = true;
		return true;
	}


	return true;
}

// Shutdowns the front end menu
// Returns true on success, false on failure
bool FrontEndShutdown()
{
	// Cleanup the menu button sprites
	SafeRelease(gNewGameButton);
	SafeRelease(gContinueButton);
	SafeRelease(gViewHiScoreButton);
	SafeRelease(gQuitGameButton);

	if (gTitleCard)
	{
		gEngine->RemoveSprite(gTitleCard);
		gTitleCard = nullptr;
	}

	gFrontEndPlayer->GetMesh()->RemoveModel(gFrontEndPlayer);
	gFrontEndPlayer = nullptr;

	return true;
}

//-------------------------------------------
// Loading Screen
//-------------------------------------------

bool LoadingSreenSetup(bool loadSaveGame)
{
	if (!loadSaveGame)
	{
		// do insert name stuff
	}
	
	// display loading screen
	gLoadingScreen = gEngine->CreateSprite("Loading_Screen_Temp.png", 0.0f, 0.0f, 0.0f);


	return true;
}

bool LoadingScreenUpdate()
{
	gEngine->DrawScene();
	return true;
}

bool LoadingScreenShutdown()
{
	gEngine->RemoveSprite(gLoadingScreen);
	gLoadingScreen = nullptr;
	return true;
}

//-------------------------------------------
// Main Game
//-------------------------------------------

// Setups the main game
// Returns true on success, false on failure
bool GameSetup()
{
	// Load the player mesh - now loaded in ProgramSetup!
	// Load arena enemy and projectile meshes
	Arena::ARENA_MESH = gEngine->LoadMesh("Arena.x");
	Arena::ENEMY_MESH = gEngine->LoadMesh("Enemy.x");
	Projectile::MESH = gEngine->LoadMesh("ProjectilePortal.x");

	// HUD Setup
	gHUDTopBar = gEngine->CreateSprite("HUD_Top_Bar.png", gEngine->GetWidth() / 2 - (HUD_TOP_BAR_WIDTH / 2), 0.0f, 0.0f);

	// Load the bomb explosion audio
	gAudioManager->LoadAudio("BombExplosion", "Media\\Audio\\BombExplosion.wav");

#ifdef _DEBUG
	CollisionObject::MARKER_MESH = gEngine->LoadMesh("dummy.x");
#endif
	return true;
}

// Updates the main game
// Returns true on success, false on failure
void GameUpdate(float frameTime)
{
	gArena->Update(frameTime);
	gArena->TargetCamera(gGameCamera);
#ifdef _DEBUG
	if (gEngine->KeyHit(Key_Tab))
	{
		//Swap which camera is the gCamera
		if (gGameCamera == gCamera)
		{
			gCamera = gDebugCamera;
		}
		else
		{
			gCamera = gGameCamera;
		}
	}
#endif
}

// Updates the main game
// Returns true on success, false on failure
bool GameShutdown()
{
	// Before deleting the arena remove all the entities in the arena
	gArena->Clear();

	// Delete the arena
	SafeRelease(gArena);

	//Delete the arena mesh
	gEngine->RemoveMesh(Arena::ARENA_MESH);
	Arena::ARENA_MESH = nullptr;
	//Delete the enemy mesh
	gEngine->RemoveMesh(Arena::ENEMY_MESH);
	Arena::ENEMY_MESH = nullptr;

	// Delete HUD
	gEngine->RemoveSprite(gHUDTopBar);
	gHUDTopBar = nullptr;

#ifdef _DEBUG
	//Delete the collision object mesh
	gEngine->RemoveMesh(CollisionObject::MARKER_MESH);
	CollisionObject::MARKER_MESH = nullptr;

	gCamera = gGameCamera;
#endif


	return true;
}

bool PauseMenuSetup()
{
	gPauseBackground = gEngine->CreateSprite("PauseBackground.tga", 0.0f, 0.0f, 0.1f);
	gViewHiScoreButton = new Button("View_Hi_Score_Button.png", D3DXVECTOR2((float)TITLE_CARD_WIDTH / 2, 500.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	gQuitGameButton = new Button("Quit_Game_Button.png", D3DXVECTOR2((float)TITLE_CARD_WIDTH / 2, 575.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	return true;
}

void PauseMenuUpdate(bool &quitProgram)
{
	bool isMouseDown = gEngine->KeyHit(Mouse_LButton);

	//Click view high scores
	if (gViewHiScoreButton->MouseIsOver() && isMouseDown)
	{
		Button::BUTTON_CLICK_SOUND->Play();
		//TODO: Load high scores
	}
	//Click quit game
	else if (gQuitGameButton->MouseIsOver() && isMouseDown)
	{
		Button::BUTTON_CLICK_SOUND->Play();
		quitProgram = true;
	}
}

bool PauseMenuShutdown()
{
	SafeRelease(gViewHiScoreButton);
	SafeRelease(gQuitGameButton);
	gEngine->RemoveSprite(gPauseBackground);
	gPauseBackground = nullptr;
	return true;
}

//-------------------------------------------
// Main
//-------------------------------------------

// Main entry to the program
int main(int argc, char* argv[])
{
	// Initialise program (TL-Engine, OpenAL etc...). Quit Program on Failure
	if (!ProgramSetup())
	{
		return EXIT_FAILURE;
	}

	// Loop to return to the front-end
	while (true)
	{
		///////////////////////////////////
		// Front End Enter

		// Setup the front end
		if (!FrontEndSetup())
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}

		bool quitFrontend = false;
		bool playNewGame = false;
		bool loadSaveGame = false;
		bool quitGame = false;

		// Update the front-end until the exit key is pressed
		while (!quitFrontend)
		{
			if (!FrontEndUpdate(gEngine->Timer(), quitGame, playNewGame, loadSaveGame))
			{
				return EXIT_FAILURE;
			}

			// React to front end flags

			if (quitGame)
			{
				// Quit the program
				FrontEndShutdown();
				ProgramShutdown();
				return EXIT_SUCCESS;

			}
			if (playNewGame || loadSaveGame)
			{
				quitFrontend = true;
			}

		}

		if (!FrontEndShutdown())
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}

		// Front End Exit
		///////////////////////////////////

		LoadingSreenSetup(loadSaveGame);
		LoadingScreenUpdate();
		LoadingScreenShutdown();

		///////////////////////////////////
		// Game Enter

		// Setup the game
		if (!GameSetup())
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}

		// Load the arena
		gArena = new Arena(loadSaveGame);
		bool quitProgram = false;
		// Continue in game loop until exited
		while (!quitProgram)
		{
			// Update the game
			float frameTime = gEngine->Timer();
			GameUpdate(frameTime);

			if (gEngine->KeyHit(Key_Escape) && !quitProgram)
			{

				if (!PauseMenuSetup())
				{
					ProgramShutdown();
					return EXIT_FAILURE;
				}
				while (!gEngine->KeyHit(Key_Escape) && gEngine->IsRunning() && !quitProgram)
				{
					PauseMenuUpdate(quitProgram);
					gEngine->DrawScene(gCamera);

					if (!gEngine->IsRunning())
					{
						ProgramShutdown();
						return EXIT_SUCCESS;
					}
				}

				if (!PauseMenuShutdown())
				{
					ProgramShutdown();
					return EXIT_FAILURE;
				}

				frameTime = gEngine->Timer();
				frameTime = 0;
			}

			// Immediate program exit required (User closed window or pressed Alt-F4)
			if (!gEngine->IsRunning())
			{
				ProgramShutdown();
				return EXIT_SUCCESS;
			}

			// Draw the scene
			gEngine->DrawScene(gCamera);
		}

		if (!GameShutdown())
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}

		// Game Exit
		///////////////////////////////////
	}

	// Shutdown the program
	ProgramShutdown();

	return EXIT_SUCCESS;
}
