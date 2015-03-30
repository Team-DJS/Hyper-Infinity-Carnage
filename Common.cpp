#include "Common.hpp"


float Random(float lowBound, float highBound)
{
	return lowBound + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highBound - lowBound)));
	//return (rand() % (int)(highBound - lowBound) + lowBound);
}

float AngleBetweenVectors(const D3DXVECTOR2& vectorA, const D3DXVECTOR2& vectorB)
{
	float acosAngle = D3DXVec2Dot(&vectorA, &vectorB) / (D3DXVec2Length(&vectorA) * D3DXVec2Length(&vectorB));

	//Occasionally the value of acosAngle will return a value out of the range for this number, (-1 >= acosAngle <= 1)
	//This is merely a rounding error, to avoid this I have used this range check to reset the value within the range
	//If this code were not present it could cause a lot of error, the result of this function being NaN (not a number)
	if (acosAngle > 1.0f)
	{
		acosAngle = 1.0f;
	}
	else if (acosAngle < -1.0f)
	{
		acosAngle = -1.0f;
	}

	//If the magnitude of one of the vectors is 0. the function does not return a valid value, it returns IND (is not defined) or infinity
	//Mathematically this means that dotProd will also be 0. 0/0 is 1 so this will be the returned value, the following if statement
	//Is used to find if IND is returned
	if (acosAngle != acosAngle)
	{
		acosAngle = 1.0f;
	}

	return (float)D3DXToDegree(acosf(acosAngle));


}