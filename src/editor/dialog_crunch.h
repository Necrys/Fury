#pragma once
//-----------------------------------------------------------------------------

namespace gui
{
	class window;
	class image_button;
	class label;
	class edit_box;
};

class GameCrunch;
//-----------------------------------------------------------------------------

extern gui::window*			wnd_crunch_dlg;
extern gui::label*			lbl_crunch_durability;
extern gui::edit_box*		edb_crunch_durability;
extern gui::image_button*	btn_crunch_ok;
extern gui::image_button*	btn_crunch_cancel;
//-----------------------------------------------------------------------------

void crunch_dlg_init();
void crunch_dlg_show( GameCrunch* crunch );
void crunch_dlg_hide();
//-----------------------------------------------------------------------------