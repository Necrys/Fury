/*=============================================================================

	The Fury

DESCRIPTION:
	dynamic game object base class

	Kulinich Alexander � 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
22/05/2010	initial version
=============================================================================*/


#include "data_dynamicobject_base.h"

//-----------------------------------------------------------------------------

GameDynamicObject::GameDynamicObject()
{
	type = DYNAMIC_UNKNOWN;
	dead = false;
	_switchable = false;
}
//-----------------------------------------------------------------------------