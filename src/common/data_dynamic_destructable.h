/*=============================================================================

	The Fury

DESCRIPTION:
	destructable box

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
01/06/2010	initial version
=============================================================================*/

#pragma once


#include "data_dynamicobject_base.h"
#include "data_tileset.h"

#ifdef NOEDIT
#include "data_objectdata.h"
#endif

//-----------------------------------------------------------------------------

class GameDestructable: public GameDynamicObject
{
public:
	GameDestructable( Tile* _tile );
	virtual ~GameDestructable();

	virtual void serialize( FILE* stream );

	virtual GameDynamicObject* clone();

	virtual void toggle(){};

	float friction;
	float restitution;

	float health;

	virtual void setPosition( const vector2f& pos );

#ifdef NOEDIT

	virtual void update( const float& delta );

	virtual void initInBox2d();

	virtual void hit( const float& power );

	virtual void render( const bool& shadowed = false );

	b2Body*			body;

	ObjectData		data;

#else
	virtual void showPropsWindow(){};

#endif

private:
	Tile* tile;

	float hitFadeTimer;
};
//-----------------------------------------------------------------------------
