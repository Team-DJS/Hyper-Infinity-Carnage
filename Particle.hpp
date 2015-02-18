#pragma once

#include "Common.hpp"

namespace HIC
{
	class Particle
	{
	public:
		// Default constructor for Particle
		Particle(IMesh* mesh, const XMFLOAT3& position, const XMFLOAT3& velocity);

		// Destructor for Particle
		~Particle();
	public:
		// Called to update the position of the particle
		void Update(float frameTime);
	private:
		IModel* mModel;
		XMFLOAT3 mVelocity;
	};
}