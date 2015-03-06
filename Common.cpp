#include "Common.hpp"

float Random(float lowBound, float highBound)
{
	return lowBound + (rand() % (int)(lowBound - highBound + 2));
}