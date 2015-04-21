#include "Enemy.hpp"
using namespace HIC;


//-----------------------------------
// Static Initialisations
//-----------------------------------

D3DXVECTOR3 Enemy::PLAYER_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Enemy
Enemy::Enemy(IMesh* mesh, const D3DXVECTOR3& position, float radius, uint32_t damage) :
	Entity(mesh, position, radius),
	mDamage(damage)
{
}

// Destructor for Enemy
Enemy::~Enemy()
{
}

//-----------------------------------
// Setters
//-----------------------------------

void Enemy::SetPlayerPosition(D3DXVECTOR3 playerPos)
{
	PLAYER_POS = playerPos;
}

//-----------------------------------
// Getters
//-----------------------------------

// Returns the damage the enemy deals
uint32_t Enemy::GetDamage() const
{
	return mDamage;
}

//-----------------------------------
// Other
//-----------------------------------

// Called to update the entity
void Enemy::Update(float frameTime)
{
	//Decide how to turn
	//Convert the mouse pixel location to a -1 to 1 coordinate system (0 in the middle)
	D3DXVECTOR3 playerVector3 = this->GetWorldPos() - PLAYER_POS;
	D3DXVECTOR2 playerVector = D3DXVECTOR2(playerVector3.x, playerVector3.z);

	//Normalise the mouse vector
	D3DXVec2Normalize(&playerVector, &playerVector);

	D3DXVECTOR3 rightVect3 = GetRightVector();	//Create and obtain the facing vector of the ship
	D3DXVECTOR2 rightVect2;
	D3DXVec2Normalize(&rightVect2, &D3DXVECTOR2(rightVect3.x, rightVect3.z));

	float dotProd = D3DXVec2Dot(&rightVect2, &playerVector);

	//Done all the maths to determine if left or right, now set the flags for turning later in the function
	if (dotProd >= 0.1f || dotProd <= -0.1f)	//Fudge factor to prevent the enemy shaking left and right
	{
		if (dotProd < 0)		// Player is to the right
		{
			RotateY(TURN_SPEED * frameTime);	//Turn right
		}
		else if (dotProd > 0)	//Player is to the left
		{
			RotateY(-TURN_SPEED * frameTime);	//Turn left
		}
	}

	//Decide whether to move forward (and how fast)
	MoveX(GetFacingVector().x * 75.0f * frameTime);
	MoveZ(GetFacingVector().z * 75.0f * frameTime);

	//Set new collision position
	GetCollisionCylinder()->SetPosition(D3DXVECTOR2(GetWorldPos().x, GetWorldPos().z));
}