#include "Arena.hpp"
using namespace HIC;

// Global Engine
I3DEngine* gEngine = nullptr;

// Global Camera
ICamera* gCamera = nullptr;

// The game arena
Arena* gArena = nullptr;

///////////////////////////
// Front End Data

IFont* gMainMenuFont = nullptr;

ISprite* gContinueButtonSprite			= nullptr;
ISprite* gNewGameButtonSprite			= nullptr;
ISprite* gViewHiScoreButtonSprite		= nullptr;
ISprite* gQuitGameButtonSprite			= nullptr;

const float MENU_BUTTON_WIDTH = 256U;
const float MENU_BUTTON_HEIGHT = 64U;

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
	gCamera = gEngine->CreateCamera(kFPS, 0.0f, 500.0f, -700.0f);

	return true;
}

// One off shutdown of the entire program
// Returns true on success, false on failure
bool ProgramShutdown()
{
	// Shutdown the TL-Engine
	gEngine->Delete();
	gEngine = nullptr;

	// Cleanup pointers
	gCamera = nullptr;

	return true;
}

// Setups the front end menu
// Returns true on success, false on failure
bool FrontEndSetup()
{
	// Load the main menu font
	gMainMenuFont = gEngine->LoadFont("Ubuntu Mono", 72);

	// Load the menu button sprites
	uint32_t halfScreenWidth = gEngine->GetWidth() / 2;

	// TODO: Check if save file exists

	gContinueButtonSprite = gEngine->CreateSprite("Continue_Button.png", halfScreenWidth - (MENU_BUTTON_WIDTH / 2), 250);

	return true;
}

// Updates the front end menu
// Returns true on success, false on failure
bool FrontEndUpdate(float frameTime)
{
	// Calculate the position to draw the title text
	uint32_t screenWidth = gEngine->GetWidth();
	uint32_t textWidth = gMainMenuFont->MeasureTextWidth("Hyper Infinity Carnage");

	// Draw the title
	gMainMenuFont->Draw("Hyper Infinity Carnage", (screenWidth / 2) - (textWidth / 2), 100, kRed);

	return true;
}

// Shutdowns the front end menu
// Returns true on success, false on failure
bool FrontEndShutdown()
{
	// Cleanup the main menu font
	gEngine->RemoveFont(gMainMenuFont);
	gMainMenuFont = nullptr;

	// Cleanup the menu button sprites
	gEngine->RemoveSprite(gContinueButtonSprite);
	gContinueButtonSprite = nullptr;

	return true;
}

// Setups the main game
// Returns true on success, false on failure
bool GameSetup()
{
	// Load the player mesh
	Player::MESH = gEngine->LoadMesh("Player.x");
	Arena::ARENA_MESH = gEngine->LoadMesh("Arena.x");
#ifdef _DEBUG
	CollisionObject::MARKER_MESH = gEngine->LoadMesh("dummy.x");
#endif
	// Load the arena
	gArena = new Arena(gEngine);

	return true;
}

// Updates the main game
// Returns true on success, false on failure
void GameUpdate(float frameTime)
{
	gArena->Update(frameTime);
	//gArena->TargetCamera(gCamera);
}

// Updates the main game
// Returns true on success, false on failure
bool GameShutdown()
{
	// Delete the arena
	delete(gArena);
	gArena = nullptr;

	// Delete the player mesh
	gEngine->RemoveMesh(Player::MESH);
	Player::MESH = nullptr;
	//Delete the arena mesh
	gEngine->RemoveMesh(Arena::ARENA_MESH);
	Arena::ARENA_MESH = nullptr;

#ifdef _DEBUG
	//Delete the collision object mesh
	gEngine->RemoveMesh(CollisionObject::MARKER_MESH);
	CollisionObject::MARKER_MESH = nullptr;
#endif

	return true;
}

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

		// Update the front-end until the exit key is pressed
		while (!gEngine->KeyHit(Key_P))
		{
			// Update the front end
			float frameTime = gEngine->Timer();
			FrontEndUpdate(frameTime);

			// Exit the program if the exit key is pressed
			if (gEngine->KeyHit(Key_Q) || !gEngine->IsRunning())
			{
				FrontEndShutdown();
				ProgramShutdown();
				return EXIT_SUCCESS;
			}

			// Draw the scene
			gEngine->DrawScene();
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
			gEngine->DrawScene();
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
