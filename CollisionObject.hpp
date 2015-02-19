#pragma once

#include "Common.hpp"

namespace HIC
{
	class CollisionObject
	{
	public:
		// Default constructor for CollisionObject
		CollisionObject(XMFLOAT2 centre);

		// Destructor for CollisionObject
		virtual ~CollisionObject();
	public:
		// Returns the position of the collision object
		XMFLOAT2 GetPosition() const;

		// Sets the position of the collision object
		void SetPosition(const XMFLOAT2& position);
	private:
		XMFLOAT2 mPosition;

#ifdef _DEBUG
		//Collision Markers - Used to see where the collision is
	public:
		static IMesh* MARKER_MESH;
	protected:
		static const float MARKER_SCALE;
		bool mMarkersExist;							//Says whether or not the collision markers exist
		float mHeight;								//The height at which the collision markers will be drawn
		vector<IModel*> mCollisionMarkers;			//Vector of collision markers
	public:
		virtual void ToggleMarkers() = 0;	//Toggles whether or not this markers' mesh
#endif
	};
}