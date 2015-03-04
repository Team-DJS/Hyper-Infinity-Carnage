#include "CollisionAABB.hpp"
#include "CollisionCylinder.hpp"
using namespace HIC;

//----------------------------------
// Collision Detection Functions
//----------------------------------

// Tests to see if the bounding cylinders supplied intersect
bool CylinderToCylinderCollision(CollisionCylinder* objectA, CollisionCylinder* objectB);

// Tests to see if the cylinder instersects with the box
bool CylinderToBoxCollision(CollisionCylinder* cylinder, CollisionAABB* box);

//------------------
// Helper Functions
//------------------

// Calculate the distance vector from the first Vector2 to the second
XMFLOAT2 CalculateDistanceVector(const XMFLOAT2& from, const XMFLOAT2& to);