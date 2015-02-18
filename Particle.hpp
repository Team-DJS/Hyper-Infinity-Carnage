#pragma once

#include "Common.hpp"

namespace HIC
{
	class Particle
	{
	public:
		// Default constructor for the Particle
		Particle(IMesh* mesh, XMFLOAT3& position, XMFLOAT3 velocity);

		// Default constructor for the Particle
		~Particle();
	
	public:
		// Called to update the position of the particle
		void Update(float frameTime);

	private:

		IModel* mModel;
		XMFLOAT3 mVelocity;
	};

}