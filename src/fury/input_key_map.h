#pragma once

#include "types.h"

//-----------------------------------------------------------------------------

enum ACTIONS
{
	// gameplay actions
	ACTION_LEFT,
	ACTION_RIGHT,
	ACTION_JUMP,
	ACTION_USE,
	ACTION_MORPH,
	ACTION_UP,
	ACTION_DOWN,

	// control actions
	ACTION_EXIT,
	ACTION_SCREENSHOT,
	ACTION_PAUSE,
	ACTION_DEBUG_DRAW,
};
//-----------------------------------------------------------------------------

void	keymap_read_config();
//-----------------------------------------------------------------------------

void	keymap_write_config();
//-----------------------------------------------------------------------------

bool	keymap_get_state( const ACTIONS& action );
//-----------------------------------------------------------------------------

bool	keymap_key_up( const ACTIONS& action );
//-----------------------------------------------------------------------------

bool	keymap_key_down( const ACTIONS& action );
//-----------------------------------------------------------------------------