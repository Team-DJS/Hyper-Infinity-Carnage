#include "Player.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

IMesh* Player::MESH = nullptr;

const float Player::RADIUS = 1.0f;
const uint32_t Player::DEFAULT_LIVES = 5U;
const uint32_t Player::DEFAULT_BOMBS = 3U;

// Movement Variables
const float Player::BASE_THRUST = 480.0f;
const float Player::TURN_SPEED = 80000.0f;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Player
Player::Player(const XMFLOAT3& position) :
	Entity(MESH, position, 350.0f),
	mLives(DEFAULT_LIVES),
	mBombs(DEFAULT_BOMBS),
	mWeapon(new Weapon())
{
	mRigidBody->setAngularDamping(10.0f);
}

// Destructor for Player
Player::~Player()
{
}

//-----------------------------------
// Setters
//-----------------------------------

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

//-----------------------------------
// Getters
//-----------------------------------

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

// Returns the weapon which the player has
Weapon* Player::GetWeapon()
{
	return mWeapon;
}

//-----------------------------------
// Other
//-----------------------------------

// Respawns the player inside the arena
void Player::Respawn()
{
	SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	ResetHealth();
}

// Called to update the entity
void Player::Update(float frameTime)
{
	this->SetModelToPhysicsBody();

	//***** Face the player in the direction of the mouse *****//

	////Convert the mouse pixel location to a -1 to 1 coordinate system (0 in the middle)
	XMFLOAT2 mouseVector;
	mouseVector.x = gEngine->GetMouseX() - (gEngine->GetWidth() / 2.0f) - 1.0f;
	mouseVector.y = 1.0f - (gEngine->GetMouseY() - gEngine->GetHeight() / 2.0f);
	
	//Normalise the mouse vector
	XMStoreFloat2(&mouseVector, XMVector2Normalize(XMLoadFloat2(&mouseVector)));
	
	XMFLOAT3 rightVect3 = GetRightVector();	//Create and obtain the facing vector of the ship
	XMFLOAT2 rightVect2 = XMFLOAT2(rightVect3.x, rightVect3.z);
	XMStoreFloat2(&rightVect2, XMVector2Normalize(XMLoadFloat2(&rightVect2)));	//Normalise the facing vector
	
	float dotProd;
	XMStoreFloat(&dotProd, XMVector2Dot(XMLoadFloat2(&rightVect2), XMLoadFloat2(&mouseVector)));
	
	////Done all the maths to determine if left or right, now set the flags for turning later in the function
	if (dotProd < 0)		// Mouse is to the right
	{
		RotateY(TURN_SPEED, frameTime);	//Turn right
	}
	else if (dotProd > 0)	//Mouse is to the left
	{
		RotateY(-TURN_SPEED, frameTime);	//Turn left
	}
	
	////******* End of direct the player *********//
	//
	////************WEAPON*************//
	//// Update the weapon
	//mWeapon->Update(frameTime, GetWorldPos(), GetFacingVector());
	//
	////*********END OF WEAPON*********//
	
	//****************************** MOVEMENT ******************************// 
	XMFLOAT3 directionalVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	if (mMoveForward)	//If thrust flag is true (thrust is applied)
	{
		directionalVelocity.z += 100.0f * frameTime;
	}
	if (mMoveRight)
	{
		directionalVelocity.x += 100.0f * frameTime;
	}
	if (mMoveBackward)
	{
		directionalVelocity.z -= 100.0f * frameTime;
	}
	if (mMoveLeft)
	{
		directionalVelocity.x -= 100.0f * frameTime;
	}
	
	//Move model by velocity vector
	GetRigidBody()->applyLinearImpulse(hkVector4(directionalVelocity.x, directionalVelocity.y, directionalVelocity.z));
	
	//Unset flags
	mMoveLeft = false;
	mMoveRight = false;
	
	mMoveForward = false;
	mMoveBackward = false;
	
	////****************************** MOVEMENT ******************************// //DO NOT MESS WITH THE MOVEMENT CODE OR VARIABLES IF YOU NEED SOMETHING - ASK DANIEL

}

//-----------------------------------
// Movement / Setters
//-----------------------------------

void Player::SetMoveForward()
{
	mMoveForward = true;
}

void Player::SetMoveBackward()
{
	mMoveBackward = true;
}

void Player::SetMoveLeft()
{
	mMoveLeft = true;
}

void Player::SetMoveRight()
{
	mMoveRight = true;
}

void Player::SetTryFire()
{
	mWeapon->SetFire();
}

void Player::Clear()
{
	mWeapon->Clear();
}