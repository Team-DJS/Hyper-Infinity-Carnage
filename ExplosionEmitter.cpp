#include "ExplosionEmitter.hpp"
using namespace HIC;

ExplosionEmitter::ExplosionEmitter(IMesh* mesh, D3DXVECTOR3 position, float emissionRate, float lifetime) :
	ParticleEmitter(mesh, position, emissionRate, lifetime)
{
}


ExplosionEmitter::~ExplosionEmitter()
{
}
