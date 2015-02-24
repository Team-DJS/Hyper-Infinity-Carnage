#pragma once

#include "Common.hpp"

namespace HIC
{
	class Scenery
	{
	public:
		// Default constructor for Scenery
		Scenery(IMesh* mesh, const XMFLOAT3& position, float scale = 1.0f);

		// Destructor for Scenery
		~Scenery();

		void LookAt(ISceneNode* at);
		IModel* GetModel();

	private:
		IModel* mModel;
		float mScale;
	};
}