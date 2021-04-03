/*=============================================================================

	The Fury

DESCRIPTION:
	system UI right panel

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
24/05/2010	initial version
=============================================================================*/

#pragma once

#include "ui_panel.h"
#include "ui_imagelist.h"
#include "ui_dropdownlist.h"

class EditModesList;
class StaticsImageList;
class BackgroundIL;
class ForegroundIL;
class CollisionsImageList;
class DynamicsImageList;
class GameStaticObject;
class system;
class _base;
class GameDynamicObject;

//-----------------------------------------------------------------------------

class RightPanel: public gui::panel
{
public:
	EditModesList*			ddlElementTypes;
	StaticsImageList*		ilStaticObjectsList;
	BackgroundIL*			ilBackgroundObjects;
	ForegroundIL*			ilForegroundObjects;
	CollisionsImageList*	ilCollisionsList;
	DynamicsImageList*		ilDynamicsList;

	RightPanel(gui::system* controller, gui::_base* parent = 0);
};
//-----------------------------------------------------------------------------

class StaticsImageList: public gui::image_list
{
public:
	StaticsImageList(gui::system* _controller, gui::_base* parent = 0): image_list(_controller, parent){};
	virtual ~StaticsImageList();

	std::vector<GameStaticObject*> templates;

	virtual void onSelectionChanged(const unsigned& id);
	virtual void reloadTemplates();

	virtual void render();
	virtual void scrollUp();
	virtual void scrollDown();
	virtual void pageUp();
	virtual void pageDown();
};
//-----------------------------------------------------------------------------

class EditModesList: public gui::drop_list
{
public:
	EditModesList(gui::system* _controller, gui::_base* parent = 0): drop_list(_controller, parent){};

	virtual void onSelectionChanged(const unsigned& id);
};
//-----------------------------------------------------------------------------

class CollisionsImageList: public gui::image_list
{
public:
	CollisionsImageList(gui::system* _controller, gui::_base* parent = 0): image_list(_controller, parent){};

	virtual void onSelectionChanged(const unsigned& id);
};
//-----------------------------------------------------------------------------

class DynamicsImageList: public gui::image_list
{
public:
	DynamicsImageList(gui::system* _controller, gui::_base* parent = 0): image_list(_controller, parent){};

	virtual void onSelectionChanged(const unsigned& id);
	virtual void reloadTemplates();

	std::vector<GameDynamicObject*> templates;
};
//-----------------------------------------------------------------------------

class BackgroundIL: public StaticsImageList
{
public:
	BackgroundIL(gui::system* _controller, gui::_base* parent = 0): StaticsImageList(_controller, parent){};
	virtual ~BackgroundIL(){};

	virtual void onSelectionChanged(const unsigned& id);
	virtual void reloadTemplates();
};
//-----------------------------------------------------------------------------

class ForegroundIL: public StaticsImageList
{
public:
	ForegroundIL(gui::system* _controller, gui::_base* parent = 0): StaticsImageList(_controller, parent){};
	virtual ~ForegroundIL(){};

	virtual void onSelectionChanged(const unsigned& id);
	virtual void reloadTemplates();
};
//-----------------------------------------------------------------------------
