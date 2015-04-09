#include "Player.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

IMesh* Player::MESH = nullptr;

const float Player::RADIUS = 1.0f;
const uint32_t Player::DEFAULT_LIVES = 5U;
const uint32_t Player::DEFAULT_BOMBS = 3U;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Player
Player::Player(const D3DXVECTOR3& position, float radius) :
	Entity(MESH, position, radius),
	mLives(DEFAULT_LIVES),
	mBombs(DEFAULT_BOMBS),
	mWeapon(new Weapon())
{
	//mWeapon->SetDamage(50U);
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
// Setters
//-----------------------------------

void Player::TakeLife()
{
	mLives--;
}

void Player::TakeBomb()
{
	mBombs--;
}

//-----------------------------------
// Other
//-----------------------------------

// Respawns the player inside the arena
void Player::Respawn()
{
	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	ResetHealth();
}

// Called to update the entity
void Player::Update(float frameTime)
{
	//***** Face the player in the direction of the mouse *****// - incomplete

	//Convert the mouse pixel location to a -1 to 1 coordinate system (0 in the middle)
	D3DXVECTOR2 mouseVector;
	mouseVector.x = gEngine->GetMouseX() - (gEngine->GetWidth() / 2.0f) - 1.0f;
	mouseVector.y = 1.0f - (gEngine->GetMouseY() - gEngine->GetHeight() / 2.0f);

	////Normalise the mouse vector
	D3DXVec2Normalize(&mouseVector, &mouseVector);

	D3DXVECTOR3 rightVect3 = GetRightVector();	//Create and obtain the facing vector of the ship
	D3DXVECTOR2 rightVect2;
	D3DXVec2Normalize(&rightVect2, &D3DXVECTOR2(rightVect3.x, rightVect3.z));

	float dotProd = D3DXVec2Dot(&rightVect2, &mouseVector);

	////Done all the maths to determine if left or right, now set the flags for turning later in the function
	if (dotProd < 0)		// Mouse is to the right
	{
		RotateY(TURN_SPEED * frameTime);	//Turn right
	}
	else if (dotProd > 0)	//Mouse is to the left
	{
		RotateY(-TURN_SPEED * frameTime);	//Turn left
	}

	//******* End of direct the player *********//

	//************WEAPON*************//
	// Update the weapon
	mWeapon->Update(frameTime, GetWorldPos(), GetFacingVector());

	//*********END OF WEAPON*********//

	//****************************** MOVEMENT ******************************// //DO NOT MESS WITH THE MOVEMENT CODE OR VARIABLES IF YOU NEED SOMETHING - ASK DANIEL
	D3DXVECTOR2 thrust = D3DXVECTOR2(0.0f, 0.0f);	//Create thrust vector and initialise it to zero
	D3DXVECTOR3 facingVect = D3DXVECTOR3(0.0f, 0.0f, 1.0f);//GetFacingVector();
	D3DXVECTOR3 rightVect = D3DXVECTOR3(1.0f, 0.0f, 0.0f);//GetRightVector();
	D3DXVECTOR2 drag = D3DXVECTOR2(0.0f, 0.0f);	//Create vector of drag (resistance to motion) of the ship

	//SetPreviousPos();	/**THIS IS FOR COLLISION RESOLUTION, SETS THE POSITION THE MODEL WAS JUST IN NOT CURRENTLY IMPLEMENTED**/

	if (mMoveForward)	//If thrust flag is true (thrust is applied)
	{
		//Increase thrust by calculated components of thrust based upon linear thrust and the ships facing vector
		thrust.x += (BASE_THRUST * facingVect.x * frameTime);
		thrust.y += (BASE_THRUST * facingVect.z * frameTime);
	}
	if (mMoveRight)
	{
		//Increase thrust by calculated components of thrust based upon linear thrust and the ships right vector
		thrust.x += (BASE_THRUST * rightVect.x * frameTime);
		thrust.y += (BASE_THRUST * rightVect.z * frameTime);
	}
	if (mMoveBackward)	//Reverse thrust flag is true (braking/reversing)
	{
		//Decrease thrust by calculated components of thrust based upon negative linear thrust and the ships facing vector
		thrust.x -= (BASE_THRUST * facingVect.x * frameTime);
		thrust.y -= (BASE_THRUST * facingVect.z * frameTime);
	}
	if (mMoveLeft)
	{
		//Decrease thrust by calculated components of thrust based upon negative linear thrust and the ships right vector
		thrust.x -= (BASE_THRUST * rightVect.x * frameTime);
		thrust.y -= (BASE_THRUST * rightVect.z * frameTime);
	}

	//Calculate drag components based on current velocity components and the drag Coefficient
	drag.x = -(mVelocity.x * DRAG_COEF) * frameTime;
	drag.y = -(mVelocity.y * DRAG_COEF) * frameTime;

	//Set new velocity based upon current velocity, thrust and drag
	mVelocity.x += thrust.x + drag.x;
	mVelocity.y += thrust.y + drag.y;

	//Move model by velocity vector
	MoveX(mVelocity.x * frameTime);
	MoveZ(mVelocity.y * frameTime);

	//Unset flags
	mMoveLeft = false;
	mMoveRight = false;

	mMoveForward = false;
	mMoveBackward = false;

	//****************************** MOVEMENT ******************************// //DO NOT MESS WITH THE MOVEMENT CODE OR VARIABLES IF YOU NEED SOMETHING - ASK DANIEL

	//Update collision object to meet new position
	GetCollisionCylinder().SetPosition(D3DXVECTOR2(GetWorldPos().x, GetWorldPos().z));
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
