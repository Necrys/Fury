/*=============================================================================

	The Fury

DESCRIPTION:
	dynamic game object base class

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
22/05/2010	initial version
=============================================================================*/

#pragma once

#include "data_staticobject.h"
#include "ui_main.h"
#include "ui_window.h"
#include "ui_label.h"
#include "ui_editbox.h"
#include "ui_imagebutton.h"

//-----------------------------------------------------------------------------

enum DYNAMIC_TYPE
{
	DYNAMIC_UNKNOWN			= 0,
	DYNAMIC_START_POINT		= 1,
	DYNAMIC_MOVEABLE_BOX	= 2,
	DYNAMIC_DESTRUCTABLE	= 3,
	DYNAMIC_CRUNCH			= 4,
	DYNAMIC_POINT			= 5,
	DYNAMIC_LIFT			= 6,
	DYNAMIC_DOOR			= 7,
	DYNAMIC_KEY				= 8,
	DYNAMIC_COIN			= 9,
	DYNAMIC_TRIGGER			= 10,
};
//-----------------------------------------------------------------------------

class GameDynamicObject: public GameStaticObject
{
public:
	GameDynamicObject();
	virtual ~GameDynamicObject(){};

	DYNAMIC_TYPE type;

	virtual void				serialize( FILE* stream ) = 0;
	virtual bool				switchable() const { return _switchable; };
	virtual GameDynamicObject*	clone() = 0;
			bool				isDead() const{ return dead; };

#ifdef NOEDIT
	virtual void				initInBox2d() = 0;
	virtual void				toggle() = 0;
#else
	virtual void				showPropsWindow() = 0;
#endif

protected:

	bool dead;
	bool _switchable;
};
//-----------------------------------------------------------------------------