#pragma once

// Include Standard Headers
#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <unordered_map>

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
