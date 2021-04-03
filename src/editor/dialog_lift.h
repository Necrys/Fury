#pragma once
//-----------------------------------------------------------------------------

namespace gui
{
	class window;
	class label;
	class edit_box;
	class image_list;
	class image_button;
	class checkbox;
};

class GameLift;
//-----------------------------------------------------------------------------

extern gui::window*			wnd_lift_dlg;
extern gui::label*			lbl_lift_id;
extern gui::edit_box*		edt_lift_id;
extern gui::image_list*		iml_all_points;
extern gui::image_list*		iml_lift_points;
extern gui::label*			lbl_lift_enabled;
extern gui::checkbox*		chb_lift_enabled;
extern gui::image_button*	btn_add_point;
extern gui::image_button*	btn_remove_point;
extern gui::image_button*	btn_lift_ok;
extern gui::image_button*	btn_lift_cancel;
//-----------------------------------------------------------------------------

void lift_dlg_init();
void lift_dlg_show( GameLift* lift );
void lift_dlg_hide();
//-----------------------------------------------------------------------------