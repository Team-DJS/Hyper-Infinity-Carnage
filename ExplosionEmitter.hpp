#pragma once
#include "ParticleEmitter.hpp"

namespace HIC
{
	class ExplosionEmitter :
		public ParticleEmitter
	{
	public:
		ExplosionEmitter(IMesh* mesh, D3DXVECTOR3 position, float emissionRate = 0.7f, float lifetime = 0.4f);
		~ExplosionEmitter();
	};
}
