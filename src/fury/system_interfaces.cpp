#include "types.h"
#include "hgefont.h"
#include "system_interfaces.h"
#include "system_main.h"
#include "str_helpers.h"
#include "input_key_map.h"
#include "sys_vars.h"
#include "system_paths.h"

//using namespace audiere;

//-----------------------------------------------------------------------------

HGE*				hge = 0;
hgeFont*			default_font = 0;

b2World*			box2d = 0;
//AudioDevicePtr		audio = 0;
GameStateManager*	stateManager = 0;
//-----------------------------------------------------------------------------

void readConfig()
{
	if( !hge )
		return;

	screen_width = hge->Ini_GetInt("GFX", "width", screen_width);
	screen_height = hge->Ini_GetInt("GFX", "height", screen_height);
	screen_bpp = hge->Ini_GetInt("GFX", "bpp", screen_bpp);
	windowed = (hge->Ini_GetInt("SYSTEM", "windowed", (int)windowed) == 0) ? false : true;
	show_fps = (hge->Ini_GetInt("SYSTEM", "showfps", (int)show_fps) == 0) ? false : true;
	vsync = (hge->Ini_GetInt("GFX", "vsync", (int)vsync) == 0) ? false : true;

//	if( windowed )
//		vsync = false;

	globalScale = screen_height / 600.0f;

	keymap_read_config();
}
//-----------------------------------------------------------------------------

void writeConfig()
{
	if( !hge )
		return;

	hge->Ini_SetInt("GFX", "width", screen_width);
	hge->Ini_SetInt("GFX", "height", screen_height);
	hge->Ini_SetInt("GFX", "bpp", screen_bpp);
	hge->Ini_SetInt("SYSTEM", "windowed", windowed ? 1 : 0);
	hge->Ini_SetInt("SYSTEM", "showfps", show_fps ? 1 : 0);
	hge->Ini_SetInt("GFX", "vsync", vsync ? 1 : 0);

	keymap_write_config();
}
//-----------------------------------------------------------------------------

int initHGE()
{
	if( hge )
		closeHGE();

	hge = hgeCreate(HGE_VERSION);
	if( !hge )
		return -1;

	hge->System_SetState(HGE_LOGFILE, "log.txt");

	hge->System_SetState(HGE_INIFILE, "fury.cfg");
	readConfig();

	hge->System_SetState(HGE_FRAMEFUNC, onFrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, onRenderFunc);
	hge->System_SetState(HGE_USESOUND, false);
	hge->System_SetState(HGE_SHOWSPLASH, false);
	hge->System_SetState(HGE_TITLE, "The Fury");

	// set up video mode
	hge->System_SetState(HGE_WINDOWED, windowed);
	hge->System_SetState(HGE_SCREENWIDTH, screen_width);
	hge->System_SetState(HGE_SCREENHEIGHT, screen_height);
	hge->System_SetState(HGE_SCREENBPP, screen_bpp);

	int32 fps = vsync ? HGEFPS_VSYNC : HGEFPS_UNLIMITED;

	hge->System_SetState(HGE_FPS, fps);

	if( !hge->System_Initiate() )
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return -2;
	}

	default_font = new hgeFont( (Path::fonts + "verdana_10.fnt").c_str() );

	return 0;
}
//-----------------------------------------------------------------------------

int initBox2D()
{
	b2AABB worldAABB;
	worldAABB.lowerBound.Set(phys_world_bounds.x1 / phys_norma, phys_world_bounds.y1 / phys_norma);
	worldAABB.upperBound.Set(phys_world_bounds.x2 / phys_norma, phys_world_bounds.y2 / phys_norma);
	b2Vec2 vgravity( phys_gravity.x / phys_norma, phys_gravity.y / phys_norma );
	box2d = new b2World(vgravity, true);

	if( !box2d )
	{
		log_msg(COLOR_CRITICAL, "Unable to create physics system");
		return -1;
	}

	box2d->SetContinuousPhysics(true);
	box2d->SetWarmStarting(true);

	collision_detector_init();

	return 0;
}
//-----------------------------------------------------------------------------

int initAudiere()
{
	return 0;
}
//-----------------------------------------------------------------------------

void closeHGE()
{
	// save configuration
	writeConfig();

	if( hge )
	{
		hge->System_Shutdown();
		hge->Release();
	}
	hge = 0;
}
//-----------------------------------------------------------------------------

void closeBox2D()
{
	if( box2d )
		delete box2d;

	box2d = 0;
}
//-----------------------------------------------------------------------------

void closeAudiere()
{

}
//-----------------------------------------------------------------------------

void closeStateManager()
{
	if( stateManager )
		delete stateManager;
	stateManager = 0;
}
//-----------------------------------------------------------------------------

int initStateManager()
{
	stateManager = new GameStateManager();
	if( !stateManager )
	{
		log_msg(COLOR_CRITICAL, "Unable to create state manager");
		return -5;
	}
	log_msg("State manager created");

	return 0;
}
//-----------------------------------------------------------------------------
