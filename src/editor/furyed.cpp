#include "system_interfaces.h"
#include "system_main.h"
#include "system_ui.h"
#include "data_level.h"
#include "helper_functions.h"
#include <locale.h>
//-----------------------------------------------------------------------------

int WINAPI WinMain( __in HINSTANCE hInstance, 
				   __in_opt HINSTANCE hPrevInstance, 
				   __in_opt LPSTR lpCmdLine, 
				   __in int nShowCmd )
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
			if( !initUI() )
			{
				log_msg("System ready! Start!");
				hge->System_Start();
			}
		}
	}

	closeHGE();

	return 0;
}
//-----------------------------------------------------------------------------