#include "PhysicsManager.hpp"

PhysicsManager* gPhysicsManager = nullptr;

///*-----------------------------------------------------------------------------------------
//Havok include and library files
//-----------------------------------------------------------------------------------------*/


// Havok has many include files - the more functions and classes you use, the more files you need to include
// Although intially less convenient, this reduces compile times over using a single catch-all include file
// We "undefine" or exclude any Havok features we're not using. Havok's linking system is over-complex, these must be correct to avoid errors
#include <Common/Base/keycode.cxx>
#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION_2012
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#undef HK_FEATURE_PRODUCT_MILSIM
//#undef HK_FEATURE_PRODUCT_PHYSICS
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
#define HK_EXCLUDE_FEATURE_MemoryTracker
#include <Common/Base/Config/hkProductFeatures.cxx>


// These are the Havok libraries that are to be linked (the Havok code basically). It is more usual to put library names in the project settings so we don't
// normally see them in the code like this - they are not needed for compiling our source, only for the link stage. However, this is a Microsoft-specific
// pragma that allows us to add libraries in a similar way to header files. It's quite convinient and especially useful for Havok with it's complex requirements
#pragma comment(lib, "hkBase.lib")
#pragma comment(lib, "hkSceneData.lib")
#pragma comment(lib, "hkVisualize.lib")
#pragma comment(lib, "hkInternal.lib")
#pragma comment(lib, "hkSerialize.lib")
#pragma comment(lib, "hkGeometryUtilities.lib")
#pragma comment(lib, "hkcdInternal.lib")
#pragma comment(lib, "hkcdCollide.lib")
#pragma comment(lib, "hkpCollide.lib")
#pragma comment(lib, "hkpConstraint.lib")
#pragma comment(lib, "hkpConstraintSolver.lib")
#pragma comment(lib, "hkpDynamics.lib")
#pragma comment(lib, "hkpInternal.lib")
#pragma comment(lib, "hkpUtilities.lib")
#pragma comment(lib, "hkpVehicle.lib")


#include <iostream>
static void OutputHavokError(const char* error, void* errorOutputObject)
{
	std::cout << error << std::endl;
}

const int PhysicsManager::HAVOK_MEMORY = 1024 * 1024;

PhysicsManager::PhysicsManager(const float worldSize, const float timeBetweenUpdates, XMFLOAT3 gravity) : 
	mPhysicsUpdateTime(timeBetweenUpdates),
	mTimeCount(0.0f)
{
	//***************************************
	// Initialise the Havok physics engine
	// This code chunk is needed for all Havok physics applications

	// Disables a warning about setting denormal floating point values to zero (values so close to zero thay they have reduced accuracy, they also slow down calculations)
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

	//Setup havok memory (havok maintains its own memory)
	mMemoryRouter = hkMemoryInitUtil::initDefault(&mBaseMalloc, hkMemorySystem::FrameInfo(HAVOK_MEMORY));
	hkBaseSystem::init(mMemoryRouter, OutputHavokError);

	//***************************************
	// Prepare the physics world

	// Create world with various global world settings
	hkpWorldCinfo physicsInfo;
	physicsInfo.m_gravity = hkVector4(gravity.x, gravity.y, gravity.z);      // Set gravity - a vector indicating strength and direction of acceleration
	physicsInfo.setBroadPhaseWorldSize(worldSize); // Size of the physics world, bodies that leave the world are treated specially...
	physicsInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY; // ...remove bodies from physics if they go outside of the world
	physicsInfo.m_collisionTolerance = 0.05f;        // Reducing collision tolerance increases accuracy, but slows performance (default = 0.1)
	mPhysicsWorld = new hkpWorld(physicsInfo);

	// This line is necessary to enable collision detection (of all types) in Havok
	hkpAgentRegisterUtil::registerAllAgents(mPhysicsWorld->getCollisionDispatcher());
}


PhysicsManager::~PhysicsManager()
{
	//***************************************
	// Release physics bodies and world

	for (auto iter = mRigidBodies.begin(); iter != mRigidBodies.end(); iter++)
	{
		(*iter)->removeReference();
	}
	mRigidBodies.clear();
	mPhysicsWorld->removeReference();

	//***************************************
	// Uninitialise the Havok physics engine

	// This code chunk should remain the same for all basic Havok physics applications

	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();
}

