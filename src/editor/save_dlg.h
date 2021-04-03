#pragma once
//-----------------------------------------------------------------------------

#include "common.h"
//-----------------------------------------------------------------------------

namespace gui
{
	class window;
	class button;
	class label;
}
//-----------------------------------------------------------------------------

extern gui::window*	wnd_save_dlg;
extern gui::button*	btn_save;
extern gui::button*	btn_no_save;
extern gui::button*	btn_cancel_save;
extern gui::label*	lbl_save_text;
extern uint32		save_dlg_next_state;
//-----------------------------------------------------------------------------

void save_dlg_init();
void save_dlg_show();
void save_dlg_hide( uint32 result );
//-----------------------------------------------------------------------------