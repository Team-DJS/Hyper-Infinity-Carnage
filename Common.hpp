#pragma once

// Include Standard Headers
#include <ctime>
#include <deque>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <exception>
#include <unordered_map>

// Include OpenAL Headers
#include <AL\alut.h>

// Include DirectX Maths Headers
#include <D3DX10math.h>

// Include TL-Engine Headers
#include <TL-Engine.h>
using namespace tle;

extern I3DEngine* gEngine;

// Helper function to safely delete a pointer
template<class T>
inline void SafeRelease(T*& ptr)
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

float AngleBetweenVectors(const D3DXVECTOR2& vectorA, const D3DXVECTOR2& vectorB);	//Returns an angle in degrees
