#include "dialog_door.h"

#include "system_interfaces.h"
#include "system_paths.h"
#include "data_dynamic_door.h"

#include "ui_window.h"
#include "ui_dropdownlist.h"
#include "ui_imagebutton.h"
#include "ui_label.h"
#include "ui_editbox.h"

#include "hgefont.h"
//-----------------------------------------------------------------------------

gui::window*		wnd_door_dlg = 0;
gui::label*			lbl_door_direction = 0;
gui::drop_list*		ddl_door_direction = 0;
gui::image_button*	btn_door_ok = 0;
gui::image_button*	btn_door_cancel = 0;
gui::label*			lbl_door_id = 0;
gui::edit_box*		edb_door_id = 0;

GameDoor* door_obj = 0;
//-----------------------------------------------------------------------------

void on_door_ok( gui::button* );
void on_door_cancel( gui::button* );
//-----------------------------------------------------------------------------

void door_dlg_init()
{
	wnd_door_dlg = new gui::window( gui_system );
	wnd_door_dlg->bounds = hgeRect( 200.0f, 200.0f, 550.0f, 400.0f );
	wnd_door_dlg->caption = "Door properties";
	wnd_door_dlg->set_visibility(false);

	btn_door_ok = new gui::image_button(gui_system, wnd_door_dlg);
	btn_door_ok->bounds = hgeRect( 141.0f, 140.0f, 191.0f, 170.0f );
	btn_door_ok->setImage( Path::uigfx + "icons\\ok.png" );
	btn_door_ok->caption = "OK";
	btn_door_ok->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_door_ok->on_press = on_door_ok;

	btn_door_cancel = new gui::image_button(gui_system, wnd_door_dlg);
	btn_door_cancel->bounds = hgeRect( 196.0f, 140.0f, 246.0f, 170.0f );
	btn_door_cancel->setImage( Path::uigfx + "icons\\cancel.png" );
	btn_door_cancel->caption = "Cancel";
	btn_door_cancel->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_door_cancel->on_press = on_door_cancel;

	lbl_door_id = new gui::label(gui_system, wnd_door_dlg);
	lbl_door_id->set_position_size( 5.0f, 15.0f, 100.0f, 20.0f );
	lbl_door_id->text = "ID";

	edb_door_id = new gui::edit_box(gui_system, wnd_door_dlg);
	edb_door_id->set_position_size( 10.0f, 45.0f, 280.0f, 20.0f );

	lbl_door_direction = new gui::label(gui_system, wnd_door_dlg);
	lbl_door_direction->set_position_size( 5.0f, 70.0f, 100.0f, 20.0f );
	lbl_door_direction->text = "Direction";

	ddl_door_direction = new gui::drop_list(gui_system, wnd_door_dlg);
	ddl_door_direction->bounds = hgeRect( 10.0f, 95.0f, 280.0f, 115.0f );

	ddl_door_direction->insertElement("up");
	ddl_door_direction->insertElement("down");
	ddl_door_direction->insertElement("left");
	ddl_door_direction->insertElement("right");
}
//-----------------------------------------------------------------------------

void door_dlg_show( GameDoor* door )
{
	ASSERT(door);
	door_obj = door;

	ddl_door_direction->setSelection( door_obj->direction );
	edb_door_id->text = door_obj->id;
	wnd_door_dlg->set_visibility(true);
	gui_system->show_modal_window(wnd_door_dlg);
}
//-----------------------------------------------------------------------------

void door_dlg_hide()
{
	door_obj = 0;
	wnd_door_dlg->set_visibility(false);
}
//-----------------------------------------------------------------------------

void on_door_ok( gui::button* )
{
	ASSERT(door_obj);
	door_obj->direction = (DOOR_OPENING_DIR)ddl_door_direction->getSelectionID();
	door_obj->id = edb_door_id->text;
	door_dlg_hide();
}
//-----------------------------------------------------------------------------

void on_door_cancel( gui::button* )
{
	door_dlg_hide();
}
//-----------------------------------------------------------------------------