void PhysicsManager::Update(float frameTime)
{
	mTimeCount += frameTime;
	//printf("Frame Time: %.8f\n", frameTime);
	// Wait a period of time before updating the physics engine
	if (mTimeCount >= mPhysicsUpdateTime)
	{
		// Simulate a period of time in the physics engine
		mPhysicsWorld->stepDeltaTime(1.0f/60.0f);

		mTimeCount = 0;	//Reset time count back to 0
	}

}

hkpRigidBody* PhysicsManager::CreateRigidBody(IModel* model, float mass)
{
	hkMassProperties massProperties;
	hkpConvexShape* shape = GetPhysicsPropertiesFromModel(model, mass, massProperties);

	// Collect together all the physics information for the ground
	hkpRigidBodyCinfo physicsInfo;
	physicsInfo.m_shape = shape;								// Use the convex shape set up above
	physicsInfo.m_mass = massProperties.m_mass;					// Set mass properties
	physicsInfo.m_centerOfMass = massProperties.m_centerOfMass;  
	physicsInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
	physicsInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;               
	physicsInfo.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;    
	
	// Create a rigid physics body using this information and add it to the physics world
	hkpRigidBody* newRigidBody = new hkpRigidBody(physicsInfo);
	mPhysicsWorld->addEntity(newRigidBody);
	mRigidBodies.push_back(newRigidBody);
	
	shape->removeReference(); // Don't need the ground shape any more, so release its memory with this function (instead of delete)

	newRigidBody->setLinearVelocity(hkVector4(.0f, 0.0f, 0.0f));
	newRigidBody->setAngularVelocity(hkVector4(.0f, 0.0f, 0.0f));


	return newRigidBody;
}

void PhysicsManager::RemoveRigidBody(hkpRigidBody* rigidBody)
{
	mRigidBodies.remove(rigidBody);
	if (rigidBody->isActive())
	{
		rigidBody->removeReference();

	}
}

hkpConvexShape* PhysicsManager::GetPhysicsPropertiesFromModel(IModel* model, float mass, hkMassProperties& massProperties)
{
	// Can get a model's scaling by analysing its (world) matrix - scaling used is the lengths of the local x, y & z axes
	float mat[16];
	model->GetMatrix(mat);
	float scaleX = sqrt(mat[0] * mat[0] + mat[1] * mat[1] + mat[2] * mat[2]);
	float scaleY = sqrt(mat[4] * mat[4] + mat[5] * mat[5] + mat[6] * mat[6]);
	float scaleZ = sqrt(mat[8] * mat[8] + mat[9] * mat[9] + mat[10] * mat[10]);

	// Extract list of vertices contained in the model's mesh
	IMesh* mesh = model->GetMesh();
	int numMeshVertices = mesh->GetNumVertices();
	float(*meshVertices)[3] = new float[numMeshVertices][3]; // Dynamic allocation of 2D array to hold vertex list (list of x,y,z)
	int i = 0;
	mesh->BeginEnumVertices();
	while (mesh->GetVertex(&meshVertices[i][0]))
	{
		// Put model scaling into mesh vertices, so Havok won't need scaling in its matrix (Havok doesn't support scaled bodies)
		meshVertices[i][0] *= scaleX;
		meshVertices[i][1] *= scaleY;
		meshVertices[i][2] *= scaleZ;
		i++;
	}
	mesh->EndEnumVertices();

	// Havok can calculate a convex hull shape from vertex list (it makes the simplest convex shape that tightly surrounds the model vertices)
	hkStridedVertices stridedVerts;
	stridedVerts.m_numVertices = numMeshVertices;
	stridedVerts.m_striding = 3 * sizeof(float);
	stridedVerts.m_vertices = &meshVertices[0][0];
	hkpConvexVerticesShape::BuildConfig config;
	config.m_convexRadius = 0;
	hkpConvexVerticesShape* shape = new hkpConvexVerticesShape(stridedVerts, config);

	// Calculate mass properties using this convex hull - centre of mass and inertial tensor
	// Put result in the parameter that was passed by reference to this function
	hkpInertiaTensorComputer::computeVertexHullVolumeMassProperties(&meshVertices[0][0], 3 * sizeof(float), numMeshVertices, mass, massProperties);

	// Finally return the convex shape
	return shape;
}