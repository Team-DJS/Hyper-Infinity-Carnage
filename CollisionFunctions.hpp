#include "CollisionAABB.hpp"
#include "CollisionCylinder.hpp"
using namespace HIC;

//----------------------------------
// Collision Detection Functions
//----------------------------------

// Tests to see if the bounding cylinders supplied intersect
bool CollisionDetect(CollisionCylinder* objectA, CollisionCylinder* objectB);

// Tests to see if the cylinder instersects with the box
bool CollisionDetect(CollisionCylinder* cylinder, CollisionAABB* box);

//------------------
// Helper Functions
//------------------

// Calculate the distance vector from the first Vector2 to the second
D3DXVECTOR2 CalculateDistanceVector(const D3DXVECTOR2& from, const D3DXVECTOR2& to);