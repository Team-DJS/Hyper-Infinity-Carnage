#pragma once

#include "Common.hpp"

namespace HIC
{
	class Scenery
	{
	public:
		// default constructor for Scenery
		Scenery(IMesh* mesh, const XMFLOAT3& position);

		// destructor for Scenery
		~Scenery();

	private:
		IModel* mModel;

	};
}