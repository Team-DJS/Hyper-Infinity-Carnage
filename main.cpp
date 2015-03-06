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

ISprite* gTitleCard			= nullptr;
Button* gNewGameButton		= nullptr;
Button* gContinueButton		= nullptr;
Button* gViewHiScoreButton	= nullptr;
Button* gQuitGameButton		= nullptr;

const float MENU_BUTTON_WIDTH = 192U;
const float MENU_BUTTON_HEIGHT = 64U;
const float TITLE_CARD_WIDTH = 800U;
const float TITLE_CARD_HEIGHT = 256;
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
	
	return true;
}

// One off shutdown of the entire program
// Returns true on success, false on failure
bool ProgramShutdown()
{
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

	gNewGameButton			= new Button("New_Game_Button.png", XMFLOAT2((float)halfScreenWidth, 350.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	gContinueButton			= new Button("Continue_Button.png", XMFLOAT2((float)halfScreenWidth, 425.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	gViewHiScoreButton		= new Button("View_Hi_Score_Button.png", XMFLOAT2((float)halfScreenWidth, 500.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	gQuitGameButton			= new Button("Quit_Game_Button.png", XMFLOAT2((float)halfScreenWidth, 575.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	
	return true;
}

// Updates the front end menu
// Returns true on success, false on failure
bool FrontEndUpdate(float frameTime)
{
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

	return true;
}

//-------------------------------------------
// Main Game
//-------------------------------------------

// Setups the main game
// Returns true on success, false on failure
bool GameSetup()
{
	// Load the player mesh
	Player::MESH = gEngine->LoadMesh("Player.x");
	Arena::ARENA_MESH = gEngine->LoadMesh("Arena.x");
	Arena::ENEMY_MESH = gEngine->LoadMesh("Enemy.x");

#ifdef _DEBUG
	CollisionObject::MARKER_MESH = gEngine->LoadMesh("dummy.x");
#endif
	// Load the arena
	gArena = new Arena();

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

	// Delete the player mesh
	gEngine->RemoveMesh(Player::MESH);
	Player::MESH = nullptr;
	//Delete the arena mesh
	gEngine->RemoveMesh(Arena::ARENA_MESH);
	Arena::ARENA_MESH = nullptr;
	//Delete the enemy mesh
	gEngine->RemoveMesh(Arena::ENEMY_MESH);
	Arena::ENEMY_MESH = nullptr;
#ifdef _DEBUG
	//Delete the collision object mesh
	gEngine->RemoveMesh(CollisionObject::MARKER_MESH);
	CollisionObject::MARKER_MESH = nullptr;

	gCamera = gGameCamera;
#endif

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
		// Update the front-end until the exit key is pressed
		while (!quitFrontend)
		{
			if (gEngine->KeyHit(Key_P))
			{
				quitFrontend = true;
			}

			//Menu option selection
			if (gEngine->KeyHit(Mouse_LButton))
			{
				if (gNewGameButton)
				{
					if (gNewGameButton->MouseIsOver())
					{
						quitFrontend = true;					//Load into main game
					}
				}
				if (gContinueButton)			
				{
					if (gContinueButton->MouseIsOver())
					{
						//TODO: Perform some action
					}
				}
				if (gViewHiScoreButton)
				{
					if (gViewHiScoreButton->MouseIsOver())
					{
						//TODO: Load high scores
					}
				}
				if (gQuitGameButton)
				{
					if (gQuitGameButton->MouseIsOver())
					{
						FrontEndShutdown();						//Quit the program
						ProgramShutdown();
						return EXIT_SUCCESS;
					}
				}
			}

			// Exit the program if the exit key/button is pressed
			if (gEngine->KeyHit(Key_Q) || !gEngine->IsRunning())
			{
				FrontEndShutdown();
				ProgramShutdown();
				return EXIT_SUCCESS;
			}

			// Update the front end
			float frameTime = gEngine->Timer();
			FrontEndUpdate(frameTime);

			// Draw the scene
			gEngine->DrawScene(gCamera);
		}

		if (!FrontEndShutdown())
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}

		// Front End Exit
		///////////////////////////////////



		// TODO: Loading Screen



		///////////////////////////////////
		// Game Enter

		// Setup the game
		if (!GameSetup())
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}

		// Continue in game loop until exited
		while (!gEngine->KeyHit(Key_Escape))
		{

			// Update the game
			float frameTime = gEngine->Timer();
			GameUpdate(frameTime);

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
