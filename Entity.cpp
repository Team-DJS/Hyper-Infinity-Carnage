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
Entity::Entity(IMesh* mesh, const XMFLOAT3& position, float radius) :
	mMesh(mesh),
	mModel(mesh->CreateModel(position.x, position.y, position.z)),
	mHealth(DEFAULT_HEALTH),
	mMaxHealth(DEFAULT_MAX_HEALTH),
	mCollisionCylinder(XMFLOAT2(position.x, position.z), radius)
{
	mRigidBody = gPhysicsManager->CreateRigidBody(mModel, 100);
	SetPhysicsBodyToModel();
	mRigidBody->addContactListener(this);
}

// Destructor for Entity
Entity::~Entity()
{
	// Remove the entity model
	mModel->GetMesh()->RemoveModel(mModel);

	if (mRigidBody->isActive())
	{
		// Remove the rigid body from the physics world
		gPhysicsManager->RemoveRigidBody(mRigidBody);
		mRigidBody->removeContactListener(this);
	}


}

//-----------------------------------
// Setters
//-----------------------------------

// Gives health to the entity. If this exceeds the maximum health, then it is clamped
void Entity::GiveHealth(uint32_t health)
{
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
	if (damage > mHealth)
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

void Entity::SetPosition(XMFLOAT3 newPosition)
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

XMFLOAT3 Entity::GetWorldPos()
{
	return XMFLOAT3(mModel->GetX(), mModel->GetY(), mModel->GetZ());
}

XMFLOAT3 Entity::GetFacingVector()
{
	float matrix[16];
	mModel->GetMatrix(matrix);

	XMFLOAT3 face;
	face.x = matrix[8];
	face.y = matrix[9];
	face.z = matrix[10];
	return face;
}

XMFLOAT3 Entity::GetRightVector()
{
	float matrix[16];
	mModel->GetMatrix(matrix);

	XMFLOAT3 right;
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

// Points the model at a given position
void Entity::LookAt(const XMFLOAT3& position)
{
	mModel->LookAt(position.x, mModel->GetY(), position.z);
}


void Entity::SetModelToPhysicsBody()
{
	// Get the (world) matrix of the physics body from Havok. However, Havok does not retain scaling for bodies, so this function ensures the existing model scaling is kept
	HK_ALIGN16(hkFloat32 mat[16]); // See comment on function above
	mModel->GetMatrix(mat);
	float scaleX = sqrt(mat[0] * mat[0] + mat[1] * mat[1] + mat[2] * mat[2]);
	float scaleY = sqrt(mat[4] * mat[4] + mat[5] * mat[5] + mat[6] * mat[6]);
	float scaleZ = sqrt(mat[8] * mat[8] + mat[9] * mat[9] + mat[10] * mat[10]);
	mRigidBody->getTransform().get4x4ColumnMajor(mat);
	mat[0] *= scaleX;	mat[1] *= scaleX;	mat[2] *= scaleX;
	mat[4] *= scaleY;	mat[5] *= scaleY;	mat[6] *= scaleY;
	mat[8] *= scaleZ;	mat[9] *= scaleZ;	mat[10] *= scaleZ;
	mModel->SetMatrix(mat);

}

void Entity::SetPhysicsBodyToModel()
{
	// Need to use the model's (world) matrix to position the physics body in Havok. However, Havok does not handle scaling of bodies, so this function removes the scaling first
	HK_ALIGN16(hkFloat32 mat[16]); // The Havok HK_ALIGN16 macro lets us declare this array so it starts on a 16 byte boundary in memory (0, 16, or 32 etc.). This 
	// allows Havok to implement a more efficient matrix copy. Must also do this in your own code - it will crash without the alignment
	mModel->GetMatrix(mat);
	float scaleX = sqrt(mat[0] * mat[0] + mat[1] * mat[1] + mat[2] * mat[2]);
	float scaleY = sqrt(mat[4] * mat[4] + mat[5] * mat[5] + mat[6] * mat[6]);
	float scaleZ = sqrt(mat[8] * mat[8] + mat[9] * mat[9] + mat[10] * mat[10]);
	mat[0] /= scaleX;	mat[1] /= scaleX;	mat[2] /= scaleX;
	mat[4] /= scaleY;	mat[5] /= scaleY;	mat[6] /= scaleY;
	mat[8] /= scaleZ;	mat[9] /= scaleZ;	mat[10] /= scaleZ;
	hkTransform transform;
	transform.set4x4ColumnMajor(mat);
	mRigidBody->setTransform(transform);
}

void Entity::contactPointCallback(const hkpContactPointEvent& p_event)
{
	//HK_ASSERT2(0xf455ea07, p_event.m_source != hkpCollisionEvent::SOURCE_WORLD, "Do not add this listener to the world.");
	//printf("IT WENT IN THE THING \n");

	// Put your collision here
	// or alternatively, set a boolean to tell you that
	//   you are colliding, and use it in your update()
}
