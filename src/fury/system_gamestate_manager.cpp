/*=============================================================================

	The Fury

DESCRIPTION:
	game state manager

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
21/05/2010	initial version
=============================================================================*/

#include "types.h"
#include "system_gamestate_manager.h"
#include "system_main.h"
#include "system_paths.h"
#include "helper_functions.h"

//-----------------------------------------------------------------------------

GameStateManager::GameStateManager()
{
	state = GAME_STATE_UNKNOWN;
}
//-----------------------------------------------------------------------------

GameStateManager::~GameStateManager()
{
}
//-----------------------------------------------------------------------------

int GameStateManager::setState(const GAME_STATE &new_state)
{
	switch( new_state )
	{
	case GAME_STATE_LOADING:
		{
			if( !level )
			{
				OPENFILENAME	ofn;

				char fileName[MAX_PATH];
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.lpstrFile = fileName;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(fileName);
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = Path::levels.c_str();
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				ofn.lpstrFilter = "Fury level files\0*.flf;\0";
				ofn.nFilterIndex = 1;

				if( GetOpenFileName(&ofn) )
				{
					if( level )
						delete level;
					level = new GameLevel();

					if( level->load(ofn.lpstrFile) )
						return -2;
					else
						return 0;
				}
				else
				{
					return -1;
				}
			}
		}
		break;
	case GAME_STATE_LEVEL:
		{
			fadeIn(1.0f);
		}
		break;
	default:
		break;
	}

	state = new_state;
	return 0;
}
//-----------------------------------------------------------------------------

GAME_STATE GameStateManager::getState() const
{
	return state;
}
//-----------------------------------------------------------------------------