//Definition of the Button class, a class that creates a rectangular sprite with mouse movement collision detection


#ifndef BUTTON_H	//Header Guard
#define BUTTON_H	

#include "CollisionAABB.hpp"
using namespace HIC;
/*--------------------------------------Class - CButton--------------------------------------*/

class Button
{
private:
	ISprite* mSprite;			//The sprite that the button will appear as
	CollisionAABB mCollision;	//The collision box of the button

public:
	static I3DEngine* gEngine;

	//Pass a spritename, position and width - let the constructor figure the rest out
	Button(string spriteName, CollisionAABB collisionBox);	//The object constructor

	~Button();					//The object destructor
	
	bool MouseIsOver();			//Function that says if the mouse is pointing at a position on the sprite
	
	//bool ButtonClicked();		//Function that says if the button has been clicked or not
};

/************************************End of Class - CButton**********************************/


#endif