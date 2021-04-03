/*=============================================================================

	The Fury

DESCRIPTION:
	main executable source,

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
04/05/2010	initial version
=============================================================================*/

#pragma once

#include "types.h"
#include <string>
#include "system_uistatemanager.h"
#include "data_tileset.h"

class GameLevel;
class UIStateManager;
class SnapGrid;
class GameStaticObject;
class GameDynamicObject;
class CollisionShape;

// frame callback function
bool onFrameFunc();

// render callback function
bool onRenderFunc();

extern std::string		appDirectory;

extern GameLevel*			level;
extern UIStateManager*		stateManager;
extern SnapGrid*			grid;
extern GameStaticObject*	selectedStaticTemplate;
extern GameStaticObject*	selectedLevelObject;
extern GameStaticObject*	selectedBackgroundTemplate;
extern GameStaticObject*	selectedBackgroundObject;
extern GameStaticObject*	selectedForegroundTemplate;
extern GameStaticObject*	selectedForegroundObject;
extern GameDynamicObject*	selectedDynamicTemplate;
extern GameDynamicObject*	selectedLevelDynamic;
extern int					selectedDynamicPoint;
extern CollisionShape*		selectedCollision;
extern int					selectedCollisionCorner;
extern Tileset*				g_tileset;

extern bool					renderGrid;
extern bool					renderBack;
extern bool					layerTransparency;

extern hgeQuad			fullscreenQuad;

extern bool					selected_hflip;
extern bool					selected_vflip;
extern float				selected_rotation;