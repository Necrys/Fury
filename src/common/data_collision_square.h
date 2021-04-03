#pragma once

#include "hgerect.h"
#include "hge.h"

#include "data_collisionshape.h"

//-----------------------------------------------------------------------------
class CollisionSquare: public CollisionShape
{
public:
	CollisionSquare();
	virtual ~CollisionSquare(){};

	hgeRect bounds;
	hgeQuad	quad;

	virtual void render();

#ifndef NOEDIT

	virtual bool testPoint( const vector2f& point );

	virtual int	 selectPoint( const vector2f& point );

	virtual void setPointPos( const unsigned& pointID, const vector2f& position );

	virtual void serialize( FILE* stream );

private:
	void correctBounds();

#else

	virtual void addToBox2D( b2World* box2d );

#endif

};