#pragma once

// Include Standard Headers
#include <deque>
#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <unordered_map>
#include <time.h>

// Include DirectX Maths Headers
#include <D3DX10math.h>
#include <stdint.h>

// Include TL-Engine Headers
#include <TL-Engine.h>
using namespace tle;

extern I3DEngine* gEngine;

// TODO: Include OpenAL Headers
#include <AL\alut.h>

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