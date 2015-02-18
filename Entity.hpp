#pragma once

#include "Common.hpp"
#include "CollisionCylinder.hpp"

namespace HIC
{
	class Entity
	{
	public:
		// Default constructor for Entity
		Entity(IMesh* mesh, const XMFLOAT3& position);

		// Destructor for Entity
		virtual ~Entity();
	public:
		// Returns the health of entity
		uint32_t GetHealth() const;

		// Gives health to the entity. If this exceeds the maximum health, then it is clamped
		void GiveHealth(uint32_t health);

		// Takes the given health away from the entity. If the damage is greater than the current
		// health of the entity, then this method will return true, otherwise false.
		bool TakeHealth(uint32_t damage);

		// Points the model at a given position
		void LookAt(const XMFLOAT3& position);

		// Moves the entity in the world x-axis
		void MoveX(float dx);

		// Moves the entity in the world z-axis
		void MoveZ(float dz);

		// Rotates the entity in the world y-axis
		void RotateY(float degrees);

		// Returns the facing vector of the entity
		XMFLOAT3 GetFacingVector();

		// Returns the right vector of the entity
		XMFLOAT3 GetRightVector();

		// Returns the model of the entity
		IModel* GetModel();

		// Returns the CollisionCylinder of the entity
		CollisionCylinder& GetCollisionCylinder();
	public:
		// Called to update the entity
		virtual void Update(float frameTime) = 0;
	private:
		static const uint32_t DEFAULT_HEALTH;
		static const uint32_t DEFAULT_MAX_HEALTH;
	private:
		IMesh* mMesh;
		IModel* mModel;
		uint32_t mHealth;
		uint32_t mMaxHealth;
		CollisionCylinder mCollisionCylinder;
	};
}
