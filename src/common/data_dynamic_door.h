#pragma once

#include "data_dynamicobject_base.h"
#include "data_tileset.h"
#include "ui_window.h"
#include "ui_main.h"
#include "str_helpers.h"
#include "ui_imagebutton.h"
#include "ui_label.h"
#include "ui_editbox.h"
#include "ui_dropdownlist.h"

#ifdef NOEDIT
#include "data_objectdata.h"
#endif

//-----------------------------------------------------------------------------
/*
class DoorPropsWindow;
class DoorPropsWindowOKBtn;
class DoorPropsWindowCancelBtn;
*/
//-----------------------------------------------------------------------------

enum DOOR_OPENING_DIR
{
	DIR_UP		= 0,
	DIR_DOWN	= 1,
	DIR_LEFT	= 2,
	DIR_RIGHT	= 3,
};
//-----------------------------------------------------------------------------

class GameDoor: public GameDynamicObject
{
public:
	GameDoor( Tile* _tile );
	virtual ~GameDoor(){};

	virtual void serialize( FILE* stream );

	virtual GameDynamicObject* clone();

	float				friction;
	float				restitution;
	float				density;
	unsigned int		group;
	DOOR_OPENING_DIR	direction;

	virtual void setPosition( const vector2f& pos );

#ifdef NOEDIT

	virtual void update( const float& delta );

	virtual void initInBox2d();

	virtual void toggle();

	//virtual void render( const bool& shadowed = false );

	b2PrismaticJoint*	joint;
	b2Body*				body;

	ObjectData		data;

	bool			opened;

#endif

#ifndef NOEDIT
//	static DoorPropsWindow*	propsWindow;

	virtual void showPropsWindow();
#endif

private:
	Tile* tile;
};
//-----------------------------------------------------------------------------

#ifndef NOEDIT
/*
class DoorPropsWindow: public gui::window
{
public:
	DoorPropsWindow(gui::system* _controller, gui::_base* parent = 0);
	virtual ~DoorPropsWindow(){};

	gui::label*		id_label;
	gui::edit_box*		id_edit;
	gui::label*		lbl_direction;
	gui::drop_list*	ddl_direction;

	DoorPropsWindowOKBtn*		btn_ok;
	DoorPropsWindowCancelBtn*	btn_cancel;

	GameDoor*		caller;
};
//-----------------------------------------------------------------------------

class DoorPropsWindowOKBtn: public gui::image_button
{
public:
	DoorPropsWindowOKBtn(gui::system* _controller, gui::_base* parent = 0): image_button(_controller, parent){};
	virtual ~DoorPropsWindowOKBtn(){};

	virtual void onButtonClick()
	{
		assert(GameDoor::propsWindow->caller);

		GameDoor::propsWindow->caller->id = GameDoor::propsWindow->id_edit->text;

		GameDoor::propsWindow->caller->direction = (DOOR_OPENING_DIR)GameDoor::propsWindow->ddl_direction->getSelectionID();

		GameDoor::propsWindow->set_visibility(false);
		controller->set_focus(0);
	}
};
//-----------------------------------------------------------------------------

class DoorPropsWindowCancelBtn: public gui::image_button
{
public:
	DoorPropsWindowCancelBtn(gui::system* _controller, gui::_base* parent = 0): image_button(_controller, parent){};
	virtual ~DoorPropsWindowCancelBtn(){};

	virtual void onButtonClick()
	{
		GameDoor::propsWindow->set_visibility(false);
		controller->set_focus(0);
	}
};
//-----------------------------------------------------------------------------
*/
#endif