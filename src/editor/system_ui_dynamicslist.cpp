/*
#include <hgeanim.h>
#include "data_dynamicobject_base.h"
#include "data_dynamic_start.h"
#include "data_dynamic_moveable.h"
#include "data_dynamic_destructable.h"
#include "data_dynamic_crunch.h"
#include "data_dynamic_point.h"
#include "data_dynamic_lift.h"
#include "data_dynamic_door.h"
#include "data_dynamic_trigger.h"
#include "data_item_key.h"
#include "data_item_coin.h"
#include "system_ui.h"
#include "system_paths.h"
#include "system_interfaces.h"
#include "system_main.h"
#include "helper_functions.h"
#include "data_tileset.h"

//-----------------------------------------------------------------------------

void DynamicsImageList::reloadTemplates()
{
	templates.clear();
	this->elements.clear();

	GameDynamicObject* object = new GameStartPoint();

	templates.insert(templates.end(), object);
	insertElement( Path::gfxcommon + "player\\fury.tga", "Точка старта" );

	std::vector< Tile* >::iterator it = g_tileset->dynamics.begin();
	while( it != g_tileset->dynamics.end() )
	{
		Tile* tile = *it;

		GameDynamicObject* obj;

		switch( tile->dynamicType )
		{
		case DYNAMIC_MOVEABLE_BOX:
			obj = new GameMovableBox( tile );
			break;
		case DYNAMIC_DESTRUCTABLE:
			obj = new GameDestructable( tile );
			break;
		case DYNAMIC_CRUNCH:
			obj = new GameCrunch( tile );
			break;
		case DYNAMIC_POINT:
			obj = new GamePoint( tile );
			break;
		case DYNAMIC_LIFT:
			obj = new GameLift( tile );
			break;
		case DYNAMIC_DOOR:
			obj = new GameDoor( tile );
			break;
		case DYNAMIC_KEY:
			obj = new GameItemKey( tile );
			break;
		case DYNAMIC_COIN:
			obj = new GameItemCoin( tile );
			break;
		case DYNAMIC_TRIGGER:
			obj = new GameTrigger( tile );
			break;
		default:
			break;
		}

		templates.insert(templates.end(), obj);
		this->insertElement( tile->anim, tile->name, tile->iconScale );

		it++;
	}
}
//-----------------------------------------------------------------------------

void DynamicsImageList::onSelectionChanged(const unsigned int &id)
{
	if( id < 0 || id > templates.size()-1 )
		return;

	selectedDynamicTemplate = templates[id];
}
//-----------------------------------------------------------------------------
*/