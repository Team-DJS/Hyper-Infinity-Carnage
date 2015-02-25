//Creation of the bodies of the functions belonging to the CButton class

#include "Button.hpp"	//Include button header file

/*--------------------------------------Class - CButton--------------------------------------*/

I3DEngine* Button::gEngine = nullptr;

Button::Button(string spriteName, CollisionAABB collisionBox) ://Constructor
	mCollision(collisionBox)
{
	if (gEngine == nullptr)
	{ 
		throw std::runtime_error("Engine pointer not set, sprite load failed");
	}
	mSprite = gEngine->CreateSprite(spriteName, (mCollision.GetPosition().x - mCollision.GetMinOffset().x), (mCollision.GetPosition().y - mCollision.GetMinOffset().y));	//Use the engine to create the sprite from the file 'spriteName' in the position provided
	
	//Initialise the sprite border values
}

Button::~Button()	//Destructor
{
	if (gEngine == nullptr)
	{
		throw std::exception("Engine pointer not set, sprite unload failed");
	}
	gEngine->RemoveSprite(mSprite);	//Remove the sprite from memory
}

bool Button::MouseIsOver()	//Checks to see if the mouse is currently on top of the sprite
{
	return true;
	//if(gameEnginePtr->GetMouseX() < maxX && gameEnginePtr->GetMouseX() > minX)	//If mouse within box x boundaries
	//{
	//	if(gameEnginePtr->GetMouseY() < maxY && gameEnginePtr->GetMouseY() > minY)	//If mouse within box y boundaries
	//	{
	//		return true;	//Mouse is over the box
	//	}
	//}
	//return false;	//Mouse is not over the box
}

/************************************End of Class - CButton**********************************/