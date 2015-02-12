#include "Arena.hpp"
using namespace HIC;

// Default constructor for Arena
Arena::Arena() :
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
