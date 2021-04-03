#pragma once

#include "types.h"

//-----------------------------------------------------------------------------

enum CONTACTSTATE
{
	CS_ADDED,
	CS_PERSISTED,
	CS_REMOVED,
};
//-----------------------------------------------------------------------------

struct contact_point
{
	b2Contact*		contact;
	b2Fixture*		shape1;
	b2Fixture*		shape2;
	b2Vec2			normal;
	b2Vec2			position;
	b2Vec2			velocity;
	b2ContactID		id;
	CONTACTSTATE	state;
};
//-----------------------------------------------------------------------------

void collision_detector_init();
//-----------------------------------------------------------------------------

void collisions_update();
//-----------------------------------------------------------------------------

void physics_update();
//-----------------------------------------------------------------------------