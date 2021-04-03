#pragma once

#include "data_dynamicobject_base.h"
#include "data_tileset.h"

#ifdef NOEDIT
#include "data_objectdata.h"
#endif

//-----------------------------------------------------------------------------

class GameItemCoin: public GameDynamicObject
{
public:

	GameItemCoin( Tile* tile );
	virtual ~GameItemCoin();

	virtual void serialize( FILE* stream );

	virtual GameDynamicObject* clone();

	virtual void setPosition( const vector2f& pos );

	unsigned int value;

#ifdef NOEDIT

	virtual void initInBox2d();

	virtual void toggle(){};

	virtual void onCollision();

	//virtual void render( const bool& shadowed = false );

	b2Body*			body;

	ObjectData		data;

#endif

#ifndef NOEDIT
	virtual void showPropsWindow(){};
#endif

private:
	Tile* tile;
};
//-----------------------------------------------------------------------------
