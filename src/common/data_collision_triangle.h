#pragma once

#include "types.h"
#include "hge.h"

#include "data_collisionshape.h"

//-----------------------------------------------------------------------------

class CollisionTriangle: public CollisionShape
{
public:
	CollisionTriangle();
	virtual ~CollisionTriangle(){};

	vector2f vertices[3];
	hgeTriple triple;

	virtual void render();

#ifndef NOEDIT

	virtual bool testPoint( const vector2f& point );

	virtual int selectPoint( const vector2f& point );

	virtual void setPointPos( const unsigned& pointID, const vector2f& position );

	virtual void serialize( FILE* stream );

#else

	virtual void addToBox2D( b2World* box2d );

#endif
};