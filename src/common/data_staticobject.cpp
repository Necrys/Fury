/*=============================================================================

	The Fury

DESCRIPTION:
	static game object

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
14/05/2010	main functions
07/05/2010	initial version
=============================================================================*/


#include <hgeanim.h>
#include "data_staticobject.h"
#include "system_interfaces.h"
#include "system_main.h"
#include "sys_vars.h"

//-----------------------------------------------------------------------------

GameStaticObject::GameStaticObject()
{
	position		= vector2f(0.0f, 0.0f);
	scale			= 1.0f;
	rotation		= 0.0f;
	texture			= 0;
	icon			= 0;
	id				= "";
	bounds			= hgeRect(0.0f, 0.0f, 1.0f, 1.0f);
	sprite			= 0;
	shadowedMask	= 0x99AAAAAA;
	visible			= true;
	textureFilter	= false;
	tilesetSpriteID = 0;
	hflip			= false;
	vflip			= false;

//	category = 0;
}
//-----------------------------------------------------------------------------

GameStaticObject::GameStaticObject(const GameStaticObject& src)
{
	position = src.position;
	scale = src.scale;
	rotation = src.rotation;
	texture = src.texture;
	icon = src.icon;
	id = src.id;
	bounds = src.bounds;
	if( src.sprite )
		sprite = new hgeAnimation(*src.sprite);
	else
		sprite = 0;
	shadowedMask = src.shadowedMask;
	visible = src.visible;
	textureFilter = src.textureFilter;
	tilesetSpriteID = src.tilesetSpriteID;
	hflip			= src.hflip;
	vflip			= src.vflip;
}
//-----------------------------------------------------------------------------

GameStaticObject::~GameStaticObject()
{
	if( sprite )
		delete sprite;
};
//-----------------------------------------------------------------------------

void GameStaticObject::update( const float& delta )
{
	if( sprite )
	{
		if( sprite->IsPlaying() )
			sprite->Update(delta);
		else
			sprite->Play();
	}
}
//-----------------------------------------------------------------------------

void GameStaticObject::render(const bool& shadowed)
{
	if( visible && sprite )
	{
		bool filter = hge->System_GetState(HGE_TEXTUREFILTER);
		hge->System_SetState(HGE_TEXTUREFILTER, textureFilter);

		float hot_x, hot_y;
		sprite->GetHotSpot(&hot_x, &hot_y);
		hgeRect sprite_rect;
		sprite_rect.x1 = position.x - abs( bounds.x1 );
		sprite_rect.x2 = sprite_rect.x1 + abs( bounds.x2 - bounds.x1 );
		sprite_rect.y1 = position.y - abs( bounds.y1 );
		sprite_rect.y2 = sprite_rect.y1 + abs( bounds.y2 - bounds.y1 );

		if( !camera_rect.Intersect(&sprite_rect) )
			return;

		sprite->SetFlip( hflip, vflip );

		if( shadowed )
		{
			sprite->SetColor(shadowedMask);
			sprite->RenderEx(float((int)position.x), float((int)position.y), rotation, scale, scale);
		}
		else
		{
			sprite->SetColor(0xFFFFFFFF);
			sprite->RenderEx(float((int)position.x), float((int)position.y), rotation, scale, scale);
		}
		hge->System_SetState(HGE_TEXTUREFILTER, filter);
		sprite->SetFlip( false, false );
	}
}
//-----------------------------------------------------------------------------

void GameStaticObject::fix_bounds()
{
	hgeRect basic_bounds = hgeRect(0.0f, 0.0f, sprite->GetWidth() * scale, sprite->GetHeight() * scale);
	hgeRect new_bounds;

	if( abs(rotation) < 0.0005f )
	{
		new_bounds = basic_bounds;
	}
	else if( abs(rotation - PI_2) < 0.0005f )
	{
		new_bounds = hgeRect( -basic_bounds.y2, 0.0f, 0.0f, basic_bounds.x2 );
	}
	else if( abs(rotation - PI) < 0.0005f )
	{
		new_bounds = hgeRect( -basic_bounds.x2, -basic_bounds.y2, 0.0f , 0.0f );
	}
	else if( abs(rotation - (PI + PI_2)) < 0.0005f )
	{
		new_bounds = hgeRect( 0.0f, -basic_bounds.x2, basic_bounds.y2, 0.0f );
	}

	bounds = new_bounds;
}