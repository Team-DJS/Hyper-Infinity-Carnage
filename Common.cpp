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

D3DXVECTOR3 WorldPosFromPixel()
{
	D3DXVECTOR4 q;
	D3DXVECTOR2 pixelPt = D3DXVECTOR2(gEngine->GetMouseX(), gEngine->GetMouseY());

	//Convert to a -1 to 1 coord system from pixel position
	q.x = pixelPt.x / (gEngine->GetWidth() / 2) - 1;
	q.y = 1 - pixelPt.y / (gEngine->GetHeight() / 2);

	q.w = gNearClip;	//Near clip distance
	q.z = 0;			//Closest depth buffer value

	//Undo perspective
	q.x *= q.w;
	q.y *= q.w;
	q.z *= q.w;

	D3DXMATRIX worldMatrix;
	gGameCamera->GetMatrix(worldMatrix);

	D3DXMATRIX viewMatrix;

	D3DXMatrixInverse(&viewMatrix, NULL, &worldMatrix);

	D3DXMATRIX projectionMatrix;
	float aspect = (float)gEngine->GetWidth() / gEngine->GetHeight();
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 3.4f, aspect, gNearClip, gFarClip);

	//Multiply by inverse viewprojmatrix to convert from camera space to world space
	D3DXMATRIX viewProj = viewMatrix * projectionMatrix;
	D3DXMatrixInverse(&viewProj, 0, &viewProj);	//Inverse the viewproj matrix
	D3DXVec4Transform(&q, &q, &viewProj);
	
	//Create floor plane at origin
	D3DXPLANE floor;
	D3DXPlaneFromPointNormal(&floor, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		
	//Calculate the position on the plane that the ray was cast to
	D3DXVECTOR3 rayPos;
	D3DXPlaneIntersectLine(&rayPos, &floor, &D3DXVECTOR3(q), &D3DXVECTOR3(gGameCamera->GetX(), gGameCamera->GetY(), gGameCamera->GetZ()));
	

	return rayPos; //Returns the position on the plane that the ray was cast to
}
