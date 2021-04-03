#pragma once

#include <list>
#include "data_dynamicobject_base.h"
#include "ui_imagelist.h"
#include "ui_il_element.h"
#include "ui_checkbox.h"

#ifdef NOEDIT
#include "data_objectdata.h"
#endif

//-----------------------------------------------------------------------------

struct	Tile;
//class	TriggerPropsWindow;
//class	TriggerPropsWindowOKBtn;
//class	TriggerPropsWindowCancelBtn;
//class	AddObjectBtn;
//class	RemoveObjectBtn;
//-----------------------------------------------------------------------------

class GameTrigger: public GameDynamicObject
{
public:
	GameTrigger( Tile* tile );
	~GameTrigger(){};
	
	std::list<GameDynamicObject*>	triggeredObjs;
	std::list<std::string>			tempObjIDs;

	hgeAnimation*	untriggeredSprite;
	hgeAnimation*	triggeredSprite;

	bool	oneWay;
	bool	switched;
	bool	hidden;

	virtual void				serialize( FILE* stream );

	virtual GameDynamicObject*	clone();

	virtual void				setPosition( const vector2f& pos );

	virtual void				render( const bool& masked = false );

#ifdef NOEDIT

	b2Body*			body;

	ObjectData		data;

	virtual void	initInBox2d();
	virtual void	toggle();

#else

//	static TriggerPropsWindow*	propsWindow;

	virtual void				showPropsWindow();

#endif

protected:
	Tile*						tile;
};
//-----------------------------------------------------------------------------
#ifndef NOEDIT
/*
class TriggerPropsWindow: public gui::window
{
public:
	TriggerPropsWindow(gui::system* _controller, gui::_base* parent = 0);
	virtual ~TriggerPropsWindow(){};

	gui::image_list*					il_all_objs;
	gui::image_list*					il_triggered_objs;
	AddObjectBtn*					btn_add_object;
	RemoveObjectBtn*				btn_remove_object;

	TriggerPropsWindowOKBtn*		btn_ok;
	TriggerPropsWindowCancelBtn*	btn_cancel;

	gui::label*						lbl_twoWays;
	gui::checkbox*						check_twoWays;
	gui::label*						lbl_hidden;
	gui::checkbox*						check_hidden;

	GameTrigger*					caller;
};
//-----------------------------------------------------------------------------

class AddObjectBtn: public gui::image_button
{
public:
	AddObjectBtn(gui::system* _controller, gui::_base* parent = 0): image_button(_controller, parent){};
	virtual ~AddObjectBtn(){};

	virtual void onButtonClick()
	{
		assert(GameTrigger::propsWindow);
		assert(GameTrigger::propsWindow->caller);

		gui::image_list_item* element = GameTrigger::propsWindow->il_all_objs->getSelection();
		if( !element )
			return;

		GameDynamicObject* obj = static_cast<GameDynamicObject*>(element->user_data);
		assert(obj);	

		gui::image_list_item* new_element = GameTrigger::propsWindow->il_triggered_objs->insertElement( "", element->text );
		new_element->user_data = (void*)obj;

		GameTrigger::propsWindow->il_all_objs->removeElement(element->id());
	}
};
//-----------------------------------------------------------------------------

class RemoveObjectBtn: public gui::image_button
{
public:
	RemoveObjectBtn(gui::system* _controller, gui::_base* parent = 0): image_button(_controller, parent){};
	virtual ~RemoveObjectBtn(){};

	virtual void onButtonClick()
	{
		assert(GameTrigger::propsWindow);
		assert(GameTrigger::propsWindow->caller);

		gui::image_list_item* element = GameTrigger::propsWindow->il_triggered_objs->getSelection();
		if( !element )
			return;

		GameDynamicObject* point = static_cast<GameDynamicObject*>(element->user_data);
		assert(point);	

		gui::image_list_item* new_element = GameTrigger::propsWindow->il_all_objs->insertElement( "", element->text );
		new_element->user_data = (void*)point;

		GameTrigger::propsWindow->il_triggered_objs->removeElement(element->id());
	}
};
//-----------------------------------------------------------------------------

class TriggerPropsWindowOKBtn: public gui::image_button
{
public:
	TriggerPropsWindowOKBtn(gui::system* _controller, gui::_base* parent = 0): image_button(_controller, parent){};
	virtual ~TriggerPropsWindowOKBtn(){};

	virtual void onButtonClick()
	{
		assert(GameTrigger::propsWindow);
		assert(GameTrigger::propsWindow->caller);

		GameTrigger::propsWindow->caller->triggeredObjs.clear();
		gui::image_list* points_list = GameTrigger::propsWindow->il_triggered_objs;
		for( unsigned i=0; i<GameTrigger::propsWindow->il_triggered_objs->size(); i++ )
		{
			gui::image_list_item* element = (*points_list)[i];
			GameDynamicObject* point = static_cast<GameDynamicObject*>(element->user_data);
			assert(point);
			GameTrigger::propsWindow->caller->triggeredObjs.push_back(point);
		}

		GameTrigger::propsWindow->caller->hidden = GameTrigger::propsWindow->check_hidden->checked;
		GameTrigger::propsWindow->caller->oneWay = !GameTrigger::propsWindow->check_twoWays->checked;

		GameTrigger::propsWindow->set_visibility(false);
		controller->set_focus(0);
	}
};
//-----------------------------------------------------------------------------

class TriggerPropsWindowCancelBtn: public gui::image_button
{
public:
	TriggerPropsWindowCancelBtn(gui::system* _controller, gui::_base* parent = 0): image_button(_controller, parent){};
	virtual ~TriggerPropsWindowCancelBtn(){};

	virtual void onButtonClick()
	{
		GameTrigger::propsWindow->set_visibility(false);
		controller->set_focus(0);
	}
};
//-----------------------------------------------------------------------------
*/
#endif