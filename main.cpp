#include "Entity\Player.hpp"
using namespace HIC;

// Global Engine
I3DEngine* gEngine = nullptr;

// Global Camera
ICamera* gCamera = nullptr;

// TEMPORARY PLAYER
Entity* gPlayer = nullptr;

// One off setup for the entire program.
// Returns true on success, false on failure
bool ProgramSetup()
{
	// Initialise the TL-Engine
	gEngine = New3DEngine(kTLX);
	gEngine->StartWindowed();

	// Add default folder for meshes and other media
	gEngine->AddMediaFolder(".\\Media");

	// initialise the camera
	gCamera = gEngine->CreateCamera(kFPS);
	gCamera->SetMovementSpeed(50.0f);
	gCamera->SetRotationSpeed(1000.0f);

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
	Player::MESH = gEngine->LoadMesh("Player.x");
	gPlayer = new Player();

	return true;
}

// Updates the front end menu
// Returns true on success, false on failure
bool FrontEndUpdate(float frameTime)
{
	gPlayer->OnUpdate(gEngine, frameTime);

	return true;
}

// Shutdowns the front end menu
// Returns true on success, false on failure
bool FrontEndShutdown()
{
	// Delete the player
	delete(gPlayer);
	gPlayer = nullptr;

	// Delete the player mesh
	gEngine->RemoveMesh(Player::MESH);
	Player::MESH = nullptr;

	return true;
}

// Setups the main game
// Returns true on success, false on failure
bool GameSetup()
{
	return true;
}

// Updates the main game
// Returns true on success, false on failure
void GameUpdate(float frameTime)
{

}

// Updates the main game
// Returns true on success, false on failure
bool GameShutdown()
{
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
