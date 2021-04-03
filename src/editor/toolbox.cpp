#include "toolbox.h"

#include "system_interfaces.h"
#include "sys_vars.h"
#include "system_main.h"
#include "system_paths.h"
#include "system_uistatemanager.h"
#include "data_tileset.h"

#include "ui_panel.h"
#include "ui_dropdownlist.h"
#include "ui_imagelist.h"
#include "ui_il_element.h"

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

#include "hgeanim.h"

#include <vector>
//-----------------------------------------------------------------------------

using namespace std;
//-----------------------------------------------------------------------------

gui::panel*			pnl_toolbox;
gui::drop_list*		ddl_edit_modes;
gui::drop_list*		ddl_tile_groups;
gui::image_list*	iml_tiles;
//-----------------------------------------------------------------------------

hgeAnimation* geometry_square = 0;
hgeAnimation* geometry_round = 0;
hgeAnimation* geometry_triangle = 0;
//-----------------------------------------------------------------------------

void edit_modes_change( uint32 selection_id );
void tile_groups_change( uint32 selection_id );
void on_tile_select( gui::image_list_item* item );
//-----------------------------------------------------------------------------

void iml_load_tiles( uint32 group );
void iml_load_geometry();
void iml_load_dynamics();
GameStaticObject* construct_object( Tile* tile );
GameDynamicObject* construct_dynamic( Tile* tile );
//-----------------------------------------------------------------------------

void toolbox_init()
{
	HTEXTURE tex = textureManager->getTexture(Path::gfxicons + "square.png");
	geometry_square = new hgeAnimation(tex, 1, 1.0f, 0.0f, 0.0f, hge->Texture_GetWidth(tex), hge->Texture_GetHeight(tex));
	tex = textureManager->getTexture(Path::gfxicons + "triangle.png");
	geometry_triangle = new hgeAnimation(tex, 1, 1.0f, 0.0f, 0.0f, hge->Texture_GetWidth(tex), hge->Texture_GetHeight(tex));
	tex = textureManager->getTexture(Path::gfxicons + "circle.png");
	geometry_round = new hgeAnimation(tex, 1, 1.0f, 0.0f, 0.0f, hge->Texture_GetWidth(tex), hge->Texture_GetHeight(tex));

	pnl_toolbox = new gui::panel( gui_system );
	pnl_toolbox->transparency = 0.8f;
	pnl_toolbox->bounds = hgeRect( (float)(screen_width - 250), 0.0f, (float)screen_width, (float)screen_height );

	ddl_edit_modes = new gui::drop_list( gui_system, pnl_toolbox );
	ddl_edit_modes->bounds = hgeRect(10.0f, 10.0f, 240.0f, 30.0f);
	ddl_edit_modes->on_change = edit_modes_change;

	ddl_tile_groups = new gui::drop_list( gui_system, pnl_toolbox );
	ddl_tile_groups->bounds = hgeRect( 10.0f, 40.0f, 240.0f, 60.0f );
	ddl_tile_groups->on_change = tile_groups_change;

	iml_tiles = new gui::image_list( gui_system, pnl_toolbox );
	iml_tiles->bounds = hgeRect( 10.0f, 70.0f, 240.0f, screen_height - 10.0f );
	iml_tiles->on_select = on_tile_select;

	ddl_tile_groups->insertElement( "Задний план" );
	ddl_tile_groups->insertElement( "Статика" );
	ddl_tile_groups->insertElement( "Передний план" );
	ddl_tile_groups->ddlTextColor = 0xFFFFFFFF;
	ddl_tile_groups->setSelection(2);

	ddl_edit_modes->insertElement( "Спрайты" );
	ddl_edit_modes->insertElement( "Геометрия" );
	ddl_edit_modes->insertElement( "Динамич. объекты" );
	ddl_edit_modes->ddlTextColor = 0xFFFFFFFF;
	ddl_edit_modes->setSelection(1);

	pnl_toolbox->set_visibility(false);
}
//-----------------------------------------------------------------------------

void toolbox_show()
{
	pnl_toolbox->set_visibility(true);
}
//-----------------------------------------------------------------------------

void toolbox_hide()
{
	pnl_toolbox->set_visibility(false);
}
//-----------------------------------------------------------------------------

void edit_modes_change( uint32 selection_id )
{
	switch( selection_id )
	{
	case 0:
		ddl_tile_groups->set_visibility(true);
		iml_tiles->bounds = hgeRect( 10.0f, 70.0f, 240.0f, screen_height - 10.0f );
		iml_tiles->columns = 2;
		ddl_tile_groups->setSelection(0);
		break;
	case 1:
		ddl_tile_groups->set_visibility(false);
		iml_tiles->bounds = hgeRect( 10.0f, 40.0f, 240.0f, screen_height - 10.0f );
		iml_tiles->columns = 1;
		stateManager->setEditLayerState(ES_COLLISIONS);
		iml_load_geometry();
		break;
	case 2:
		ddl_tile_groups->set_visibility(false);
		iml_tiles->bounds = hgeRect( 10.0f, 40.0f, 240.0f, screen_height - 10.0f );
		iml_tiles->columns = 1;
		stateManager->setEditLayerState(ES_DYNAMICS);
		iml_load_dynamics();
		break;
	default:
		ASSERT(0);
		break;
	}
}
//-----------------------------------------------------------------------------

