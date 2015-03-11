#include "Common.hpp"
//#include <Common/Base/System/Init/PlatformInit.cxx>
//#include <Common/Base/keycode.cxx>
//#include <Common/Base/Config/hkProductFeatures.cxx>



float Random(float lowBound, float highBound)
{
	return lowBound + (rand() % (int)(lowBound - highBound + 2));
}