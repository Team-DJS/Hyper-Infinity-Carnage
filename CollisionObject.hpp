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
		CollisionObject(const D3DXVECTOR2& centre);

		// Destructor for CollisionObject
		virtual ~CollisionObject();

	public:
		//--------------
		// Setters
		//--------------
		
		// Sets the position of the collision object
		void SetPosition(const D3DXVECTOR2& position);
		
		//---------------
		// Getters
		//---------------
		
		// Returns the position of the collision object
		D3DXVECTOR2 GetPosition() const;
		
		D3DXVECTOR2 GetPreviousPosition() const;

		//--------------
		// Other
		//--------------

	private:
		D3DXVECTOR2 mPosition;
		D3DXVECTOR2 mPreviousPosition;

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