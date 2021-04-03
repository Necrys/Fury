/*=============================================================================

	The Fury

DESCRIPTION:
	basic collision shape class

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
15/05/2010	initial version
=============================================================================*/

#pragma once

#include "types.h"
#include "data_level.h"
#ifdef NOEDIT
#include "data_objectdata.h"
#endif
//-----------------------------------------------------------------------------

enum PHYSIC_OBJ
{
	PHYSIC_OBJ_SQUARE = 0,
	PHYSIC_OBJ_TRIANGLE = 1,
	PHYSIC_OBJ_CIRCLE = 2,
};
//-----------------------------------------------------------------------------

class CollisionShape
{
public:
	CollisionShape();
	virtual ~CollisionShape(){};

	uint32		borderColor;
	uint32		fillColor;

	virtual void render() = 0;

#ifndef NOEDIT

	bool	selected;
	uint32	borderSelectedColor;
	uint32	fillSelectedColor;
	uint32	pointColor;

	virtual bool testPoint( const vector2f& point ) = 0;

	virtual int selectPoint( const vector2f& point ) = 0;

	virtual void setPointPos( const unsigned& pointID, const vector2f& position ) = 0;

	virtual void serialize( FILE* stream ) = 0;

#else

	ObjectData	data;

	virtual void addToBox2D( b2World* box2d ) = 0;

#endif
};
//-----------------------------------------------------------------------------