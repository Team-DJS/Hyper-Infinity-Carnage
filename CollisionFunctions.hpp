#include "CollisionAABB.hpp"
#include "CollisionCylinder.hpp"
using namespace HIC;

XMFLOAT2 CalculateDistanceVector(XMFLOAT2 from, XMFLOAT2 to)
{
	return XMFLOAT2(to.x - from.x, to.y - from.y);
}

bool CylinderToCylinderCollision(CollisionCylinder* objectA, CollisionCylinder* objectB)	//***Tests to see if the bounding cylinders supplied intersect
{																						//A cylinders object will be treated as a point if the radius = 0;
	bool hasColided = false;
	float distBetweenPoints = 0.0f;
	XMFLOAT2 vect = CalculateDistanceVector(objectA->GetPosition(), objectB->GetPosition());

	distBetweenPoints = (vect.x * vect.x) + (vect.y * vect.y);	//Calculates the magnitude of the vector between the two models

	return distBetweenPoints <= (objectA->GetRadius() + objectB->GetRadius()) * (objectA->GetRadius() + objectB->GetRadius());	//If the bounding radii of the objects intersect
}

bool CylinderToBoxCollision(CollisionCylinder* cylinder, CollisionAABB* box)	//***Tests to see if the cylinder instersects with the box
{																			//Giving radius 0 for the sphere treats this collision point to box collision
	bool hasCollided = false;

	float radius = cylinder->GetRadius();

	XMFLOAT2 min = XMFLOAT2(box->GetMinPosition().x - radius, box->GetMinPosition().y - radius);
	XMFLOAT2 max = XMFLOAT2(box->GetMaxPosition().x + radius, box->GetMaxPosition().y + radius);

	XMFLOAT2 spherePos = cylinder->GetPosition();

	if (spherePos.x >= min.x && spherePos.x <= max.x)
	{
		if (spherePos.y >= min.y && spherePos.y <= max.y)
		{
			hasCollided = true;
		}
	}
	return hasCollided;
}