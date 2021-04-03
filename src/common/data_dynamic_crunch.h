#pragma once


#include "data_dynamicobject_base.h"
#include "data_tileset.h"
#include "str_helpers.h"

#ifdef NOEDIT
#include "data_objectdata.h"
#else
#include "dialog_crunch.h"
#endif

//-----------------------------------------------------------------------------

class GameCrunch: public GameDynamicObject
{
public:
	GameCrunch( Tile* _tile );
	virtual ~GameCrunch();

	virtual void serialize( FILE* stream );

	virtual GameDynamicObject* clone();

	virtual void toggle(){};

	float friction;
	float restitution;

	unsigned health;

	virtual void setPosition( const vector2f& pos );

#ifdef NOEDIT

	virtual void update( const float& delta );

	virtual void initInBox2d();

	virtual void hit();

	virtual void render( const bool& shadowed = false );

	b2Body*			body;

	ObjectData		data;
#else
//	static dlg_crunch_options*	propsWindow;
	virtual void showPropsWindow();
#endif

private:
	Tile* tile;

	hgeAnimation*	masks[4];
};
//-----------------------------------------------------------------------------