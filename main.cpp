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
string gPlayerName;
string gPlayerScore;


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
ISprite* gNameInputScreen = nullptr;
IFont* gLoadingScreenFont = nullptr;

//------------------------
// Pause Screen data
//------------------------

Button* gResumeButton = nullptr;

ISprite* gPauseBackground = nullptr;

//------------------------
// InGame HUD Data
//------------------------

ISprite* gHUDScore = nullptr;
ISprite* gHUDHealth = nullptr;
ISprite* gHUDLives = nullptr;
ISprite* gHUDStage = nullptr;
ISprite* gHUDBombs = nullptr;

const float HUD_WIDTH = 254U;
const float HUD_SCORE_WIDTH = 300U;
const float HUD_HEIGHT = 56U;

//------------------------
// High Scores Data
//------------------------

ISprite* gGameOverScreen = nullptr;
bool gShowHighScores = false;

ISprite* gHighScoreCard = nullptr;
IFont* gHighScoreFont = nullptr;

const float HIGH_SCORE_WIDTH = 900;
const float HIGH_SCORE_HEIGHT = 600;

string gHighScoreNames[10];
string gHighScoreScores[10];
const std::string HIGH_SCORES_FILENAME = "High_Scores.hic";

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
	//gEngine->StartFullscreen(1920, 1080);
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
	
	if (!std::ifstream(HIGH_SCORES_FILENAME))
	{
		std::ofstream file(HIGH_SCORES_FILENAME);
		if (file.is_open())
		{
			for (int i = 0; i < 10; i++)
			{
				file << "NONAME" << " " << to_string(1000 - i*100) << endl;
			}
		}
	}

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
// High Score
//-------------------------------------------

// Set up the high score list
bool HighScoreSetup(bool gameOver = false)
{
	if (gameOver)
	{
		gGameOverScreen = gEngine->CreateSprite("Game_Over_screen.png", 0.0f, 0.0f, 0.1f);
	}
	gHighScoreCard = gEngine->CreateSprite("High_Score_Card.tga", 400, 60, 0.0f);
	gHighScoreFont = gEngine->LoadFont("Berlin Sans FB Demi", 48);

	std::ifstream file("High_Scores.hic");

	if (!file.is_open())
	{
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		file >> gHighScoreNames[i];
		file >> gHighScoreScores[i];
	}

	return true;
}

// update the high score list
bool HighScoreUpdate(bool gameOver = false)
{
	int height = static_cast<int>(gHighScoreCard->GetX()) - static_cast<int>(HIGH_SCORE_HEIGHT / 2) + 60;
	for (int i = 0; i < 10; i++)
	{
		gHighScoreFont->Draw(gHighScoreNames[i], static_cast<int>(gHighScoreCard->GetX()) + 30, height + i * 35, kBlue);
		gHighScoreFont->Draw(gHighScoreScores[i], static_cast<int>(gHighScoreCard->GetX()) + 500, height + i * 35, kBlue);
	}

	if (gameOver)
	{
		gHighScoreFont->Draw(gPlayerName, static_cast<int>(gHighScoreCard->GetX()) + 30, height + 390, 0xFFFF0000);
		gHighScoreFont->Draw(gPlayerScore, static_cast<int>(gHighScoreCard->GetX()) + 500, height + 390, 0xFFFF0000);
	}

	return true;
}

// Shutdown the high score list
bool HighScoreShutdown(bool gameOver = false)
{
	if (gameOver)
	{
		gEngine->RemoveSprite(gGameOverScreen);
		gGameOverScreen = nullptr;
	}
	gEngine->RemoveSprite(gHighScoreCard);
	gHighScoreCard = nullptr;

	gEngine->RemoveFont(gHighScoreFont);
	gHighScoreFont = nullptr;
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
	gTitleCard = gEngine->CreateSprite("Title_Card.png", halfScreenWidth - (TITLE_CARD_WIDTH / 2), 20.0f, 0.1f);

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

	if (gShowHighScores)
	{
		if (!HighScoreUpdate())
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}
	}

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
		if (gShowHighScores)
		{
			if (!HighScoreShutdown())
			{
				ProgramShutdown();
				return EXIT_FAILURE;
			}
			gShowHighScores = false;
		}
		else
		{
			if (!HighScoreSetup())
			{
				ProgramShutdown();
				return EXIT_FAILURE;
			}
			gShowHighScores = true;
		}
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

	if (gShowHighScores)
	{
		if (!HighScoreShutdown())
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}
	}

	return true;
}

