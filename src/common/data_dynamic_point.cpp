#include "hgeanim.h"
#include "hgefont.h"
#include "data_dynamic_point.h"
#include "data_level.h"
#include "system_interfaces.h"
#include "system_paths.h"

//-----------------------------------------------------------------------------
#ifndef NOEDIT
//PointPropsWindow* GamePoint::propsWindow = 0;
#include "dialog_point.h"
#endif
//-----------------------------------------------------------------------------

GamePoint::GamePoint( Tile* _tile )
{
	assert(_tile);
	tile = _tile;

	texture = tile->anim->GetTexture();
	icon	= tile->anim->GetTexture();
	sprite	= new hgeAnimation( *tile->anim );
	scale	= tile->scale;

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;
	bounds = hgeRect( 0, 0, half_width * 2.0f , half_height * 2.0f);

	id = "new_point";

	type = DYNAMIC_POINT;

	tilesetSpriteID = tile->id;

#ifndef NOEDIT
/*	if( !propsWindow )
	{
		propsWindow = new PointPropsWindow(gui_system);
		propsWindow->bounds = hgeRect(200.0f, 300.0f, 500.0f, 410.0f );
		propsWindow->caption = "Свойства точки";
		propsWindow->set_visibility(false);
	}*/
	just_added = true;
#endif
}
//-----------------------------------------------------------------------------

GameDynamicObject* GamePoint::clone()
{
	GamePoint* new_point = new GamePoint(tile);
	new_point->id = id;
	new_point->position = position;

	return static_cast<GameDynamicObject*>(new_point);
}
//-----------------------------------------------------------------------------

void GamePoint::serialize( FILE* stream )
{
	assert(stream);

	struct dynamic_point
	{
		unsigned int	type;
		unsigned int	obj_type;
		unsigned int	tile_id;
		float			position_x;
		float			position_y;
		char			name[128];
	} obj;

	obj.type = FLF_SECTION_DYNAMIC;
	obj.obj_type = DYNAMIC_POINT;
	obj.tile_id = tilesetSpriteID;
	obj.position_x = position.x;
	obj.position_y = position.y;
	memset(obj.name, 0, sizeof(char)*128);
	strcpy_s(obj.name, 128, id.c_str());

	fwrite( &obj, sizeof(dynamic_point), 1, stream );
}
//-----------------------------------------------------------------------------
#ifndef NOEDIT

void GamePoint::showPropsWindow()
{
	point_dlg_show( this, just_added );
	just_added = false;
}
//-----------------------------------------------------------------------------
#endif