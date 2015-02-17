#include "Arena.hpp"
#include <fstream>
using namespace HIC;

// Default constructor for Arena
Arena::Arena(I3DEngine* engine) :
	mEngine(engine),
	mPlayer(new Player())
{
}

// Destructor for Arena
Arena::~Arena()
{
}

// Updates all the entities inside the arena
void Arena::Update(float frameTime)
{
	// Update the player
	mPlayer->Update(frameTime);
	
	// Handle player input
	if (mEngine->KeyHeld(Key_W))
	{
		mPlayer->MoveZ(50.0f * frameTime);
	}
	if (mEngine->KeyHeld(Key_A))
	{
		mPlayer->MoveX(-50.0f * frameTime);
	}
	if (mEngine->KeyHeld(Key_S))
	{
		mPlayer->MoveZ(-50.0f * frameTime);
	}
	if (mEngine->KeyHeld(Key_D))
	{
		mPlayer->MoveX(50.0f * frameTime);
	}
	

	// Update all the enemies
	for (auto& enemy : mEnemies)
	{
		enemy->Update(frameTime);
	}
}

// Proceeds to the next stage
void Arena::NextStage()
{
}

// Saves the game to be loaded at a later date
void Arena::Save()
{
}

// Removes all entities from the arena
void Arena::Clear()
{
}
