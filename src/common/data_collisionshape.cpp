/*=============================================================================

	The Fury

DESCRIPTION:
	basic collision shape class

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
16/05/2010	initial version
=============================================================================*/


#include "data_collisionshape.h"

//-----------------------------------------------------------------------------

CollisionShape::CollisionShape()
{
	borderColor			= 0xFF4CFF00;
	fillColor			= 0x99267F00;
#ifndef NOEDIT
	fillSelectedColor	= 0x9938BC00;
	borderSelectedColor	= 0xFF4CFF00;
	pointColor			= 0xFFFFD800;
	selected			= false;
#else
	data.objectPtr		= (void*)this;
	data.type			= OBJECT_STATIC;
#endif
}
//-----------------------------------------------------------------------------
