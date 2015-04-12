//Definition of the Button class, a class that creates a rectangular sprite with mouse movement collision detection


#ifndef BUTTON_H	//Header Guard
#define BUTTON_H

#include "CollisionAABB.hpp"
#include "AudioManager.hpp"

namespace HIC
{
	class Button
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------

		//Pass a spritename, position and width - let the constructor figure the exact collision box out
		Button(string spriteName, D3DXVECTOR2 position, float width, float height);

		// Destructor for Button
		~Button();
	public:
		//--------------
		// Setters
		//--------------

		//--------------
		// Getters
		//--------------

		//Returns whether or not the mouse is pointing at a position on the sprite
		bool MouseIsOver();

		//--------------
		// Other
		//--------------
	public:
		static AudioSource* BUTTON_OVER_SOUND;
	private:
		ISprite* mSprite;			//The sprite that the button will appear as
		CollisionAABB mCollision;	//The collision box of the button
		bool mIsOver;
	};
}

#endif