#pragma once

// Include Standard Headers
#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <unordered_map>
#include <time.h>

// Include TL-Engine Headers
#include <TL-Engine.h>
using namespace tle;

extern I3DEngine* gEngine;

// Include DirectX Maths Headers
#include <DirectXMath.h>
using namespace DirectX;

// TODO: Include OpenAL Headers
#include <AL\alut.h>

// TODO: Include Physics Engine Headers
/*-----------------------------------------------------------------------------------------
	Havok include and library files
-----------------------------------------------------------------------------------------*/
//#define HK_PLATFORM_SIM // *Must* define before the includes if using the SIMD version of Havok (SIMD are advanced CPU instructions supported by all modern PCs)
//
//// Havok has many include files - the more functions and classes you use, the more files you need to include
//// Although intially less convenient, this reduces compile times over using a single catch-all include file
//#include <Common/Base/hkBase.h>
//#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
//#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
//#include <Common/Base/Fwd/hkcstdio.h>
//#include <Physics2012/Dynamics/World/hkpWorld.h>
//#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>
//#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
//#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>
//
//// We "undefine" or exclude any Havok features we're not using. Havok's linking system is over-complex, these must be correct to avoid errors
//#undef HK_FEATURE_PRODUCT_AI
//#undef HK_FEATURE_PRODUCT_ANIMATION
//#undef HK_FEATURE_PRODUCT_CLOTH
//#undef HK_FEATURE_PRODUCT_DESTRUCTION_2012
//#undef HK_FEATURE_PRODUCT_DESTRUCTION
//#undef HK_FEATURE_PRODUCT_BEHAVIOR
//#undef HK_FEATURE_PRODUCT_MILSIM
////#undef HK_FEATURE_PRODUCT_PHYSICS
//#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
//#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
//#define HK_EXCLUDE_FEATURE_MemoryTracker
//
//// These are the Havok libraries that are to be linked (the Havok code basically). It is more usual to put library names in the project settings so we don't
//// normally see them in the code like this - they are not needed for compiling our source, only for the link stage. However, this is a Microsoft-specific
//// pragma that allows us to add libraries in a similar way to header files. It's quite convinient and especially useful for Havok with it's complex requirements
//#pragma comment(lib, "hkBase.lib")
//#pragma comment(lib, "hkSceneData.lib")
//#pragma comment(lib, "hkVisualize.lib")
//#pragma comment(lib, "hkInternal.lib")
//#pragma comment(lib, "hkSerialize.lib")
//#pragma comment(lib, "hkGeometryUtilities.lib")
//#pragma comment(lib, "hkcdInternal.lib")
//#pragma comment(lib, "hkcdCollide.lib")
//#pragma comment(lib, "hkpCollide.lib")
//#pragma comment(lib, "hkpConstraint.lib")
//#pragma comment(lib, "hkpConstraintSolver.lib")
//#pragma comment(lib, "hkpDynamics.lib")
//#pragma comment(lib, "hkpInternal.lib")
//#pragma comment(lib, "hkpUtilities.lib")
//#pragma comment(lib, "hkpVehicle.lib")


// Helper function to safely delete a pointer
template<class T>
inline void SafeRelease(T* &ptr)
{
#ifdef _DEBUG
	if (ptr != nullptr)
	{
		delete(ptr);
		ptr = nullptr;
	}
#else
	delete(ptr);
	ptr = nullptr;
#endif
}

// Random number generator
float Random(float lowBound, float highBound);