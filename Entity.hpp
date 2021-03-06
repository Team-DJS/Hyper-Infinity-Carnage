#pragma once

#include "Common.hpp"
#include "CollisionCylinder.hpp"
#include "CollisionAABB.hpp"

namespace HIC
{
	class Entity
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------

		// Default constructor for Entity
		Entity(IMesh* mesh, const D3DXVECTOR3& position, float radius);

		// Destructor for Entity
		virtual ~Entity();
	public:
		//--------------
		// Setters
		//--------------

		// Gives health to the entity. If this exceeds the maximum health, then it is clamped
		void GiveHealth(uint32_t health);

		// Takes the given health away from the entity. If the damage is greater than the current
		// health of the entity, then this method will return true, otherwise false.
		bool TakeHealth(uint32_t damage);

		// Moves the entity in the world x-axis
		void MoveX(float dx);

		// Moves the entity in the world z-axis
		void MoveZ(float dz);

		// Rotates the entity in the world y-axis
		void RotateY(float degrees);

		// Places the entity at the provided world position
		void SetPosition(D3DXVECTOR3 newPos);

		// Resets the players health to starting health
		void ResetHealth();

		// Set the health of the entity
		void SetHealth(uint32_t health);

		//---------------
		// Getters
		//---------------

		// Returns the health of entity
		uint32_t GetHealth() const;

		// Returns the world position of the entity
		D3DXVECTOR3 GetWorldPos();

		// Returns the facing vector of the entity
		D3DXVECTOR3 GetFacingVector();

		// Returns the right vector of the entity
		D3DXVECTOR3 GetRightVector();

		// Returns the model of the entity
		IModel* GetModel();

		// Returns the CollisionCylinder of the entity
		CollisionCylinder* GetCollisionCylinder();

		//--------------
		// Other
		//--------------

		void CollisionResolution(CollisionCylinder& collidingWith);
		void CollisionResolution(CollisionAABB& collidingWith);

		// Points the model at a given position
		void LookAt(const D3DXVECTOR3& position);
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

	protected:
		//Movement variables
		D3DXVECTOR2 mVelocity; // Horizontal (x) and vertical (y) velocities

		//Constant values
		static const float BASE_THRUST;
		static const float DRAG_COEF;
		static const float TURN_SPEED;

		int mDelayed = false;
	};
}
