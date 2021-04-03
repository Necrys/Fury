#pragma once

#include "types.h"
#include <hgerect.h>
#include "debug_log.h"
#include "helper_texturemanager.h"
#include "helper_xmlcache.h"
#include "system_collider.h"
#include "system_gamestate_manager.h"
#include "ui_main.h"

//-----------------------------------------------------------------------------

class b2World;
//-----------------------------------------------------------------------------

// main interfaces
extern HGE*		hge;
extern hgeFont*	default_font;
//-----------------------------------------------------------------------------

extern b2World*					box2d;
//extern audiere::AudioDevicePtr	audio;

// helper interfaces
extern TextureManager*		textureManager;
extern XMLCache*			xmlCache;
extern gui::system*			gui_system;
extern GameStateManager*	stateManager;

int initHGE();
int initBox2D();
int initAudiere();
int initStateManager();

void closeHGE();
void closeBox2D();
void closeAudiere();
void closeStateManager();

void writeConfig();
void readConfig();

int initHelpers();