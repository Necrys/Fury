#pragma once
//-----------------------------------------------------------------------------

namespace gui
{
	class panel;
	class image_button;
	class label;
}
//-----------------------------------------------------------------------------

extern gui::panel*			pnl_toolbar;
extern gui::image_button*	btn_new_file;
extern gui::image_button*	btn_open_file;
extern gui::image_button*	btn_save_file;
extern gui::image_button*	btn_preferences;
extern gui::image_button*	btn_grid_increase;
extern gui::image_button*	btn_grid_decrease;
extern gui::label*			lbl_grid_size;
extern gui::label*			lbl_grid_size_cap;

void toolbar_init();
//-----------------------------------------------------------------------------