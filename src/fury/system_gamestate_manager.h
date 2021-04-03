/*=============================================================================

	The Fury

DESCRIPTION:
	game state manager

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
21/05/2010	initial version
=============================================================================*/

#pragma once

enum GAME_STATE
{
	GAME_STATE_UNKNOWN	= 0,
	GAME_STATE_LOADING	= 1,
	GAME_STATE_LEVEL	= 2,
};

//-----------------------------------------------------------------------------

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	int setState( const GAME_STATE& new_state );

	GAME_STATE getState() const;

private:
	GAME_STATE state;
};
//-----------------------------------------------------------------------------