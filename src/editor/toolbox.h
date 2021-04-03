#pragma once
//-----------------------------------------------------------------------------

namespace gui
{
	class panel;
	class drop_list;
	class image_list;
}
//-----------------------------------------------------------------------------

extern gui::panel*		pnl_toolbox;
extern gui::drop_list*	ddl_edit_modes;
extern gui::drop_list*	ddl_tile_groups;
extern gui::image_list*	iml_tiles;
//-----------------------------------------------------------------------------

void toolbox_init();
void toolbox_show();
void toolbox_hide();
//-----------------------------------------------------------------------------