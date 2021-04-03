#pragma once

#include "data_dynamicobject_base.h"
#include "data_tileset.h"
#include "ui_window.h"
#include "ui_imagebutton.h"
#include "ui_label.h"
#include "ui_editbox.h"
#include "str_helpers.h"
#include "ui_main.h"
#include "system_main.h"
#include "system_interfaces.h"
#include "data_level.h"
//-----------------------------------------------------------------------------

class GamePoint : public GameDynamicObject
{
public:
	GamePoint( Tile* tile );
	virtual ~GamePoint(){};

	virtual void serialize( FILE* stream );

	virtual GameDynamicObject* clone();

	virtual void toggle(){};

	std::string	id;
	Tile*		tile;

#ifndef NOEDIT
	virtual void showPropsWindow();
	bool just_added;
#endif

#ifdef NOEDIT
	virtual void initInBox2d(){};
#endif
};
//-----------------------------------------------------------------------------
