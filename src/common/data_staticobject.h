#pragma once

#include "types.h"
#include "hge.h"
#include <string>
#include <hgerect.h>

#include "types.h"
#ifdef NOEDIT
#include "system_collider.h"
#endif

//-----------------------------------------------------------------------------

class hgeAnimation;

//-----------------------------------------------------------------------------

#define SPRITE_FLIP_HORIZONTAL	0x00000001
#define SPRITE_FLIP_VERTICAL	0x00000002
#define	SPRITE_ROTATE_90		0x00000004
#define SPRITE_ROTATE_180		0x00000008
#define	SPRITE_ROTATE_270		0x0000000C
//-----------------------------------------------------------------------------

class GameStaticObject
{
public:
	GameStaticObject();
	GameStaticObject(const GameStaticObject& src);
	virtual ~GameStaticObject();

	vector2f		position;
	float			scale;
	float			rotation;
	HTEXTURE		texture;
	HTEXTURE		icon;		// ui image
	std::string	id;
	hgeRect			bounds;
	uint32			shadowedMask;
	bool			visible;
	bool			textureFilter;
	unsigned		tilesetSpriteID;
	bool			hflip;
	bool			vflip;

	OBJ_CATEGORY	category;

	virtual void update( const float& delta );

	virtual void render( const bool& shadowed = false );

	void fix_bounds();
#ifdef NOEDIT
	virtual void onCollision( b2Fixture* shape, contact_point* point ){};
#endif

//private:
	hgeAnimation*	sprite;
};
//-----------------------------------------------------------------------------