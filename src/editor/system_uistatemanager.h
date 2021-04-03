/*=============================================================================

	The Fury

DESCRIPTION:
	UI state manager

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
07/05/2009	initial version
=============================================================================*/
#pragma once

//-----------------------------------------------------------------------------

enum GLOBALSTATE
{
	GS_LOADINTERFACE	= 0,
	GS_NOLEVEL			= 1,
	GS_LEVEL			= 2,
	GS_LEVELLOAD		= 3,
	GS_NEWLEVEL			= 4,
};
//-----------------------------------------------------------------------------

enum EDITSTATE
{
	ES_NOLEVEL			= 0,
	ES_FOREGROUND		= 1,
	ES_STATICS			= 2,
	ES_COLLISIONS		= 3,
	ES_BACKGROUND		= 4,
	ES_DYNAMICS			= 5,
};
//-----------------------------------------------------------------------------

class UIStateManager
{
public:
	UIStateManager();
	~UIStateManager();

	GLOBALSTATE getGlobalState() const;
	EDITSTATE	getEditLayerState() const;

	void		setGlobalState( const GLOBALSTATE& state );
	void		setEditLayerState( const EDITSTATE& state );

private:
	
	GLOBALSTATE	globalState;
	EDITSTATE	editState;
};
//-----------------------------------------------------------------------------
