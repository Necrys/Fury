#include "system_interfaces.h"
#include "system_main.h"
#include "str_helpers.h"
#include "system_paths.h"
#include "sys_vars.h"
#include "hgefont.h"

//using namespace audiere;

//-----------------------------------------------------------------------------
HGE*			hge = 0;
hgeFont*		default_font = 0;
b2World*		box2d = 0;
//AudioDevicePtr	audio = 0;

//-----------------------------------------------------------------------------

void readConfig()
{
	if( !hge )
		return;

	screen_width = hge->Ini_GetInt("GFX", "width", screen_width);
	screen_height = hge->Ini_GetInt("GFX", "height", screen_height);
	screen_bpp = hge->Ini_GetInt("GFX", "bpp", screen_bpp);
	windowed = (hge->Ini_GetInt("SYSTEM", "windowed", (int)windowed) == 0) ? false : true;
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

	hge->System_SetState(HGE_INIFILE, "editor.cfg");
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
	hge->System_SetState(HGE_FPS, FPS);

	if( !hge->System_Initiate() )
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return -2;
	}

	default_font = new hgeFont( (Path::fonts + "verdana_10.fnt").c_str() );
	if( !default_font )
		return -3;

	return 0;
}

//-----------------------------------------------------------------------------
int initBox2D()
{
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

}

//-----------------------------------------------------------------------------
void closeAudiere()
{

}