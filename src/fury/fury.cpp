#include "types.h"
#include <string>
#include <vector>
#include "system_interfaces.h"
#include "system_main.h"
#include "data_level.h"
#include "helper_functions.h"
#include "system_paths.h"
#include <locale.h>
//-----------------------------------------------------------------------------

using namespace std;
//-----------------------------------------------------------------------------

void app_exit( const uint32& code )
{
	exit(code);
}
//-----------------------------------------------------------------------------

int WINAPI WinMain( __in	HINSTANCE	hInstance, 
				   __in_opt HINSTANCE	hPrevInstance, 
				   __in_opt LPSTR		lpCmdLine, 
				   __in		int			nShowCmd )
{
	init_working_dir();

	setlocale(LC_ALL, "Russian");
	setlocale(LC_NUMERIC, "C");

	char buffer[MAX_PATH];
	getSystemPath(buffer, MAX_PATH);
	appDirectory = buffer;

	if( !initHGE() )
	{
		if( !initHelpers() )
		{
			if( !initStateManager() )
			{
				if( !stateManager->setState(GAME_STATE_LOADING) )
				{
					log_msg("System ready! Start!");
					hge->System_Start();
				}
			}
		}
	}

	closeHGE();

	// never get here
	return 0;
}
//-----------------------------------------------------------------------------