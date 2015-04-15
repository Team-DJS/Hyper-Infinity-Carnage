#include "ExplosionEmitter.hpp"
using namespace HIC;

ExplosionEmitter::ExplosionEmitter(IMesh* mesh, D3DXVECTOR3 position) :
	ParticleEmitter(mesh, position)
{
}


ExplosionEmitter::~ExplosionEmitter()
{
}
