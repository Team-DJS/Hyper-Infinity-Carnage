#include "Player.hpp"
using namespace HIC;

//-----------------------------------
// Static Initialisations
//-----------------------------------

IMesh* Player::MESH = nullptr;
IMesh* Player::SHIELD = nullptr;

const float Player::RADIUS = 1.0f;
const uint32_t Player::DEFAULT_LIVES = 3U;
const uint32_t Player::DEFAULT_BOMBS = 3U;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Player
Player::Player(const D3DXVECTOR3& position, float radius) :
	Entity(MESH, position, radius),
	mShieldModel(SHIELD->CreateModel(position.x, position.y, position.z)),
	mLives(DEFAULT_LIVES),
	mBombs(DEFAULT_BOMBS),
	mWeapon(new Weapon()),
	mShield(true),
	mShieldTimer(3.0f)
{
	mShieldModel->Scale(1.2f);
	mShieldModel->AttachToParent(GetModel());
}

// Destructor for Player
Player::~Player()
{
	mShieldModel->GetMesh()->RemoveModel(mShieldModel);
}

//-----------------------------------
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

void Player::GiveShield()
{
	mShield = true;
	mShieldTimer.Reset();
	mShieldModel->ResetScale();
	mShieldModel->Scale(1.2f);
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

bool Player::IsShielded()
{
	return mShield;
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
	if (mShield)
	{
		//mShieldModel->SetPosition(GetWorldPos().x, GetWorldPos().y, GetWorldPos().z);
		//mShieldModel->RotateY(60 * frameTime);
		mShieldTimer.Update(frameTime);
	}
	if (mShield && mShieldTimer.IsComplete())
	{
		mShield = false;
		mShieldTimer.Reset();
		mShieldModel->Scale(0.000001f);
		//mShieldModel->SetPosition(0.0f, 0.0f, -1200.0f);
	}
	
	//***** Face the player in the direction of the mouse *****//
	D3DXVECTOR3 rightVect3 = GetRightVector();	//Create and obtain the right vector of the ship
	D3DXVECTOR2 rightVect2;
	D3DXVec2Normalize(&rightVect2, &D3DXVECTOR2(rightVect3.x, rightVect3.z));

	D3DXVECTOR3 rayPosition3 = WorldPosFromPixel();
	D3DXVECTOR2 rayPosition2 = D3DXVECTOR2(rayPosition3.x, rayPosition3.z);

	D3DXVECTOR2 playerWorldPos2 = D3DXVECTOR2(this->GetWorldPos().x, this->GetWorldPos().z);

	D3DXVECTOR2 rayVector2 = rayPosition2 - playerWorldPos2;
	D3DXVec2Normalize(&rayVector2, &rayVector2);

	float dotProd = D3DXVec2Dot(&rightVect2, &rayVector2);

	////Done all the maths to determine if left or right, now set the flags for turning later in the function
	if (dotProd >= 0.05f || dotProd <= -0.05f)	//Fudge factor to prevent the player shaking left and right
	{
		if (dotProd < 0)		// Mouse is to the right
		{
			RotateY(TURN_SPEED * frameTime);	//Turn right
		}
		else if (dotProd > 0)	//Mouse is to the left
		{
			RotateY(-TURN_SPEED * frameTime);	//Turn left
		}
	}
	//this->GetModel()->LookAt(foo.x, 0.0f, foo.z);
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
	MoveX(2 * mVelocity.x * frameTime);
	MoveZ(2 *mVelocity.y * frameTime);

	//Unset flags
	mMoveLeft = false;
	mMoveRight = false;

	mMoveForward = false;
	mMoveBackward = false;

	//****************************** MOVEMENT ******************************// //DO NOT MESS WITH THE MOVEMENT CODE OR VARIABLES IF YOU NEED SOMETHING - ASK DANIEL

	//Update collision object to meet new position
	GetCollisionCylinder()->SetPosition(D3DXVECTOR2(GetWorldPos().x, GetWorldPos().z));
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