//-------------------------------------------
// Loading Screen
//-------------------------------------------

// Gets the key the player is pressing and returns char of that key
char GetNameInput()
{
	if (gEngine->IsRunning())
	{
		if (gEngine->KeyHit(Key_A))
		{
			return 'A';
		}
		if (gEngine->KeyHit(Key_B))
		{
			return 'B';
		}
		if (gEngine->KeyHit(Key_C))
		{
			return 'C';
		}
		if (gEngine->KeyHit(Key_D))
		{
			return 'D';
		}
		if (gEngine->KeyHit(Key_E))
		{
			return 'E';
		}
		if (gEngine->KeyHit(Key_F))
		{
			return 'F';
		}
		if (gEngine->KeyHit(Key_G))
		{
			return 'G';
		}
		if (gEngine->KeyHit(Key_H))
		{
			return 'H';
		}
		if (gEngine->KeyHit(Key_I))
		{
			return 'I';
		}
		if (gEngine->KeyHit(Key_J))
		{
			return 'J';
		}
		if (gEngine->KeyHit(Key_K))
		{
			return 'K';
		}
		if (gEngine->KeyHit(Key_L))
		{
			return 'L';
		}
		if (gEngine->KeyHit(Key_M))
		{
			return 'M';
		}
		if (gEngine->KeyHit(Key_N))
		{
			return 'N';
		}
		if (gEngine->KeyHit(Key_O))
		{
			return 'O';
		}
		if (gEngine->KeyHit(Key_P))
		{
			return 'P';
		}
		if (gEngine->KeyHit(Key_Q))
		{
			return 'Q';
		}
		if (gEngine->KeyHit(Key_R))
		{
			return 'R';
		}
		if (gEngine->KeyHit(Key_S))
		{
			return 'S';
		}
		if (gEngine->KeyHit(Key_T))
		{
			return 'T';
		}
		if (gEngine->KeyHit(Key_U))
		{
			return 'U';
		}
		if (gEngine->KeyHit(Key_V))
		{
			return 'V';
		}
		if (gEngine->KeyHit(Key_W))
		{
			return 'W';
		}
		if (gEngine->KeyHit(Key_X))
		{
			return 'X';
		}
		if (gEngine->KeyHit(Key_Y))
		{
			return 'Y';
		}
		if (gEngine->KeyHit(Key_Z))
		{
			return 'Z';
		}
		if (gEngine->KeyHit(Key_1))
		{
			return '1';
		}
		if (gEngine->KeyHit(Key_2))
		{
			return '2';
		}
		if (gEngine->KeyHit(Key_3))
		{
			return '3';
		}
		if (gEngine->KeyHit(Key_4))
		{
			return '4';
		}
		if (gEngine->KeyHit(Key_5))
		{
			return '5';
		}
		if (gEngine->KeyHit(Key_6))
		{
			return '6';
		}
		if (gEngine->KeyHit(Key_7))
		{
			return '7';
		}
		if (gEngine->KeyHit(Key_8))
		{
			return '8';
		}
		if (gEngine->KeyHit(Key_9))
		{
			return '9';
		}
		if (gEngine->KeyHit(Key_0))
		{
			return '0';
		}
		if (gEngine->KeyHit(Key_Minus))
		{
			return '-';
		}

		if (gEngine->KeyHit(Key_Back))
		{
			return '<';
		}

		if (gEngine->KeyHit(Key_Return))
		{
			return '%';
		}
	}
	return '$';
}

bool LoadingSreenSetup(const bool nameEntered)
{

	if (!nameEntered)
	{
		gNameInputScreen = gEngine->CreateSprite("Enter_Name_Card.tga", 200, 60);
		gLoadingScreenFont = gEngine->LoadFont("Berlin Sans FB Demi", 48);
	}
	// display loading screen
	gLoadingScreen = gEngine->CreateSprite("Loading_Screen_Temp.png", 0.0f, 0.0f, 0.1f);
	
	
	return true;
}

