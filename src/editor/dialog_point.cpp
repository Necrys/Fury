#include "dialog_point.h"

#include "common.h"
#include "system_interfaces.h"
#include "system_main.h"
#include "system_paths.h"
#include "data_dynamic_point.h"

#include "ui_window.h"
#include "ui_imagebutton.h"
#include "ui_label.h"
#include "ui_editbox.h"

#include "hgefont.h"
//-----------------------------------------------------------------------------

gui::window*		wnd_point_dlg = 0;
gui::label*			lbl_point_id = 0;
gui::edit_box*		edb_point_id = 0;
gui::image_button*	btn_point_ok = 0;
gui::image_button*	btn_point_cancel = 0;
//-----------------------------------------------------------------------------

GamePoint*			point_obj = 0;
//-----------------------------------------------------------------------------

void on_btn_point_ok( gui::button* );
void on_btn_point_cancel( gui::button* );
//-----------------------------------------------------------------------------

void point_dlg_init()
{
	wnd_point_dlg = new gui::window( gui_system );
	wnd_point_dlg->bounds = hgeRect( 200.0f, 200.0f, 550.0f, 400.0f );
	wnd_point_dlg->caption = "Point properties";
	wnd_point_dlg->set_visibility(false);

	btn_point_ok = new gui::image_button( gui_system, wnd_point_dlg );
	btn_point_ok->bounds = hgeRect( 141.0f, 150.0f, 191.0f, 180.0f );
	btn_point_ok->setImage( (Path::uigfx + "icons\\ok.png").c_str() );
	btn_point_ok->caption = "OK";
	btn_point_ok->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_point_ok->on_press = on_btn_point_ok;

	btn_point_cancel = new gui::image_button( gui_system, wnd_point_dlg );
	btn_point_cancel->bounds = hgeRect( 196.0f, 150.0f, 246.0f, 180.0f );
	btn_point_cancel->setImage( (Path::uigfx + "icons\\cancel.png").c_str() );
	btn_point_cancel->caption = "Cancel";
	btn_point_cancel->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_point_cancel->on_press = on_btn_point_cancel;

	lbl_point_id = new gui::label( gui_system, wnd_point_dlg );
	lbl_point_id->bounds = hgeRect(10.0f, 10.0f, 80.0f, 22.0f);
	lbl_point_id->text = "Point ID";
	lbl_point_id->align = HGETEXT_TOP | HGETEXT_RIGHT;

	edb_point_id = new gui::edit_box( gui_system, wnd_point_dlg );
	edb_point_id->bounds = hgeRect(90.0f, 10.0f, 340.0f, 22.0f);
	edb_point_id->text = "New point";

	wnd_point_dlg->set_visibility(false);
}
//-----------------------------------------------------------------------------

void point_dlg_show( GamePoint* point, bool first_add )
{
	ASSERT(point);

	point_obj = point;
	wnd_point_dlg->set_visibility(true);
	btn_point_cancel->enabled = !first_add;
	gui_system->show_modal_window(wnd_point_dlg);

	edb_point_id->text = point_obj->id;
}
//-----------------------------------------------------------------------------

void point_dlg_hide()
{
	point_obj = 0;
	wnd_point_dlg->set_visibility(false);
	gui_system->set_focus(0);
}
//-----------------------------------------------------------------------------

void on_btn_point_ok( gui::button* )
{
	ASSERT(point_obj);

	if( level->points.find(edb_point_id->text) != level->points.end() )
	{
		if( level->points.find(edb_point_id->text)->second != point_obj )
		{
			log_msg("Этот ID уже используется, нужно задать другой");
			return;
		}
	}
	else
	{
		level->dynamics.remove( point_obj ); // на случай если точка уже есть в списке
		level->dynamics.push_back( point_obj );
		
		if( btn_point_cancel->enabled )
		{
			if( level->points.find(point_obj->id) != level->points.end() )
				level->points.erase(point_obj->id);
		}

		level->points[edb_point_id->text] = point_obj;
	}

	point_obj->id = edb_point_id->text;
	point_dlg_hide();
}
//-----------------------------------------------------------------------------

void on_btn_point_cancel( gui::button* )
{
	ASSERT(point_obj);

	point_dlg_hide();
}
//-----------------------------------------------------------------------------