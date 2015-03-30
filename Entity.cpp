#include "Entity.hpp"
using namespace HIC;


//-----------------------------------
// Static Initialisations
//-----------------------------------

const uint32_t Entity::DEFAULT_HEALTH = 100U;
const uint32_t Entity::DEFAULT_MAX_HEALTH = 100U;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

// Default constructor for Entity
Entity::Entity(IMesh* mesh, const D3DXVECTOR3& position, float radius) :
	mMesh(mesh),
	mModel(mesh->CreateModel(position.x, position.y, position.z)),
	mHealth(DEFAULT_HEALTH),
	mMaxHealth(DEFAULT_MAX_HEALTH),
	mCollisionCylinder(D3DXVECTOR2(position.x, position.z), radius)
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
#ifdef _DEBUG
	if (damage > mMaxHealth)
	{
		throw std::runtime_error("Cannot take more than max health");
	}
#endif

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
}

void Entity::ResetHealth()
{
	mHealth = DEFAULT_HEALTH;
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

CollisionCylinder& Entity::GetCollisionCylinder()
{
	return mCollisionCylinder;
}

//-----------------------------------
// Other
//-----------------------------------

void Entity::CollisionResolution(CollisionCylinder& collidingWith)
{
	////Reset model position to outside of the collision
	//mModel->SetX(mCollisionCylinder.GetPreviousPosition().x);
	//mModel->SetZ(mCollisionCylinder.GetPreviousPosition().y);
	//
	////Modify the direction of movement based upon the ships position relative to the collision object it collided with and the current movement vector
	//XMVECTOR vectToOtherSphere = XMLoadFloat2(&D3DXVECTOR2(collidingWith.GetPosition().x - mCollisionCylinder.GetPosition().x, collidingWith.GetPosition().y - mCollisionCylinder.GetPosition().y));	//Vector between ship and collided object
	//
	////Angle by which the vector is to be rotated (degrees) - Based upon dot product and other trigonomety
	////angle to rotate by equals 180 - 2 * alpha where alpha is the angle between the tangent of the spheres and the movement vector
	//XMVECTOR angleBetweenVectors = XMVector2AngleBetweenVectors(XMLoadFloat2(vectToOtherSphere), XMLoadFloat2(velocity))
	//	
	//float rotationAngle = 180.0f - (2.0f * (90.0f - XMConvertToDegrees())));
	//rotationAngle = XMConvertToRadians(rotationAngle);	//Convert to radians for use with c++ trig formulae
	//
	////Calculate the new velocity by rotating the vector using a 2D rotation matrix (multiply components by 0.8f to make movement more realistic
	//velocity = Vector2(velocity.GetX()*cosf(rotationAngle) - velocity.GetZ()*sinf(rotationAngle) * 0.8f,
	//velocity.GetX()*sinf(rotationAngle) + velocity.GetZ() * cosf(rotationAngle) * 0.8f);
}

// Points the model at a given position
void Entity::LookAt(const D3DXVECTOR3& position)
{
	mModel->LookAt(position.x, mModel->GetY(), position.z);
}