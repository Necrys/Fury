#pragma once

#include <list>
#include "data_dynamicobject_base.h"
#include "ui_window.h"
#include "ui_label.h"
#include "ui_editbox.h"
#include "ui_imagebutton.h"
#include "ui_imagelist.h"
#include "ui_il_element.h"
#include "ui_checkbox.h"
#include "ui_main.h"
#include "helper_functions.h"

#ifdef NOEDIT
#include "data_objectdata.h"
#endif

//-----------------------------------------------------------------------------

struct	Tile;
class	GamePoint;
//class	LiftPropsWindow;
//class	LiftPropsWindowOKBtn;
// class	LiftPropsWindowCancelBtn;
//class	AddPointBtn;
//class	RemovePointBtn;

//-----------------------------------------------------------------------------

enum LIFT_MOVE_ORDER
{
	MOVE_LOOP,
	MOVE_REVERSED,
	MOVE_ONCE,
};
//-----------------------------------------------------------------------------

class GameLift: public GameDynamicObject
{
public:
	GameLift( Tile* tile );
	~GameLift(){};

	std::list<GamePoint*>	points;
	std::list<std::string>	pointsTemp;
	LIFT_MOVE_ORDER			moveOrder;
	bool					enabled;

	float					density;
	float					friction;
	float					restitution;
	float					speed;

	virtual void				serialize( FILE* stream );

	virtual GameDynamicObject*	clone();

	virtual void				setPosition( const vector2f& pos );

#ifndef NOEDIT
//	static LiftPropsWindow*	propsWindow;

	virtual void				showPropsWindow();
#endif

#ifdef NOEDIT

	virtual void update( const float& delta );

	virtual void initInBox2d();

	b2PrismaticJoint*	joint;
	b2Body*				body;

	ObjectData		data;

	bool			reverse;

	float			tempSpeed;

	virtual void toggle();

#endif

private:

	GamePoint*	getNextPoint() const;

	Tile*					tile;
};
//-----------------------------------------------------------------------------

#ifndef NOEDIT
/*
class LiftPropsWindow: public gui::window
{
public:
	LiftPropsWindow(gui::system* _controller, gui::_base* parent = 0);
	virtual ~LiftPropsWindow(){};

	gui::label*		id_label;
	gui::edit_box*		id_edit;
	gui::image_list*	all_points_list;
	gui::image_list*	lift_points_list;
	AddPointBtn*	add_point_btn;
	RemovePointBtn*	remove_point_btn;
	gui::checkbox*		check_enabled;
	gui::label*		lbl_enabled;

	LiftPropsWindowOKBtn*		ok_btn;
	LiftPropsWindowCancelBtn*	cancel_btn;

	GameLift*	caller_point;
};
//-----------------------------------------------------------------------------

class AddPointBtn: public gui::image_button
{
public:
	AddPointBtn(gui::system* _controller, gui::_base* parent = 0): image_button(_controller, parent){};
	virtual ~AddPointBtn(){};

	virtual void onButtonClick()
	{
		assert(GameLift::propsWindow);
		assert(GameLift::propsWindow->caller_point);

		gui::image_list_item* element = GameLift::propsWindow->all_points_list->getSelection();
		if( !element )
			return;

		GamePoint* point = static_cast<GamePoint*>(element->user_data);
		assert(point);	

		gui::image_list_item* new_element = GameLift::propsWindow->lift_points_list->insertElement( "", element->text );
		new_element->user_data = (void*)point;

		GameLift::propsWindow->all_points_list->removeElement(element->id());
	}
};
//-----------------------------------------------------------------------------

class RemovePointBtn: public gui::image_button
{
public:
	RemovePointBtn(gui::system* _controller, gui::_base* parent = 0): image_button(_controller, parent){};
	virtual ~RemovePointBtn(){};

	virtual void onButtonClick()
	{
		assert(GameLift::propsWindow);
		assert(GameLift::propsWindow->caller_point);

		gui::image_list_item* element = GameLift::propsWindow->lift_points_list->getSelection();
		if( !element )
			return;

		GamePoint* point = static_cast<GamePoint*>(element->user_data);
		assert(point);	

		gui::image_list_item* new_element = GameLift::propsWindow->all_points_list->insertElement( "", element->text );
		new_element->user_data = (void*)point;

		GameLift::propsWindow->lift_points_list->removeElement(element->id());
	}
};
//-----------------------------------------------------------------------------

class LiftPropsWindowOKBtn: public gui::image_button
{
public:
	LiftPropsWindowOKBtn(gui::system* _controller, gui::_base* parent = 0): image_button(_controller, parent){};
	virtual ~LiftPropsWindowOKBtn(){};

	virtual void onButtonClick()
	{
		assert(GameLift::propsWindow);
		assert(GameLift::propsWindow->caller_point);

		GameLift::propsWindow->caller_point->points.clear();
		gui::image_list* points_list = GameLift::propsWindow->lift_points_list;
		for( unsigned i=0; i<GameLift::propsWindow->lift_points_list->size(); i++ )
		{
			gui::image_list_item* element = (*points_list)[i];
			GamePoint* point = static_cast<GamePoint*>(element->user_data);
			assert(point);
			GameLift::propsWindow->caller_point->points.push_back(point);
		}

		GameLift::propsWindow->caller_point->enabled = GameLift::propsWindow->check_enabled->checked;

		GameLift::propsWindow->caller_point->id = GameLift::propsWindow->id_edit->text;
		GameLift::propsWindow->set_visibility(false);
		controller->set_focus(0);
	}
};
//-----------------------------------------------------------------------------

class LiftPropsWindowCancelBtn: public gui::image_button
{
public:
	LiftPropsWindowCancelBtn(gui::system* _controller, gui::_base* parent = 0): image_button(_controller, parent){};
	virtual ~LiftPropsWindowCancelBtn(){};

	virtual void onButtonClick()
	{
		GameLift::propsWindow->set_visibility(false);
		controller->set_focus(0);
	}
};
//-----------------------------------------------------------------------------
*/
#endif