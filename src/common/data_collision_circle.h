/*=============================================================================

	The Fury

DESCRIPTION:
	circle collision shape class

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
16/05/2010	initial version
=============================================================================*/

#pragma once

#include "types.h"

#include "data_collisionshape.h"

//-----------------------------------------------------------------------------

const unsigned circleTriangles = 32;

class CollisionCircle: public CollisionShape
{
public:
	CollisionCircle();
	virtual ~CollisionCircle(){};

	vector2f center;
	vector2f radius;
	hgeTriple triangles[circleTriangles];

	virtual void render();

	virtual void rebuildBorders();

#ifndef NOEDIT

	virtual bool testPoint( const vector2f& point );

	virtual int selectPoint( const vector2f& point );

	virtual void setPointPos( const unsigned& pointID, const vector2f& position );

	virtual void serialize( FILE* stream );

#else

	virtual void addToBox2D( b2World* box2d );

#endif

};