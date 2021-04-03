
#include "types.h"
#include <string>
#include "data_level.h"
#include "data_staticobject.h"
#include "data_tileset.h"
#include "types.h"

using namespace std;
//-----------------------------------------------------------------------------

uint32		screen_width	= 800;
uint32		screen_height	= 600;
uint8		screen_bpp		= 32;
bool		windowed		= true;
bool		show_fps		= true;
bool		vsync			= false;
//-----------------------------------------------------------------------------

vector2f	global_position(0.0f, 0.0f);
vector2f	camera_position;
hgeRect		camera_rect;
//-----------------------------------------------------------------------------

bool		game_pause		= false;
bool		frame_tick		= false;
bool		debug_render	= false;
//-----------------------------------------------------------------------------

hgeRect		phys_world_bounds(0.0f, 0.0f, 0.0f, 0.0f);
vector2f	phys_gravity(0.0f, 1600.0f);
//-----------------------------------------------------------------------------

float		globalScale		= 1.0f;
string		appDirectory	= "";
string		levelFile		= "default.flf";
GameLevel*	level			= 0;
Tileset*	g_tileset		= 0;
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------