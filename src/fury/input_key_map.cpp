#include "types.h"
#include <map>
#include "str_helpers.h"
#include "system_interfaces.h"
#include "input_key_map.h"

using namespace std;
//-----------------------------------------------------------------------------

map<ACTIONS, int16>	key_bindings;
//-----------------------------------------------------------------------------

void keymap_read_config()
{
	assert(hge);

	key_bindings[ACTION_LEFT] = (int16)strtol(hge->Ini_GetString("KEYS", "left", "25"), 0, 16);
	key_bindings[ACTION_RIGHT] = (int16)strtol(hge->Ini_GetString("KEYS", "right", "27"), 0, 16);
	key_bindings[ACTION_JUMP] = (int16)strtol(hge->Ini_GetString("KEYS", "jump", "26"), 0, 16);
	key_bindings[ACTION_USE] = (int16)strtol(hge->Ini_GetString("KEYS", "action", "20"), 0, 16);
	key_bindings[ACTION_MORPH] = (int16)strtol(hge->Ini_GetString("KEYS", "morph", "28"), 0, 16);
	key_bindings[ACTION_EXIT] = (int16)strtol(hge->Ini_GetString("KEYS", "exit", "1B"), 0, 16);
	key_bindings[ACTION_SCREENSHOT] = (int16)strtol(hge->Ini_GetString("KEYS", "screenshot", "79"), 0, 16);
	key_bindings[ACTION_PAUSE] = (int16)strtol(hge->Ini_GetString("KEYS", "pause", "50"), 0, 16);
	key_bindings[ACTION_UP] = (int16)strtol(hge->Ini_GetString("KEYS", "up", "26"), 0, 16);
	key_bindings[ACTION_DOWN] = (int16)strtol(hge->Ini_GetString("KEYS", "down", "28"), 0, 16);

	key_bindings[ACTION_DEBUG_DRAW] = HGEK_F1;
}
//-----------------------------------------------------------------------------

bool keymap_get_state( const ACTIONS& action )
{
	return hge->Input_GetKeyState( key_bindings[action] );
}
//-----------------------------------------------------------------------------

bool keymap_key_up( const ACTIONS& action )
{
	return hge->Input_KeyUp( key_bindings[action] );
}
//-----------------------------------------------------------------------------

bool keymap_key_down( const ACTIONS& action )
{
	return hge->Input_KeyDown( key_bindings[action] );
}
//-----------------------------------------------------------------------------

void keymap_write_config()
{
	assert(hge);

	hge->Ini_SetString("KEYS", "left", byte2hexstr((char)key_bindings[ACTION_LEFT]));
	hge->Ini_SetString("KEYS", "right", byte2hexstr((char)key_bindings[ACTION_RIGHT]));
	hge->Ini_SetString("KEYS", "jump", byte2hexstr((char)key_bindings[ACTION_JUMP]));
	hge->Ini_SetString("KEYS", "action", byte2hexstr((char)key_bindings[ACTION_USE]));
	hge->Ini_SetString("KEYS", "morph", byte2hexstr((char)key_bindings[ACTION_MORPH]));
	hge->Ini_SetString("KEYS", "exit", byte2hexstr((char)key_bindings[ACTION_EXIT]));
	hge->Ini_SetString("KEYS", "screenshot", byte2hexstr((char)key_bindings[ACTION_SCREENSHOT]));
	hge->Ini_SetString("KEYS", "pause", byte2hexstr((char)key_bindings[ACTION_PAUSE]));
	hge->Ini_SetString("KEYS", "up", byte2hexstr((char)key_bindings[ACTION_UP]));
	hge->Ini_SetString("KEYS", "down", byte2hexstr((char)key_bindings[ACTION_DOWN]));
}
//-----------------------------------------------------------------------------