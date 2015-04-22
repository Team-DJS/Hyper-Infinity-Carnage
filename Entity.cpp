#include "Entity.hpp"
using namespace HIC;


//-----------------------------------
// Static Initialisations
//-----------------------------------

const uint32_t Entity::DEFAULT_HEALTH = 100U;
const uint32_t Entity::DEFAULT_MAX_HEALTH = 100U;

// Movement Variables
const float Entity::BASE_THRUST = 480.0f;
const float Entity::DRAG_COEF = 5.0f;
const float Entity::TURN_SPEED = 200.0f;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Entity
Entity::Entity(IMesh* mesh, const D3DXVECTOR3& position, float radius) :
	mMesh(mesh),
	mModel(mesh->CreateModel(position.x, position.y, position.z)),
	mHealth(DEFAULT_HEALTH),
	mMaxHealth(DEFAULT_MAX_HEALTH),
	mCollisionCylinder(D3DXVECTOR2(position.x, position.z), radius),
	mVelocity(0.0f, 0.0f)
{
}

// Destructor for Entity
Entity::~Entity()
{
	// Remove the entity model
	mModel->GetMesh()->RemoveModel(mModel);
}

//-----------------------------------
// Setters
//-----------------------------------

// Gives health to the entity. If this exceeds the maximum health, then it is clamped
void Entity::GiveHealth(uint32_t health)
{
#ifdef _DEBUG
	if (health > mMaxHealth)
	{
		throw std::runtime_error("Cannot give more than max health");
	}
#endif

	// Add the health to the current health
	mHealth += health;

	// Clamp it so it does not go beyond the maximum health
	if (mHealth > mMaxHealth)
	{
		mHealth = mMaxHealth;
	}
}

// Takes the given health away from the entity. If the damage is greater than the current
// health of the entity, then this method will return true, otherwise false.
bool Entity::TakeHealth(uint32_t damage)
{
	if (damage >= mHealth)
	{
		mHealth = 0U;
		return true;
	}
	mHealth -= damage;
	return false;
}

// Moves the entity in the world x-axis
void Entity::MoveX(float dx)
{
	mModel->MoveX(dx);
}

// Moves the entity in the world z-axis
void Entity::MoveZ(float dz)
{
	mModel->MoveZ(dz);
}

void Entity::RotateY(float degrees)
{
	mModel->RotateY(degrees);
}

void Entity::SetPosition(D3DXVECTOR3 newPosition)
{
	mModel->SetPosition(newPosition.x, newPosition.y, newPosition.z);
	mCollisionCylinder.SetPosition(D3DXVECTOR2(GetWorldPos().x, GetWorldPos().z));
}

void Entity::ResetHealth()
{
	mHealth = DEFAULT_HEALTH;
}

void Entity::SetHealth(uint32_t health)
{
	mHealth = health;
}

//-----------------------------------
// Getters
//-----------------------------------

// Returns the health of entity
uint32_t Entity::GetHealth() const
{
	return mHealth;
}

D3DXVECTOR3 Entity::GetWorldPos()
{
	return D3DXVECTOR3(mModel->GetX(), mModel->GetY(), mModel->GetZ());
}

D3DXVECTOR3 Entity::GetFacingVector()
{
	float matrix[16];
	mModel->GetMatrix(matrix);

	D3DXVECTOR3 face;
	face.x = matrix[8];
	face.y = matrix[9];
	face.z = matrix[10];
	return face;
}

D3DXVECTOR3 Entity::GetRightVector()
{
	float matrix[16];
	mModel->GetMatrix(matrix);

	D3DXVECTOR3 right;
	right.x = matrix[0];
	right.y = matrix[1];
	right.z = matrix[2];
	return right;
}

IModel* Entity::GetModel()
{
	return mModel;
}

CollisionCylinder* Entity::GetCollisionCylinder()
{
	return &mCollisionCylinder;
}

//-----------------------------------
// Other
//-----------------------------------

void Entity::CollisionResolution(CollisionCylinder& collidingWith)
{
	////Reset model position to outside of the collision
	//mModel->SetX(mCollisionCylinder.GetPreviousPosition().x);
	//mModel->SetZ(mCollisionCylinder.GetPreviousPosition().y);

	//Modify the direction of movement based upon the ships position relative to the collision object it collided with and the current movement vector
	D3DXVECTOR2 vectToOtherSphere = collidingWith.GetPosition() - mCollisionCylinder.GetPosition();	//Vector between ship and collided object
	
	
	//Find out how overlapped they are and then offset them that amount
	float distanceAway = D3DXVec2Length(&vectToOtherSphere);
	distanceAway -= (this->GetCollisionCylinder()->GetRadius() + collidingWith.GetRadius());

	distanceAway /= 2;

	D3DXVec2Normalize(&vectToOtherSphere, &vectToOtherSphere);

	vectToOtherSphere.x *= distanceAway;
	vectToOtherSphere.y *= distanceAway;

	mModel->MoveX(vectToOtherSphere.x);
	mModel->MoveZ(vectToOtherSphere.y);

	mCollisionCylinder.SetPosition(D3DXVECTOR2(mModel->GetX(), mModel->GetZ()));
}

void Entity::CollisionResolution(CollisionAABB& collidingWith)
{
	//Get all of the relevant data from the collision objects
	//Sphere object
	float radius = GetCollisionCylinder()->GetRadius();
	D3DXVECTOR2 spherePos = GetCollisionCylinder()->GetPosition();

	//Axis Alligned Bounding Box object 
	//Also applies the radius of the sphere to the box
	float minX = collidingWith.GetMinPosition().x - radius;
	float maxX = collidingWith.GetMaxPosition().x + radius;
	float minZ = collidingWith.GetMinPosition().y - radius;
	float maxZ = collidingWith.GetMaxPosition().y + radius;


	if (GetCollisionCylinder()->GetPreviousPosition().x >= minX && GetCollisionCylinder()->GetPreviousPosition().x <= maxX)	//If the sphere is outside the x boundaries of the box
	{
		//Side X hit
		mCollidedX = true;
		mModel->SetZ( GetCollisionCylinder()->GetPreviousPosition().y);	//Reset position to outside of the collision area
	}
	if (GetCollisionCylinder()->GetPreviousPosition().y >= minZ && GetCollisionCylinder()->GetPreviousPosition().y <= maxZ)
	{
		//Side Z hit
		mCollidedZ = true;
		mModel->SetX(GetCollisionCylinder()->GetPreviousPosition().x);	//Reset position to outside of the collision area
	}

	GetCollisionCylinder()->SetPosition({ mModel->GetX(), mModel->GetZ() });
}

// Points the model at a given position
void Entity::LookAt(const D3DXVECTOR3& position)
{
	mModel->LookAt(position.x, mModel->GetY(), position.z);
}
