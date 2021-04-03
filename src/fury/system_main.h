#pragma once

#include <string>
#include "data_tileset.h"
#include "data_level.h"
#include "system_gamestate_manager.h"

//-----------------------------------------------------------------------------

bool onFrameFunc();
//-----------------------------------------------------------------------------

bool onRenderFunc();
//-----------------------------------------------------------------------------

extern float			globalScale;
extern vector2f		global_position;
extern vector2f		camera_position;
extern std::string		appDirectory;
extern std::string		levelFile;

extern GameLevel*		level;
extern Tileset*			g_tileset;

extern hgeQuad			fullscreenQuad;
//-----------------------------------------------------------------------------
