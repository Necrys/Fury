#include "dialog_trigger.h"

#include "system_main.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "data_level.h"
#include "data_dynamic_trigger.h"
#include "data_dynamicobject_base.h"

#include "ui_window.h"
#include "ui_imagebutton.h"
#include "ui_label.h"
#include "ui_editbox.h"
#include "ui_checkbox.h"
#include "ui_imagelist.h"

#include "hgefont.h"
//-----------------------------------------------------------------------------

gui::window*		wnd_trigger_dlg = 0;
gui::image_list*	iml_trigger_all_objs = 0;
gui::image_list*	iml_trigger_objs = 0;
gui::image_button*	btn_trigger_add_obj = 0;
gui::image_button*	btn_trigger_remove_obj = 0;
gui::label*			lbl_trigger_two_ways = 0;
gui::checkbox*		chb_trigger_two_ways = 0;
gui::label*			lbl_trigger_hidden = 0;
gui::checkbox*		chb_trigger_hidden = 0;
gui::image_button*	btn_trigger_ok = 0;
gui::image_button*	btn_trigger_cancel = 0;

GameTrigger* trigger_obj = 0;
//-----------------------------------------------------------------------------

void on_trigger_add_obj( gui::button* );
void on_trigger_remove_obj( gui::button* );
void on_trigger_ok( gui::button* );
void on_trigger_cancel( gui::button* );
//-----------------------------------------------------------------------------

void trigger_dlg_init()
{
	wnd_trigger_dlg = new gui::window(gui_system);
	wnd_trigger_dlg->bounds = hgeRect(150.0f, 100.0f, 550.0f, 550.0f );
	wnd_trigger_dlg->caption = "Trigger properties";
	wnd_trigger_dlg->set_visibility(false);

	iml_trigger_all_objs = new gui::image_list(gui_system, wnd_trigger_dlg);
	iml_trigger_all_objs->set_position_size(20, 20, 160, 320);

	iml_trigger_objs = new gui::image_list(gui_system, wnd_trigger_dlg);
	iml_trigger_objs->set_position_size(220, 20, 160, 320);

	btn_trigger_ok = new gui::image_button(gui_system, wnd_trigger_dlg);
	btn_trigger_ok->bounds = hgeRect( 141, 400, 191, 430 );
	btn_trigger_ok->setImage( Path::uigfx + "icons\\ok.png" );
	btn_trigger_ok->caption = "OK";
	btn_trigger_ok->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_trigger_ok->on_press = on_trigger_ok;

	btn_trigger_cancel = new gui::image_button(gui_system, wnd_trigger_dlg);
	btn_trigger_cancel->bounds = hgeRect( 196.0f, 400.0f, 246.0f, 430.0f );
	btn_trigger_cancel->setImage( Path::uigfx + "icons\\cancel.png" );
	btn_trigger_cancel->caption = "Cancel";
	btn_trigger_cancel->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_trigger_cancel->on_press = on_trigger_cancel;

	btn_trigger_add_obj = new gui::image_button(gui_system, wnd_trigger_dlg);
	btn_trigger_add_obj->bounds = hgeRect( 185.0f, 60.0f, 215.0f, 90.0f );
	btn_trigger_add_obj->setImage( Path::uigfx + "icons\\right.png" );
	btn_trigger_add_obj->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_trigger_add_obj->on_press = on_trigger_add_obj;

	btn_trigger_remove_obj = new gui::image_button(gui_system, wnd_trigger_dlg);
	btn_trigger_remove_obj->bounds = hgeRect( 185.0f, 100.0f, 215.0f, 130.0f );
	btn_trigger_remove_obj->setImage( Path::uigfx + "icons\\left.png" );
	btn_trigger_remove_obj->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_trigger_remove_obj->on_press = on_trigger_remove_obj;

	lbl_trigger_two_ways	= new gui::label(gui_system, wnd_trigger_dlg);
	lbl_trigger_two_ways->set_position_size(20, 345, 60, 20);
	lbl_trigger_two_ways->text = "Two ways";

	chb_trigger_two_ways = new gui::checkbox(gui_system, wnd_trigger_dlg);
	chb_trigger_two_ways->set_position_size(90, 350, 12, 12);

	lbl_trigger_hidden	= new gui::label(gui_system, wnd_trigger_dlg);
	lbl_trigger_hidden->set_position_size(130, 345, 50, 20);
	lbl_trigger_hidden->text = "Hidden";

	chb_trigger_hidden = new gui::checkbox(gui_system, wnd_trigger_dlg);
	chb_trigger_hidden->set_position_size(190, 350, 12, 12);
}
//-----------------------------------------------------------------------------

