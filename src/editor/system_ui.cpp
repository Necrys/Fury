#include <hgefont.h>
#include "ui_label.h"
#include "ui_imagebutton.h"
#include "ui_dropdownlist.h"
#include "ui_imagelist.h"
#include "ui_editbox.h"
#include "system_ui.h"
#include "system_main.h"
#include "system_paths.h"
#include "system_interfaces.h"
#include "system_uistatemanager.h"
#include "data_level.h"
#include "grid.h"
#include "str_helpers.h"
#include "sys_vars.h"
#include "toolbar.h"
#include "save_dlg.h"
#include "preferences_dlg.h"
#include "toolbox.h"
#include "dialog_crunch.h"
#include "dialog_point.h"
#include "dialog_lift.h"
#include "dialog_door.h"
#include "dialog_trigger.h"
//-----------------------------------------------------------------------------

int initUI()
{
	stateManager = new UIStateManager();
	stateManager->setGlobalState(GS_LOADINTERFACE);

	pref_dlg_init();
	save_dlg_init();
	toolbar_init();
	toolbox_init();
	crunch_dlg_init();
	point_dlg_init();
	lift_dlg_init();
	door_dlg_init();
	trigger_dlg_init();

	stateManager->setGlobalState(GS_NOLEVEL);

	// init grid
	if( grid )
		delete grid;
	grid = 0;

	grid = new SnapGrid();
	lbl_grid_size->text = uint2str(grid->gridStep);

	return 0;
}
//-----------------------------------------------------------------------------
