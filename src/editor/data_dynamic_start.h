/*=============================================================================

	The Fury

DESCRIPTION:
	start point object

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
22/05/2010	initial version
=============================================================================*/

#pragma once

#include "data_dynamicobject_base.h"

//-----------------------------------------------------------------------------

class GameStartPoint: public GameDynamicObject
{
public:
	GameStartPoint();
	virtual ~GameStartPoint(){};

	virtual void serialize( FILE* stream );

	virtual void toggle(){};

	virtual GameDynamicObject* clone();

#ifndef NOEDIT
	virtual void showPropsWindow(){};
#endif
};
//-----------------------------------------------------------------------------