bool LoadingScreenUpdate(bool &nameEntered)
{
	if (!nameEntered)
	{
		// do insert name stuff
		char input = GetNameInput();

		if (input != '$' && input != '<' && input != '%' && gPlayerName.length() < 10)
		{
			gPlayerName += input;
		}

		if (input == '<' && gPlayerName.length() > 0)
		{
			gPlayerName.pop_back();
		}

		if (input == '%' && gPlayerName.length() > 0)
		{
			nameEntered = true;
			if (gNameInputScreen)
			{
				gEngine->RemoveSprite(gNameInputScreen);
				gNameInputScreen = nullptr;
			}
			if (gLoadingScreenFont)
			{
				gEngine->RemoveFont(gLoadingScreenFont);
				gLoadingScreenFont = nullptr;
			}
			return true;
		}
		gLoadingScreenFont->Draw(gPlayerName, static_cast<int>(gNameInputScreen->GetX()) + 50, static_cast<int>(gNameInputScreen->GetY()) + 200, kBlue);
		
		gEngine->DrawScene();

		return false;
	}

	gEngine->DrawScene();
	
	return true;
}

bool LoadingScreenShutdown()
{
	gEngine->RemoveSprite(gLoadingScreen);
	gLoadingScreen = nullptr;

	if (gNameInputScreen)
	{
		gEngine->RemoveSprite(gNameInputScreen);
		gNameInputScreen = nullptr;
	}
	if (gLoadingScreenFont)
	{
		gEngine->RemoveFont(gLoadingScreenFont);
		gLoadingScreenFont = nullptr;
	}
	return true;
}

//-------------------------------------------
// Main Game
//-------------------------------------------

