#pragma once
//-----------------------------------------------------------------------------

namespace gui
{
	class window;
	class image_button;
	class label;
	class edit_box;
};

class GamePoint;
//-----------------------------------------------------------------------------

extern gui::window*			wnd_point_dlg;
extern gui::label*			lbl_point_id;
extern gui::edit_box*		edb_point_id;
extern gui::image_button*	btn_point_ok;
extern gui::image_button*	btn_point_cancel;
//-----------------------------------------------------------------------------

void point_dlg_init();
void point_dlg_show( GamePoint* crunch, bool first_add = false );
void point_dlg_hide();
//-----------------------------------------------------------------------------