void tile_groups_change( uint32 selection_id )
{
	iml_load_tiles( selection_id );
}
//-----------------------------------------------------------------------------

void iml_load_tiles( uint32 group )
{
	iml_tiles->clear();

	if( !g_tileset )
		return;

	vector<Tile*>* tiles;

	if( group == 0 )
	{
		tiles = &g_tileset->backgrounds;
		stateManager->setEditLayerState(ES_BACKGROUND);
	}
	else if( group == 1 )
	{
		tiles = &g_tileset->statics;
		stateManager->setEditLayerState(ES_STATICS);
	}
	else if( group == 2 )
	{
		tiles = &g_tileset->foregrounds;
		stateManager->setEditLayerState(ES_FOREGROUND);
	}
	else
	{
		ASSERT(0);
	}

	for( uint32 i=0; i<tiles->size(); i++ )
	{
		Tile* t = (*tiles)[i];

		float x, y, w, h;
		t->anim->GetTextureRect(&x, &y, &w, &h);

		iml_tiles->insertElement( t->texture, t->name, x, y, w, h, t->iconScale );
	}
}
//-----------------------------------------------------------------------------

void iml_load_geometry()
{
	iml_tiles->clear();

	iml_tiles->insertElement( geometry_square, "Square" );
	iml_tiles->insertElement( geometry_triangle, "Triangle" );
	iml_tiles->insertElement( geometry_round, "Round" );
}
//-----------------------------------------------------------------------------

void iml_load_dynamics()
{
	iml_tiles->clear();

	if( !g_tileset )
		return;

	for( uint32 i=0; i<g_tileset->dynamics.size(); i++ )
	{
		Tile* t = g_tileset->dynamics[i];

		float x, y, w, h;
		t->anim->GetTextureRect(&x, &y, &w, &h);

		iml_tiles->insertElement( t->texture, t->name, x, y, w, h, t->iconScale );
	}
}
//-----------------------------------------------------------------------------

void on_tile_select( gui::image_list_item* item )
{
	EDITSTATE state = stateManager->getEditLayerState();

	switch( state )
	{
	case ES_FOREGROUND:
		{
			if( selectedForegroundTemplate )
				delete selectedForegroundTemplate;
			if( item )
			{
				selectedForegroundTemplate = construct_object( g_tileset->foregrounds[item->id()] );
				selectedForegroundTemplate->rotation = 0.0f;
				selectedForegroundTemplate->hflip = false;
				selectedForegroundTemplate->vflip = false;
				selectedForegroundTemplate->fix_bounds();
			}
		}
		break;
	case ES_STATICS:
		{
			if( selectedStaticTemplate )
				delete selectedStaticTemplate;
			if( item )
			{
				selectedStaticTemplate = construct_object( g_tileset->statics[item->id()] );
				selectedStaticTemplate->rotation = 0.0f;
				selectedStaticTemplate->hflip = false;
				selectedStaticTemplate->vflip = false;
				selectedStaticTemplate->fix_bounds();
			}
		}
		break;
	case ES_BACKGROUND:
		{
			if( selectedBackgroundTemplate )
				delete selectedBackgroundTemplate;
			if( item )
			{
				selectedBackgroundTemplate = construct_object( g_tileset->backgrounds[item->id()] );
				selectedBackgroundTemplate->rotation = 0.0f;
				selectedBackgroundTemplate->hflip = false;
				selectedBackgroundTemplate->vflip = false;
				selectedBackgroundTemplate->fix_bounds();
			}
		}
		break;
	case ES_COLLISIONS:
		{

		}
		break;
	case ES_DYNAMICS:
		{
			if( selectedDynamicTemplate )
				delete selectedDynamicTemplate;
			if( item )
			{
				selectedDynamicTemplate = construct_dynamic( g_tileset->dynamics[item->id()] );
				selectedDynamicTemplate->rotation = 0.0f;
				selectedDynamicTemplate->hflip = false;
				selectedDynamicTemplate->vflip = false;
				selectedDynamicTemplate->fix_bounds();
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	selected_hflip = false;
	selected_vflip = false;
	selected_rotation = 0.0f;
}
//-----------------------------------------------------------------------------

GameStaticObject* construct_object( Tile* tile )
{
	ASSERT(tile);

	GameStaticObject* obj = new GameStaticObject();

	obj->texture = tile->anim->GetTexture();
	obj->icon = tile->anim->GetTexture();
	obj->sprite = new hgeAnimation( *tile->anim );
	obj->scale = tile->scale;
	obj->bounds = hgeRect(0.0f, 0.0f, tile->anim->GetWidth() * obj->scale, tile->anim->GetHeight() * obj->scale);
	obj->tilesetSpriteID = tile->id;

	return obj;
}
//-----------------------------------------------------------------------------

GameDynamicObject* construct_dynamic( Tile* tile )
{
	ASSERT(tile);

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
	case DYNAMIC_START_POINT:
		obj = new GameStartPoint();
		break;
	default:
		ASSERT(0);
		break;
	}

	return obj;
}
//-----------------------------------------------------------------------------