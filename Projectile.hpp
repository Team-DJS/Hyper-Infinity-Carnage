#pragma once

#include "Common.hpp"

class Projectile
{
public:
	Projectile();
	~Projectile();

	uint32_t GetDamage();
	void Update();

private:
	uint32_t mDamage;
	IModel* mModel;
};