void trigger_dlg_show( GameTrigger* trigger )
{
	ASSERT(trigger);
	trigger_obj = trigger;

	chb_trigger_two_ways->checked = !trigger_obj->oneWay;
	chb_trigger_hidden->checked = trigger_obj->hidden;

	iml_trigger_all_objs->clear();
	iml_trigger_objs->clear();

	std::list<GameDynamicObject*>::iterator dit = level->dynamics.begin();
	while( dit != level->dynamics.end() )
	{
		std::list<GameDynamicObject*>::iterator tdit = trigger_obj->triggeredObjs.begin();
		bool found = false;
		while( tdit != trigger_obj->triggeredObjs.end() )
		{
			if( (*tdit)->id == (*dit)->id )
			{
				gui::image_list_item* element = iml_trigger_objs->insertElement( "", (*dit)->id );
				element->user_data = (void*)(*dit);
				found = true;
				break;
			}

			tdit++;
		}

		if( !found )
		{
			if( (*dit)->switchable() )
			{
				gui::image_list_item* element = iml_trigger_all_objs->insertElement( "", (*dit)->id );
				element->user_data = (void*)(*dit);
			}
		}

		dit++;
	}

	wnd_trigger_dlg->set_visibility(true);
	gui_system->show_modal_window(wnd_trigger_dlg);
}
//-----------------------------------------------------------------------------

void trigger_dlg_hide()
{
	wnd_trigger_dlg->set_visibility(false);
	trigger_obj = 0;
}
//-----------------------------------------------------------------------------

void on_trigger_add_obj( gui::button* )
{
	gui::image_list_item* element = iml_trigger_all_objs->getSelection();
	if( !element )
		return;

	GameDynamicObject* obj = (GameDynamicObject*)element->user_data;
	ASSERT(obj);

	gui::image_list_item* new_element = iml_trigger_objs->insertElement( "", element->text );
	new_element->user_data = (void*)obj;

	iml_trigger_all_objs->removeElement(element->id());
}
//-----------------------------------------------------------------------------

void on_trigger_remove_obj( gui::button* )
{
	gui::image_list_item* element = iml_trigger_objs->getSelection();
	if( !element )
		return;

	GameDynamicObject* obj = (GameDynamicObject*)element->user_data;
	ASSERT(obj);	

	gui::image_list_item* new_element = iml_trigger_all_objs->insertElement( "", element->text );
	new_element->user_data = (void*)obj;

	iml_trigger_objs->removeElement(element->id());
}
//-----------------------------------------------------------------------------

void on_trigger_ok( gui::button* )
{
	ASSERT(trigger_obj);

	trigger_obj->hidden = chb_trigger_hidden->checked;
	trigger_obj->oneWay = !chb_trigger_two_ways->checked;

	trigger_obj->triggeredObjs.clear();
	for( uint32 i=0; i<iml_trigger_objs->size(); i++ )
	{
		GameDynamicObject* obj = (GameDynamicObject*)((*iml_trigger_objs)[i])->user_data;
		ASSERT(obj);
		trigger_obj->triggeredObjs.push_back(obj);
	}

	trigger_dlg_hide();
}
//-----------------------------------------------------------------------------

void on_trigger_cancel( gui::button* )
{
	trigger_dlg_hide();
}
//-----------------------------------------------------------------------------