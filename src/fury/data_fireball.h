/*=============================================================================

	The Fury

DESCRIPTION:
	fireball

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
31/05/2010	initial version
=============================================================================*/

#pragma once

#include <hgeparticle.h>
#include "data_effect.h"
#include "data_objectdata.h"

//-----------------------------------------------------------------------------

class GameFireball: public GameEffect
{
public:
	GameFireball();
	virtual ~GameFireball();

	// physics
	b2Body*		body;

	bool splash;

	ObjectData	data;

	vector2f direction;
	
	virtual void update( const float& delta );

	virtual void fire( const vector2f& position, const float& scale = 1.0f );

	virtual void onCollision( b2Fixture* fix, contact_point* point );
};
//-----------------------------------------------------------------------------
