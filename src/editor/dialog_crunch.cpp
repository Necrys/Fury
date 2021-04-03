#include "dialog_crunch.h"

#include "common.h"
#include "system_interfaces.h"
#include "system_main.h"
#include "system_paths.h"
#include "data_dynamic_crunch.h"

#include "ui_window.h"
#include "ui_imagebutton.h"
#include "ui_label.h"
#include "ui_editbox.h"

#include "hgefont.h"
//-----------------------------------------------------------------------------

gui::window*		wnd_crunch_dlg = 0;
gui::label*			lbl_crunch_durability = 0;
gui::edit_box*		edb_crunch_durability = 0;
gui::image_button*	btn_crunch_ok = 0;
gui::image_button*	btn_crunch_cancel = 0;
//-----------------------------------------------------------------------------

GameCrunch*			crunch_obj = 0;
//-----------------------------------------------------------------------------

void on_btn_crunch_ok( gui::button* );
void on_btn_crunch_cancel( gui::button* );
//-----------------------------------------------------------------------------

void crunch_dlg_init()
{
	wnd_crunch_dlg = new gui::window( gui_system );
	wnd_crunch_dlg->bounds = hgeRect( 200.0f, 200.0f, 550.0f, 400.0f );
	wnd_crunch_dlg->caption = "Crunch properties";
	wnd_crunch_dlg->set_visibility(false);

	btn_crunch_ok = new gui::image_button( gui_system, wnd_crunch_dlg );
	btn_crunch_ok->bounds = hgeRect( 141.0f, 150.0f, 191.0f, 180.0f );
	btn_crunch_ok->setImage( (Path::uigfx + "icons\\ok.png").c_str() );
	btn_crunch_ok->caption = "OK";
	btn_crunch_ok->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_crunch_ok->on_press = on_btn_crunch_ok;

	btn_crunch_cancel = new gui::image_button( gui_system, wnd_crunch_dlg );
	btn_crunch_cancel->bounds = hgeRect( 196.0f, 150.0f, 246.0f, 180.0f );
	btn_crunch_cancel->setImage( (Path::uigfx + "icons\\cancel.png").c_str() );
	btn_crunch_cancel->caption = "Cancel";
	btn_crunch_cancel->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_crunch_cancel->on_press = on_btn_crunch_cancel;

	lbl_crunch_durability = new gui::label( gui_system, wnd_crunch_dlg );
	lbl_crunch_durability->bounds = hgeRect(10.0f, 10.0f, 80.0f, 22.0f);
	lbl_crunch_durability->text = "Durability";
	lbl_crunch_durability->align = HGETEXT_TOP | HGETEXT_RIGHT;

	edb_crunch_durability = new gui::edit_box( gui_system, wnd_crunch_dlg );
	edb_crunch_durability->bounds = hgeRect(90.0f, 10.0f, 340.0f, 22.0f);
	edb_crunch_durability->text = "3";

	wnd_crunch_dlg->set_visibility(false);
}
//-----------------------------------------------------------------------------

void crunch_dlg_show( GameCrunch* crunch )
{
	ASSERT(crunch);

	crunch_obj = crunch;
	wnd_crunch_dlg->set_visibility(true);
	gui_system->show_modal_window(wnd_crunch_dlg);

	edb_crunch_durability->text = int2str(crunch_obj->health);
}
//-----------------------------------------------------------------------------

void crunch_dlg_hide()
{
	crunch_obj = 0;
	wnd_crunch_dlg->set_visibility(false);
	gui_system->set_focus(0);
}
//-----------------------------------------------------------------------------

void on_btn_crunch_ok( gui::button* )
{
	ASSERT(crunch_obj);

	crunch_obj->health = str2int(edb_crunch_durability->text);
	crunch_dlg_hide();
}
//-----------------------------------------------------------------------------

void on_btn_crunch_cancel( gui::button* )
{
	ASSERT(crunch_obj);

	crunch_dlg_hide();
}
//-----------------------------------------------------------------------------