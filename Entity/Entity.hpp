#pragma once

#include "Common.hpp"

namespace HIC
{
	class Entity
	{
	public:
		// Default constructor for Entity
		Entity();

		// Destructor for Entity
		~Entity();
	public:
		// Called to update the entity
		virtual void OnUpdate(float frameTime) = 0;
	private:
		IModel* mModel;
	};
}
