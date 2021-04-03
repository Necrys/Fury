/*=============================================================================

	The Fury

DESCRIPTION:
	movable box

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
04/05/2010	initial version
=============================================================================*/

#pragma once


#include "data_dynamicobject_base.h"
#include "data_tileset.h"

#ifdef NOEDIT
#include "data_objectdata.h"
#endif

//-----------------------------------------------------------------------------

class GameMovableBox: public GameDynamicObject
{
public:
	GameMovableBox( Tile* tile );
	~GameMovableBox();

	virtual void serialize( FILE* stream );

	virtual GameDynamicObject* clone();

	float density;
	float friction;
	float restitution;

	virtual void toggle(){};

	virtual void setPosition( const vector2f& pos );


#ifdef NOEDIT

	virtual void update( const float& delta );

	virtual void initInBox2d();

	b2Body*			body;

	ObjectData		data;
#else
	virtual void showPropsWindow(){};

#endif

private:
	Tile* tile;
};