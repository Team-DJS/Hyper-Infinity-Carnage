#pragma once

#include "Common.hpp"

namespace HIC
{
	class Scenery
	{
	public:
		// Default constructor for Scenery
		Scenery(IMesh* mesh, const XMFLOAT3& position);

		// Destructor for Scenery
		~Scenery();

	private:
		IModel* mModel;

	};
}