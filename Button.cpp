#include "Button.hpp"	//Include button header file
#include "CollisionFunctions.hpp"

//-----------------------------------
// Static Initialisations
//-----------------------------------

I3DEngine* Button::gEngine = nullptr;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

//Constructor
Button::Button(string spriteName, XMFLOAT2 position, float width, float height) :
	mCollision(CollisionAABB(position, XMFLOAT2(-(width / 2), -(height / 2)), XMFLOAT2(width / 2, height / 2)))
{
	if (gEngine == nullptr)
	{ 
		throw std::runtime_error("Engine pointer not set, sprite load failed");
	}
	mSprite = gEngine->CreateSprite(spriteName, (mCollision.GetPosition().x + mCollision.GetMinOffset().x), (mCollision.GetPosition().y + mCollision.GetMinOffset().y));	//Use the engine to create the sprite from the file 'spriteName' in the position provided
	
	//Initialise the sprite border values
}

//Destructor
Button::~Button()	
{
	if (gEngine == nullptr)
	{
		throw std::exception("Engine pointer not set, sprite unload failed");
	}
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
	return CylinderToBoxCollision(&CollisionCylinder(XMFLOAT2((float)gEngine->GetMouseX(), (float)gEngine->GetMouseY()), 0.0f), &mCollision);
}

//-----------------------------------
// Other
//-----------------------------------