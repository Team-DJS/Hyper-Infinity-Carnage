#include "Player.hpp"
using namespace HIC;

IMesh* Player::MESH = nullptr;
const uint32_t Player::DEFAULT_LIVES = 5U;
const uint32_t Player::DEFAULT_BOMBS = 3U;

// Default constructor for Player
Player::Player() : 
	Entity(MESH, XMFLOAT3(0.0f, 0.0f, 0.0f)),
	mLives(DEFAULT_LIVES),
	mBombs(DEFAULT_BOMBS),
	mWeapon(new Weapon())
{
}

// Destructor for Player
Player::~Player()
{
}

// Returns the number of remaining lives the player has
uint32_t Player::GetLives() const
{
	return mLives;
}

// Returns the number of reminaing bombs the player has
uint32_t Player::GetBombs() const
{
	return mBombs;
}

// Gives an extra life to the player
void Player::GiveLife()
{
	mLives++;
}

// Gives an extra bomb to the player
void Player::GiveBomb()
{
	mBombs++;
}

// Respawns the player inside the arena
void Player::Respawn()
{
}

// Called to update the entity
void Player::Update(float frameTime)
{
}
