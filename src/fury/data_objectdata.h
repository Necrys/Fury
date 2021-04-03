/*=============================================================================

	The Fury

DESCRIPTION:
	object description

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
22/05/2010	initial version
=============================================================================*/

#pragma once

//-----------------------------------------------------------------------------

enum OBJECT_TYPE
{
	OBJECT_UNKNOWN		= 0,
	OBJECT_STATIC		= 1,
	OBJECT_PLAYER		= 2,
	OBJECT_MOVABLE		= 3,
	OBJECT_EFFECT		= 4,
	OBJECT_DESTRUCTABLE = 5,
	OBJECT_CRUNCH		= 6,
	OBJECT_DOOR			= 7,
	OBJECT_KEY			= 8,
	OBJECT_COIN			= 9,
	OBJECT_TRIGGER		= 10,
};

//-----------------------------------------------------------------------------

struct ObjectData
{
	bool	isDynamic;
	bool	isLift;
	bool	isWater;
	bool	isExitZone;
	bool	isFireball;
	bool	isPhysical;
	bool	isSensor;

	OBJECT_TYPE	type;

	void*	objectPtr;

	ObjectData()
	{
		type = OBJECT_UNKNOWN;
		isDynamic = false;
		isLift = false;
		isWater = false;
		isExitZone = false;
		isFireball = false;
		isPhysical = true;
		isSensor	= false;

		objectPtr = 0;
	}
};
//-----------------------------------------------------------------------------
