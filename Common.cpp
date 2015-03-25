#include "Common.hpp"
//#include <Common/Base/System/Init/PlatformInit.cxx>
//#include <Common/Base/keycode.cxx>
//#include <Common/Base/Config/hkProductFeatures.cxx>



float Random(float lowBound, float highBound)
{
	return lowBound + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highBound - lowBound)));
	//return (rand() % (int)(highBound - lowBound) + lowBound);
}