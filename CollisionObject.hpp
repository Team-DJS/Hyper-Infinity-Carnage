#pragma once

#include "Common.hpp"

namespace HIC
{
	class CollisionObject
	{
	public:
		//-----------------------------
		// Constructors/Destructors
		//-----------------------------

		// Default constructor for CollisionObject
		CollisionObject(const XMFLOAT2& centre);

		// Destructor for CollisionObject
		virtual ~CollisionObject();

	public:
		//--------------
		// Setters
		//--------------
		
		// Sets the position of the collision object
		void SetPosition(const XMFLOAT2& position);
		
		//---------------
		// Getters
		//---------------
		
		// Returns the position of the collision object
		XMFLOAT2 GetPosition() const;
		
		XMFLOAT2 GetPreviousPosition() const;

		//--------------
		// Other
		//--------------

	private:
		XMFLOAT2 mPosition;
		XMFLOAT2 mPreviousPosition;

#ifdef _DEBUG
		//--------------------
		// Collision Markers
		//--------------------
	
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