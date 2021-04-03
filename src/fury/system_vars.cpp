#include "types.h"
#include <hgevector.h>
#include <string>
#include "data_level.h"
#include "data_staticobject.h"
#include "data_tileset.h"

using namespace std;

//	screen mode
unsigned int	screenWidth		= 800;
unsigned int	screenHeight	= 600;
unsigned char	screenBPP		= 32;
bool			windowed		= true;
float			globalScale		= 1.0f;

string			appDirectory = "";

string			levelFile = "default.flf";

vector2f		globalPosition	= vector2f(0.0f, 0.0f);
vector2f		cameraPosition;

GameLevel*		level			= 0;
Tileset*		g_tileset		= 0;

float			physicsNorma	= 1.0f;

bool			game_pause		= false;