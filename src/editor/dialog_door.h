#pragma once
//-----------------------------------------------------------------------------

namespace gui
{
	class window;
	class drop_list;
	class label;
	class image_button;
}

class GameDoor;
//-----------------------------------------------------------------------------

extern gui::window*			wnd_door_dlg;
extern gui::drop_list*		ddl_door_direction;
extern gui::image_button*	btn_door_ok;
extern gui::image_button*	btn_door_cancel;
//-----------------------------------------------------------------------------

void door_dlg_init();
void door_dlg_show( GameDoor* door );
void door_dlg_hide();
//-----------------------------------------------------------------------------