#pragma once

#include "Common.hpp"

namespace HIC
{
	class Scenery
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------

		// Constructor for Scenery
		Scenery(IMesh* mesh, const D3DXVECTOR3& position, float scale = 1.0f);

		// Destructor for Scenery
		~Scenery();

	public:
		//--------------
		// Setters
		//--------------

		//---------------
		// Getters
		//---------------
		
		IModel* GetModel();
		
		//--------------
		// Other
		//--------------

		void LookAt(ISceneNode* at);

	private:
		IModel* mModel;
		float mScale;
	};
}