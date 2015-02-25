#include "CollisionFunctions.hpp"

//----------------------------------
// Collision Detection Functions
//----------------------------------

// Tests to see if the bounding cylinders supplied intersect
bool CylinderToCylinderCollision(CollisionCylinder* objectA, CollisionCylinder* objectB)	
{
	bool hasColided = false;
	float distBetweenPoints = 0.0f;
	XMFLOAT2 vect = CalculateDistanceVector(objectA->GetPosition(), objectB->GetPosition());

	distBetweenPoints = (vect.x * vect.x) + (vect.y * vect.y);	//Calculates the magnitude of the vector between the two models

	return distBetweenPoints <= (objectA->GetRadius() + objectB->GetRadius()) * (objectA->GetRadius() + objectB->GetRadius());	//If the bounding radii of the objects intersect
}

// Tests to see if the cylinder instersects with the box
bool CylinderToBoxCollision(CollisionCylinder* cylinder, CollisionAABB* box)
{																				
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

//------------------
// Helper Functions
//------------------

// Calculate the distance vector from the first Vector2 to the second
XMFLOAT2 CalculateDistanceVector(XMFLOAT2 from, XMFLOAT2 to)	
{
	return XMFLOAT2(to.x - from.x, to.y - from.y);
}