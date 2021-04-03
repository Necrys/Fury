#include "types.h"
#include <string>
#include "data_level.h"
#include "system_uistatemanager.h"
#include "grid.h"
#include "data_staticobject.h"
#include "data_dynamicobject_base.h"
#include "data_collisionshape.h"
#include "data_tileset.h"

using namespace std;

//	screen mode
//unsigned int		screen_width		= 800;
//unsigned int		screen_height	= 600;
//unsigned char		screen_bpp		= 32;
//bool				windowed		= true;

//string				appDirectory = "";

//hgeVector			global_position	= hgeVector(0.0f, 0.0f);

//GameLevel*			level = 0;
UIStateManager*		stateManager = 0;
SnapGrid*			grid = 0;
GameStaticObject*	selectedStaticTemplate = 0;
GameStaticObject*	selectedLevelObject = 0;
GameStaticObject*	selectedBackgroundTemplate = 0;
GameStaticObject*	selectedBackgroundObject = 0;
GameStaticObject*	selectedForegroundTemplate = 0;
GameStaticObject*	selectedForegroundObject = 0;
GameDynamicObject*	selectedDynamicTemplate = 0;
GameDynamicObject*	selectedLevelDynamic = 0;
int					selectedDynamicPoint = 0;
CollisionShape*		selectedCollision = 0;
int					selectedCollisionCorner = 0;
//Tileset*			g_tileset = 0;

bool				renderGrid = true;
bool				renderBack = false;
bool				layerTransparency = true;

bool				selected_hflip = false;
bool				selected_vflip = false;
float				selected_rotation = 0.0f;