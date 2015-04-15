#pragma once

#include "ParticleEmitter.hpp"

namespace HIC
{
	class ExplosionEmitter : public ParticleEmitter
	{
	public:
		ExplosionEmitter(IMesh* mesh, D3DXVECTOR3 position);

		~ExplosionEmitter();
	};
}
