#pragma once

#include "../Common.hpp"

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
		// Returns the position of the entity in world space
		XMFLOAT3 GetPosition() const;

		// Sets the position of the entity in world space
		void SetPosition(const XMFLOAT3& position);

		// Move the entity in the world axis
		void Move(float dx, float dy, float dz);

		// Point the model at a given position
		void LookAt(const XMFLOAT3& position);
	public:
		// Called to update the entity
		virtual void OnUpdate(I3DEngine* engine, float frameTime) = 0;
	protected:
		// Returns the model of the entity
		IModel* GetModel();
	private:
		IModel* mModel;
	};
}
