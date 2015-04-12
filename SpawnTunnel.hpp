#pragma once

#include "Common.hpp"
#include "Timer.hpp"

namespace HIC
{
	class SpawnTunnel
	{
	public:
		SpawnTunnel(D3DXVECTOR3 pos);
		~SpawnTunnel();

		static IMesh* SPAWN_TUNNEL_MESH;

		bool Update(float frameTime);

		void SetPosition(D3DXVECTOR3 pos);

	private:

		Timer mTimer;

		float mSize;

		IModel* mModel;
	};
}
