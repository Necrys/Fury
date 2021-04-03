#pragma once
//-----------------------------------------------------------------------------

namespace gui
{
	class label;
	class edit_box;
	class drop_list;
	class image_button;
	class window;
}
//-----------------------------------------------------------------------------

extern gui::window*			wnd_pref_dlg;
extern gui::label*			lbl_level_name;
extern gui::edit_box*		edb_level_name;
extern gui::label*			lbl_level_music;
extern gui::drop_list*		ddl_level_music;
extern gui::label*			lbl_level_tileset;
extern gui::drop_list*		ddl_level_tileset;
extern gui::image_button*	btn_pref_ok;
extern gui::image_button*	btn_pref_cancel;
//-----------------------------------------------------------------------------

void pref_dlg_init();
void pref_dlg_show( bool new_level );
void pref_dlg_hide();
//-----------------------------------------------------------------------------