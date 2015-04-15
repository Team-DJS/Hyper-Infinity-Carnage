#include "Button.hpp"	//Include button header file
#include "CollisionFunctions.hpp"

//-----------------------------------
// Static Initialisations
//-----------------------------------

AudioSource* Button::BUTTON_OVER_SOUND = nullptr;
AudioSource* Button::BUTTON_CLICK_SOUND = nullptr;

//-----------------------------------
// Constructors / Destructors
//-----------------------------------

//Constructor
Button::Button(string spriteName, D3DXVECTOR2 position, float width, float height) :
	mCollision(CollisionAABB(position, D3DXVECTOR2(-(width / 2), -(height / 2)), D3DXVECTOR2(width / 2, height / 2))),
	mIsOver(false)
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
	// Check if the mouse is currently over this button
	bool isCurrentlyOver = CollisionDetect(&CollisionCylinder(D3DXVECTOR2((float) gEngine->GetMouseX(), (float) gEngine->GetMouseY()), 0.0f), &mCollision);

	// Check if the mouse has just entered the button
	if (isCurrentlyOver && !mIsOver)
	{
		BUTTON_OVER_SOUND->Play();
		mIsOver = true;
	}

	// Check if the button has just left the button
	if (!isCurrentlyOver && mIsOver)
	{
		mIsOver = false;
	}

	return isCurrentlyOver;
}

//-----------------------------------
// Other
//-----------------------------------