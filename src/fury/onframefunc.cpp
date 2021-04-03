#include "types.h"
#include "system_main.h"
#include "system_interfaces.h"
#include "data_level.h"
#include "data_staticobject.h"
#include "data_objectdata.h"
#include "helper_functions.h"
#include "input_key_map.h"
#include "sys_vars.h"
#include <time.h>

//-----------------------------------------------------------------------------

uint32 ttimer = 0;
uint32 tlastframetime = 0;
uint32 tframetime;
uint32 frametime = 1000 / FPS;

bool	key_use_down	= false;
bool	key_use_up		= false;
bool	key_morph_down	= false;
bool	key_left_down	= false;
bool	key_right_down	= false;
//-----------------------------------------------------------------------------

bool onFrameFunc()
{
	frame_tick = false;
	float timeDelta = hge->Timer_GetDelta();

	tframetime = clock();
	ttimer += tframetime - tlastframetime;
	tlastframetime = tframetime;
	if( ttimer >= frametime )
	{
		float frameDelta = (float)ttimer / 1000.0f;
		ttimer = 0;
		frame_tick = true;

		switch( stateManager->getState() )
		{
		case GAME_STATE_UNKNOWN:
			{
				stateManager->setState( GAME_STATE_LOADING );
			}
			break;
		case GAME_STATE_LOADING:
			{
				if( level && level->loaded() )
					level->run();
			}
			break;
		case GAME_STATE_LEVEL:
			{
				if( !game_pause )
				{
					physics_update();

					if( keymap_get_state(ACTION_LEFT) )
						level->player->moveLeft();
					if( keymap_get_state(ACTION_RIGHT) ) 
						level->player->moveRight();
					if( keymap_get_state(ACTION_JUMP) )
						level->player->jump();
					else
						level->player->jumpBounce();

					if( keymap_get_state(ACTION_UP) )
						if( level->player->rope_object.is_created() )
							level->player->rope_object.move_up();

					if( keymap_get_state(ACTION_DOWN) )
						if( level->player->rope_object.is_created() )
							level->player->rope_object.move_down();

					if( key_use_down )
						level->player->actionBegin();

					if( key_use_up )
						level->player->actionEnd();

					if( key_morph_down )
						level->player->toggleSelection();

					if( key_left_down )
						level->player->selection(true);

					if( key_right_down )
						level->player->selection(false);

					key_use_down = false;
					key_use_up = false;
					key_morph_down = false;
					key_right_down = false;
					key_left_down = false;
				}

				if( level )
					level->update(frameDelta);
			}
			break;
		default:
			{
			}
			break;
		}
	}

	fadeUpdate(timeDelta);

	gui_system->update();

	log_update(timeDelta);

	if( keymap_key_down(ACTION_SCREENSHOT) )
		hge->System_Snapshot();

	if( keymap_key_down(ACTION_EXIT) )
		return true;

	if( keymap_key_down(ACTION_PAUSE) )
		game_pause = !game_pause;

	if( keymap_key_down(ACTION_USE) )
		key_use_down = true;

	if( keymap_key_up(ACTION_USE) )
		key_use_up = true;

	if( keymap_key_down(ACTION_MORPH) )
		key_morph_down = true;

	if( keymap_key_down(ACTION_LEFT) )
		key_left_down = true;

	if( keymap_key_down(ACTION_RIGHT) )
		key_right_down = true;

	if( keymap_key_down(ACTION_DEBUG_DRAW) )
		debug_render = !debug_render;

	return false;
}
//-----------------------------------------------------------------------------