// Setups the main game
// Returns true on success, false on failure
bool GameSetup(bool loadSaveGame)
{
	// Load the player mesh - now loaded in ProgramSetup!
	// Load arena enemy and projectile meshes
	Player::SHIELD = gEngine->LoadMesh("ShieldSphere.x");
	Arena::ARENA_MESH = gEngine->LoadMesh("Arena.x");
	Arena::ENEMY_MESH = gEngine->LoadMesh("Enemy.x");
	Projectile::MESH = gEngine->LoadMesh("quad_additive.x");

	// HUD Setup
	gHUDScore = gEngine->CreateSprite("HUD_Score.png", gEngine->GetWidth() / 2 - (HUD_SCORE_WIDTH / 2), 0.0f, 0.2f);
	gHUDHealth = gEngine->CreateSprite("HUD_Health.png", 0.0f, 0.0f, 0.2f);
	gHUDStage = gEngine->CreateSprite("HUD_Stage.png", gEngine->GetWidth() - HUD_WIDTH, 0.0f, 0.2f);
	gHUDLives = gEngine->CreateSprite("HUD_Lives.png", 0.0f, gEngine->GetHeight() - HUD_HEIGHT, 0.2f);
	gHUDBombs = gEngine->CreateSprite("HUD_Bombs.png", gEngine->GetWidth() - HUD_WIDTH, gEngine->GetHeight() - HUD_HEIGHT, 0.2f);

	// Load the bomb explosion audio
	gAudioManager->LoadAudio("BombExplosion", "Media\\Audio\\BombExplosion.wav");

#ifdef _DEBUG
	CollisionObject::MARKER_MESH = gEngine->LoadMesh("dummy.x");
#endif

	// Load the arena
	gArena = new Arena(loadSaveGame, gPlayerName);

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

	// Delete player shield mesh
	gEngine->RemoveMesh(Player::SHIELD);
	Player::SHIELD = nullptr;

	//Delete the arena mesh
	gEngine->RemoveMesh(Arena::ARENA_MESH);
	Arena::ARENA_MESH = nullptr;
	//Delete the enemy mesh
	gEngine->RemoveMesh(Arena::ENEMY_MESH);
	Arena::ENEMY_MESH = nullptr;

	// Delete HUD
	gEngine->RemoveSprite(gHUDScore);
	gEngine->RemoveSprite(gHUDHealth);
	gEngine->RemoveSprite(gHUDStage);
	gEngine->RemoveSprite(gHUDLives);
	gEngine->RemoveSprite(gHUDBombs);
	gHUDScore = nullptr;

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
	gResumeButton = new Button("Resume_Button.png", D3DXVECTOR2((float)gEngine->GetWidth() / 2 - TITLE_CARD_WIDTH / 2, 425.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	gViewHiScoreButton = new Button("View_Hi_Score_Button.png", D3DXVECTOR2((float)gEngine->GetWidth() / 2 - TITLE_CARD_WIDTH / 2, 500.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	gQuitGameButton = new Button("Quit_Game_Button.png", D3DXVECTOR2((float)gEngine->GetWidth() / 2 - TITLE_CARD_WIDTH / 2, 575.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
	return true;
}

bool PauseMenuUpdate(bool &quitGame, bool &quitPause)
{
	bool isMouseDown = gEngine->KeyHit(Mouse_LButton);

	if (gResumeButton->MouseIsOver() && isMouseDown)
	{
		Button::BUTTON_CLICK_SOUND->Play();
		quitPause = true;
		return true;
	}

	//Click view high scores
	if (gViewHiScoreButton->MouseIsOver() && isMouseDown)
	{
		Button::BUTTON_CLICK_SOUND->Play();

		if (gShowHighScores)
		{
			if (!HighScoreShutdown())
			{
				ProgramShutdown();
				return EXIT_FAILURE;
			}
			gShowHighScores = false;
		}
		else
		{
			if (!HighScoreSetup())
			{
				ProgramShutdown();
				return EXIT_FAILURE;
			}
			gShowHighScores = true;
		}
	}
	//Click quit game
	else if (gQuitGameButton->MouseIsOver() && isMouseDown)
	{
		Button::BUTTON_CLICK_SOUND->Play();
		quitGame = true;
	}

	if (gShowHighScores)
	{
		if (!HighScoreUpdate())
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}
	}
	return true;
}

bool PauseMenuShutdown()
{
	SafeRelease(gResumeButton);
	SafeRelease(gViewHiScoreButton);
	SafeRelease(gQuitGameButton);
	gEngine->RemoveSprite(gPauseBackground);
	gPauseBackground = nullptr;

	if (gShowHighScores)
	{
		if (!HighScoreShutdown())
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}
		gShowHighScores = false;
	}

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
		
		gPlayerName = "";
		bool nameEntered;
		if (!loadSaveGame)
		{
			nameEntered = false;
		}
		else
		{
			nameEntered = true;
		}

		LoadingSreenSetup(nameEntered);

		while (!LoadingScreenUpdate(nameEntered))
		{
			Sleep(2);
		}
		if (!LoadingScreenUpdate(nameEntered))
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}

		LoadingScreenShutdown();

		///////////////////////////////////
		// Game Enter

		// Setup the game
		if (!GameSetup(loadSaveGame))
		{
			ProgramShutdown();
			return EXIT_FAILURE;
		}

		bool gameOver = false;

		// Continue in game loop until exited
		while (!quitGame)
		{
			// Update the game
			float frameTime = gEngine->Timer();
			GameUpdate(frameTime);

			if (!gArena->PlayerHasLives())
			{
				quitGame = true;
				gameOver = true;
			}

			if (gEngine->KeyHit(Key_Escape))
			{

				if (!PauseMenuSetup())
				{
					ProgramShutdown();
					return EXIT_FAILURE;
				}
				bool quitPause = false;
				while (!gEngine->KeyHit(Key_Escape) && gEngine->IsRunning() && !quitGame && !quitPause)
				{
					PauseMenuUpdate(quitGame, quitPause);
					gEngine->DrawScene(gCamera);

					if (!gEngine->IsRunning())
					{
						ProgramShutdown();
						return EXIT_FAILURE;
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

		if (gameOver)
		{
			HighScoreSetup(true);
			// Game over screen
			gPlayerName = gArena->GetPlayerName();
			gPlayerScore = to_string(gArena->GetScore());

			Timer exitTimer(5.0f);
			while (true)
			{
				HighScoreUpdate(true);
				exitTimer.Update(gEngine->Timer());
				if (exitTimer.IsComplete() || (gEngine->IsRunning() && gEngine->KeyHit(Key_Escape)))
				{
					break;
				}
				else
				{
					gEngine->DrawScene();
				}
			}
			
			HighScoreShutdown(true);
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
