// ProSkill Games Test.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

bool ProgramSetup()
{
	return true;
}

bool ProgramShutdown()
{
	return true;
}

bool FrontEndSetup()
{
	return true;
}

bool FrontEndUpdate()
{
	return true;
}

bool FrontEndShutdown()
{
	return true;
}

bool GameSetup()
{
	return true;
}

void GameUpdate()
{

}

bool GameShutdown()
{
	return true;
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* GameEngine = New3DEngine( kTLX );
	GameEngine->StartWindowed();

	// Add default folder for meshes and other media
	GameEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/

	IMesh* buildingMesh = GameEngine->LoadMesh("Cube.x");

	IModel* building = buildingMesh->CreateModel(0.0f, 0.0f, 0.0f);

	building->SetSkin("brown.PNG");
	building->ScaleX(1.0f);
	building->ScaleY(2.0f);
	building->ScaleZ(1.0f);

	ICamera* camera = GameEngine->CreateCamera(kFPS);
	camera->SetMovementSpeed(2.0f);
	camera->SetRotationSpeed(2.0f);

	//camera->SetPosition(0.0f, 20.0f, -20.0f);


	// The main game loop, repeat until engine is stopped
	while (GameEngine->IsRunning())
	{
		// Draw the scene
		GameEngine->DrawScene();

		/**** Update your scene each frame here ****/
		if (GameEngine->KeyHit(Key_Escape))
		{
			GameEngine->Stop();
		}
	}
	
	// Delete the 3D engine now we are finished with it
	GameEngine->Delete();
}
