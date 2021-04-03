#pragma once

#include <string>
#include <list>
#include <map>
#include "types.h"

//-----------------------------------------------------------------------------

class GameStaticObject;
class CollisionShape;
class GameDynamicObject;
class BackgroundRenderer;
class GamePoint;
//-----------------------------------------------------------------------------

enum FLF_SECTION
{
	FLF_SECTION_MAGIC			=	0,	//	4 chars - magic number
	FLF_SECTION_NAME			=	1,	//	name - 48 chars
	FLF_SECTION_MUSIC			=	2,	//	music - 48 chars
	FLF_SECTION_TILESET			=	3,	//	tileset - 48 chars
	FLF_SECTION_STATIC			=	4,	//	static object
	FLF_SECTION_PHYSIC_GEOMETRY	=	5,	//	physic geometry object
	FLF_SECTION_DYNAMIC			=	6,	//	dynamic objects
	FLF_SECTION_BACKGROUND		=	7,	//	background objects
	FLF_SECTION_FOREGROUND		=	8,	//	foreground objects
};
//-----------------------------------------------------------------------------

enum LEVEL_LAYER
{
	LAYER_BACKGROUND	= 0,
	LAYER_STATIC		= 1,
	LAYER_FOREGROUND	= 2,
	LAYER_UNKNOWN		= 3,
};
//-----------------------------------------------------------------------------

class GameLevel
{
public:
	GameLevel();
	~GameLevel();

	std::string file;

	//	fields
	std::string	name;
	std::string	music;
	int			musicID;
	std::string	tileset;
	int			tilesetID;
	bool		hasStartPoint;

	// objects
	std::list<GameStaticObject*>	statics;
	std::list<GameStaticObject*>	foregrounds;
	std::list<GameStaticObject*>	backgrounds;
	std::list<CollisionShape*>		collisions;
	std::list<GameDynamicObject*>	dynamics;

	std::map<std::string, GamePoint*>	points;

	BackgroundRenderer*				backgroundImage;

	int save();
	int load( const std::string& file );

	bool saved() const;

	void update(const float& deltaTime);
	void render();

	GameStaticObject*	tile_select( const vector2f& point, LEVEL_LAYER layer );
	CollisionShape*		collision_select( const vector2f& point );
	GameDynamicObject*	dynamic_select( const vector2f& point );

	void tile_add( GameStaticObject* obj, LEVEL_LAYER layer = LAYER_STATIC, bool toend = true );
	void tile_remove( GameStaticObject* obj, LEVEL_LAYER layer = LAYER_STATIC );

	void collision_add( CollisionShape* obj );
	void collision_remove( CollisionShape* obj );

	void dynamic_add( GameDynamicObject* obj );
	void dynamic_remove( GameDynamicObject* obj );

private:
	
	bool changesSaved;
};