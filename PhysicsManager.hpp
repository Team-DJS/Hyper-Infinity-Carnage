#pragma once

#define HK_PLATFORM_SIM // Define before the includes if using the SIMD version of Havok (SIMD are advanced CPU instructions supported by all modern PCs)

#include <Common/Base/hkBase.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Fwd/hkcstdio.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Physics2012/Dynamics/World/hkpWorld.h>
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics2012/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics2012/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Physics2012/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics2012/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

#include <Common/Base/System/Init/PlatformInit.cxx>

#include <Physics2012/Dynamics/Collide/ContactListener/hkpContactListener.h>

#include "Common.hpp"


class PhysicsManager
{
private:
	// Havok initialisation variables
	hkMallocAllocator mBaseMalloc;
	hkMemoryRouter*   mMemoryRouter;
	static const int  HAVOK_MEMORY;		// Amount of memory to reserve for Havok

	//Havok world data
	hkpWorld* mPhysicsWorld;

	//List of rigid bodies
	list<hkpRigidBody*> mRigidBodies;

	// Physics timing
	float mTimeCount;				// Length of time since last tick
	const float mPhysicsUpdateTime;	// Length of time to wait between updates of the physics engine

public:
	PhysicsManager(const float worldSize, const float timeBetweenUpdates, XMFLOAT3 gravity = XMFLOAT3(0.0f, -9.8f, 0.0f));
	~PhysicsManager();
	
	void Update(float frameTime);

	hkpRigidBody* CreateRigidBody(IModel* model, float mass);

	void RemoveRigidBody(hkpRigidBody* rigidBody);

private:
	hkpConvexShape* GetPhysicsPropertiesFromModel(IModel* model, float mass, hkMassProperties& massProperties);

};