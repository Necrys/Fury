
#include "debug_log.h"
#include "helper_texturemanager.h"
#include "helper_xmlcache.h"
#include "ui_main.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "helper_functions.h"

using namespace std;

//-----------------------------------------------------------------------------

TextureManager*		textureManager = 0;
XMLCache*			xmlCache = 0;
gui::system*		gui_system = 0;

hgeQuad				fullscreenQuad;

//-----------------------------------------------------------------------------

int initHelpers()
{
	// init texture manager
	if( textureManager )
		delete textureManager;
	textureManager = 0;

	textureManager = new TextureManager();
	if( !textureManager )
	{
		log_msg(COLOR_CRITICAL, "Unable to initialize texture manager");
		return -2;
	}

	log_msg("Texture manager initialized");

	// init xml cache
	if( xmlCache )
		delete xmlCache;
	xmlCache = 0;

	xmlCache = new XMLCache();
	if( !xmlCache )
	{
		log_msg(COLOR_CRITICAL, "Unable to initialize XML cache");
		return -3;
	}

	log_msg("XML cache initialized");

	// init ui controller
	if( gui_system )
		delete gui_system;
	gui_system = 0;
	
	gui_system = new gui::system();
	if( !gui_system )
	{
		log_msg(COLOR_CRITICAL, "Unable to initialize UI controller");
		return -4;
	}

//	uiController->setFont( (Path::fonts + "verdana_10.fnt").c_str() );
//	UICursor* cursor = new UICursor( (Path::uigfx + "cursor.png").c_str(), 0, 0, 32, 32, 1, 1 );
//	uiController->setCursor( cursor );
	gui_system->setSkin( Path::uiskins + "dark_gray.xml" );
	log_msg("UI controller initialized");

	initFullscreenQuad();

	return 0;
}
//-----------------------------------------------------------------------------
