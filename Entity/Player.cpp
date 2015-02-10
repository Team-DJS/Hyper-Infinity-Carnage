#include "Player.hpp"
using namespace HIC;

IMesh* Player::MESH = nullptr;
uint32_t Player::DEFAULT_LIVES = 5U;

// Default constructor for Player
Player::Player() : 
	Entity(MESH, XMFLOAT3(0.0f, 0.0f, 0.0f)),
	mLives(DEFAULT_LIVES)
{
}

// Destructor for Player
Player::~Player()
{
}

// Returns the number of remaining lives the player has
uint32_t Player::GetRemainingLives() const
{
	return mLives;
}

// Called to update the entity
void Player::OnUpdate(I3DEngine* engine, float frameTime)
{
	IModel* model = GetModel();

	// Check for player movement
	if (engine->KeyHeld(Key_W))
	{
		model->MoveZ(10.0f * frameTime);
	}
}
