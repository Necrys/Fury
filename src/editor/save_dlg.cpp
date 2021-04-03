#include "save_dlg.h"

#include "ui_window.h"
#include "ui_button.h"
#include "ui_label.h"

#include "system_interfaces.h"
#include "system_main.h"
#include "sys_vars.h"
#include "data_level.h"

#include "hgefont.h"
//-----------------------------------------------------------------------------

gui::window*	wnd_save_dlg;
gui::button*	btn_save;
gui::button*	btn_no_save;
gui::button*	btn_cancel_save;
gui::label*		lbl_save_text;
uint32			save_dlg_next_state;
//-----------------------------------------------------------------------------

void save_press( gui::button* btn );
void no_save_press( gui::button* btn );
void cancel_save_press( gui::button* btn );
//-----------------------------------------------------------------------------

void save_dlg_init()
{
	wnd_save_dlg = new gui::window( gui_system );
	wnd_save_dlg->set_visibility(false);
	wnd_save_dlg->bounds = hgeRect( screen_width / 2.0f - 200.0f, screen_height / 2.0f - 40.0f, screen_width / 2.0f + 200.0f, screen_height / 2.0f + 40.0f );
	wnd_save_dlg->caption = "Сохранить";

	lbl_save_text = new gui::label( gui_system, wnd_save_dlg );
	lbl_save_text->text = "Текущий уровень не сохранён, хотите сделать это сейчас?";
	lbl_save_text->bounds = hgeRect(30.0f, 10.0f, 370.0f, 22.0f);
	lbl_save_text->align = HGETEXT_TOP | HGETEXT_CENTER;

	btn_save = new gui::button( gui_system, wnd_save_dlg );
	btn_save->caption = "Сохранить";
	btn_save->bounds = hgeRect( 30.0f, 35.0f, 130.0f, 55.0f );
	btn_save->on_press = save_press;

	btn_no_save = new gui::button( gui_system, wnd_save_dlg );
	btn_no_save->caption = "Не сохранять";
	btn_no_save->bounds = hgeRect( 150.0f, 35.0f, 250.0f, 55.0f );
	btn_no_save->on_press = no_save_press;

	btn_cancel_save = new gui::button( gui_system, wnd_save_dlg );
	btn_cancel_save->caption = "Отмена";
	btn_cancel_save->bounds = hgeRect( 270.0f, 35.0f, 370.0f, 55.0f );
	btn_cancel_save->on_press = cancel_save_press;
}
//-----------------------------------------------------------------------------

void save_dlg_show()
{
	wnd_save_dlg->set_visibility(true);
	gui_system->show_modal_window( wnd_save_dlg );
}
//-----------------------------------------------------------------------------

void save_dlg_hide( uint32 result )
{
	wnd_save_dlg->set_visibility(false);

	switch( result )
	{
	case 1:
	case 2:
		if( level )
			delete level;
		level = 0;
		stateManager->setGlobalState( (GLOBALSTATE)save_dlg_next_state );
		break;
	case 3:
	default:
		break;
	}
}
//-----------------------------------------------------------------------------

void save_press( gui::button* btn )
{
	level->save();
	log_msg("Уровень сохранён");
	save_dlg_hide(1);
}
//-----------------------------------------------------------------------------

void no_save_press( gui::button* btn )
{
	log_msg("Уровень не был сохранён", COLOR_WARNING);
	save_dlg_hide(2);
}
//-----------------------------------------------------------------------------

void cancel_save_press( gui::button* btn )
{
	save_dlg_hide(3);
}
//-----------------------------------------------------------------------------