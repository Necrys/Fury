#pragma once


#include "debug_log.h"
#include "helper_texturemanager.h"
#include "helper_xmlcache.h"
#include "ui_main.h"

class b2World;

// main interfaces
extern	HGE*					hge;
extern	hgeFont*				default_font;
extern	b2World*				box2d;
//extern	audiere::AudioDevicePtr	audio;

// helper interfaces
extern TextureManager*	textureManager;
extern XMLCache*		xmlCache;
extern gui::system*		gui_system;

int initHGE();
int initBox2D();
int initAudiere();

void closeHGE();
void closeBox2D();
void closeAudiere();

void writeConfig();
void readConfig();

int initHelpers();