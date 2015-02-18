#pragma once

#include "Entity.hpp"

namespace HIC
{
	class Enemy : public Entity
	{
	public:
		// Default constructor for Enemy
		Enemy(IMesh* mesh, const XMFLOAT3& position, float radius, uint32_t damage);

		// Destructor for Enemy
		~Enemy();
	public:
		// Returns the damage the enemy deals
		uint32_t GetDamage() const;

		// Called to update the entity
		void Update(float frameTime);
	private:
		uint32_t mDamage;
	};
}
