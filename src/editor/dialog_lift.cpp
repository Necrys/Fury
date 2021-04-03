#include "dialog_lift.h"

#include "system_paths.h"

#include "ui_window.h"
#include "ui_imagebutton.h"
#include "ui_label.h"
#include "ui_editbox.h"
#include "ui_checkbox.h"
#include "ui_imagelist.h"

#include "data_dynamic_lift.h"
#include "data_dynamic_point.h"

#include "hgefont.h"
//-----------------------------------------------------------------------------

gui::window*		wnd_lift_dlg = 0;
gui::label*			lbl_lift_id = 0;
gui::edit_box*		edt_lift_id = 0;
gui::image_list*	iml_all_points = 0;
gui::image_list*	iml_lift_points = 0;
gui::label*			lbl_lift_enabled = 0;
gui::checkbox*		chb_lift_enabled = 0;
gui::image_button*	btn_add_point = 0;
gui::image_button*	btn_remove_point = 0;
gui::image_button*	btn_lift_ok = 0;
gui::image_button*	btn_lift_cancel = 0;

GameLift* lift_obj = 0;
//-----------------------------------------------------------------------------

void on_btn_add_point( gui::button* );
void on_btn_remove_point( gui::button* );
void on_btn_lift_ok( gui::button* );
void on_btn_lift_cancel( gui::button* );
//-----------------------------------------------------------------------------

void lift_dlg_init()
{
	wnd_lift_dlg = new gui::window( gui_system );
	wnd_lift_dlg->bounds = hgeRect( 150.0f, 100.0f, 550.0f, 550.0f );
	wnd_lift_dlg->caption = "Lift properties";
	wnd_lift_dlg->set_visibility(false);

	lbl_lift_id = new gui::label(gui_system, wnd_lift_dlg);
	lbl_lift_id->text = "ID";
	lbl_lift_id->bounds = hgeRect(20.0f, 20.0f, 60.0f, 40.0f);
	lbl_lift_id->align = HGETEXT_RIGHT;

	edt_lift_id = new gui::edit_box(gui_system, wnd_lift_dlg);
	edt_lift_id->bounds = hgeRect(80.0f, 20.0f, 280.0f, 40.0f);

	iml_all_points = new gui::image_list(gui_system, wnd_lift_dlg);
	iml_all_points->bounds = hgeRect(20.0f, 60.0f, 180.0f, 380.0f);

	iml_lift_points = new gui::image_list(gui_system, wnd_lift_dlg);
	iml_lift_points->bounds = hgeRect(220.0f, 60.0f, 380.0f, 380.0f);

	btn_lift_ok = new gui::image_button(gui_system, wnd_lift_dlg);
	btn_lift_ok->bounds = hgeRect( 141.0f, 400.0f, 191.0f, 430.0f );
	btn_lift_ok->setImage( Path::uigfx + "icons\\ok.png" );
	btn_lift_ok->caption = "OK";
	btn_lift_ok->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_lift_ok->on_press = on_btn_lift_ok;

	btn_lift_cancel = new gui::image_button(gui_system, wnd_lift_dlg);
	btn_lift_cancel->bounds = hgeRect( 196.0f, 400.0f, 246.0f, 430.0f );
	btn_lift_cancel->setImage( Path::uigfx + "icons\\cancel.png" );
	btn_lift_cancel->caption = "Cancel";
	btn_lift_cancel->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_lift_cancel->on_press = on_btn_lift_cancel;

	btn_add_point = new gui::image_button(gui_system, wnd_lift_dlg);
	btn_add_point->bounds = hgeRect( 185.0f, 100.0f, 215.0f, 130.0f );
	btn_add_point->setImage( Path::uigfx + "icons\\right.png" );
	btn_add_point->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_add_point->on_press = on_btn_add_point;

	btn_remove_point = new gui::image_button(gui_system, wnd_lift_dlg);
	btn_remove_point->bounds = hgeRect( 185.0f, 140.0f, 215.0f, 170.0f );
	btn_remove_point->setImage( Path::uigfx + "icons\\left.png" );
	btn_remove_point->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_remove_point->on_press = on_btn_lift_cancel;

	lbl_lift_enabled = new gui::label(gui_system, wnd_lift_dlg);
	lbl_lift_enabled->set_position_size(20, 385, 45, 20);
	lbl_lift_enabled->text = "Enabled";

	chb_lift_enabled = new gui::checkbox(gui_system, wnd_lift_dlg);
	chb_lift_enabled->set_position_size( 75, 390, 12, 12 );
}
//-----------------------------------------------------------------------------

void lift_dlg_show( GameLift* lift )
{
	ASSERT(lift);

	lift_obj = lift;
	wnd_lift_dlg->set_visibility(true);

	edt_lift_id->text = lift_obj->id;
	chb_lift_enabled->checked = lift_obj->enabled;

	iml_lift_points->clear();
	iml_all_points->clear();

	std::map<std::string, GamePoint*>::iterator pit = level->points.begin();
	while( pit != level->points.end() )
	{
		std::list<GamePoint*>::iterator lpit = lift_obj->points.begin();
		bool found = false;
		while( lpit != lift_obj->points.end() )
		{
			if( (*lpit)->id == pit->second->id )
			{
				gui::image_list_item* element = iml_lift_points->insertElement( "", pit->second->id );
				element->user_data = (void*)pit->second;
				found = true;
				break;
			}

			lpit++;
		}

		if( !found )
		{
			gui::image_list_item* element = iml_all_points->insertElement( "", pit->second->id );
			element->user_data = (void*)pit->second;
		}

		pit++;
	}

	gui_system->show_modal_window( wnd_lift_dlg );
}
//-----------------------------------------------------------------------------

void lift_dlg_hide()
{
	lift_obj = 0;
	wnd_lift_dlg->set_visibility(false);
}
//-----------------------------------------------------------------------------

void on_btn_add_point( gui::button* )
{
	gui::image_list_item* element = iml_all_points->getSelection();
	if( !element )
		return;

	GamePoint* point = (GamePoint*)element->user_data;
	ASSERT(point);

	gui::image_list_item* new_element = iml_lift_points->insertElement( "", element->text );
	new_element->user_data = (void*)point;

	iml_all_points->removeElement(element->id());
}
//-----------------------------------------------------------------------------

void on_btn_remove_point( gui::button* )
{
	gui::image_list_item* element = iml_lift_points->getSelection();
	if( !element )
		return;

	GamePoint* point = (GamePoint*)element->user_data;
	ASSERT(point);	

	gui::image_list_item* new_element = iml_all_points->insertElement( "", element->text );
	new_element->user_data = (void*)point;

	iml_lift_points->removeElement(element->id());
}
//-----------------------------------------------------------------------------

void on_btn_lift_ok( gui::button* )
{
	ASSERT(lift_obj);

	lift_obj->id = edt_lift_id->text;
	lift_obj->enabled = chb_lift_enabled->checked;

	lift_obj->points.clear();
	for( uint32 i=0; i<iml_lift_points->size(); i++ )
	{
		GamePoint* point = (GamePoint*)((*iml_lift_points)[i])->user_data;
		ASSERT(point);
		lift_obj->points.push_back(point);
	}

	lift_dlg_hide();
}
//-----------------------------------------------------------------------------

void on_btn_lift_cancel( gui::button* )
{
	lift_dlg_hide();
}
//-----------------------------------------------------------------------------