#pragma once

#include "Common.hpp"

namespace HIC
{
	class CollisionObject
	{
	public:
		CollisionObject();
		virtual ~CollisionObject();

		XMFLOAT2 GetPosition();
		void SetPosition(XMFLOAT2);
	private:
		XMFLOAT2 mPosition;
	};

}