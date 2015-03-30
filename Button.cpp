#include "Button.hpp"	//Include button header file
#include "CollisionFunctions.hpp"

//-----------------------------------
// Static Initialisations
//-----------------------------------

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

//Constructor
Button::Button(string spriteName, D3DXVECTOR2 position, float width, float height) :
	mCollision(CollisionAABB(position, D3DXVECTOR2(-(width / 2), -(height / 2)), D3DXVECTOR2(width / 2, height / 2)))
{
	mSprite = gEngine->CreateSprite(spriteName, (mCollision.GetPosition().x + mCollision.GetMinOffset().x), (mCollision.GetPosition().y + mCollision.GetMinOffset().y));	//Use the engine to create the sprite from the file 'spriteName' in the position provided
	
	//Initialise the sprite border values
}

//Destructor
Button::~Button()	
{
	gEngine->RemoveSprite(mSprite);	//Remove the sprite from memory
}

//-----------------------------------
// Setters
//-----------------------------------

//-----------------------------------
// Getters
//-----------------------------------

//Checks to see if the mouse is currently on top of the sprite
bool Button::MouseIsOver()	
{
	return CylinderToBoxCollision(&CollisionCylinder(D3DXVECTOR2((float)gEngine->GetMouseX(), (float)gEngine->GetMouseY()), 0.0f), &mCollision);
}

//-----------------------------------
// Other
//-----------------------------------