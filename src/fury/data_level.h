#pragma once
//-----------------------------------------------------------------------------

#include <string>
#include <list>
#include <map>
#include <hgerect.h>
#include "data_player.h"
#include "data_effect.h"
#include "background.h"
//-----------------------------------------------------------------------------

class GameStaticObject;
class GameDynamicObject;
class CollisionShape;
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

class GameLevel
{
public:
	GameLevel();
	~GameLevel();

	std::string file;

	//	fields
	std::string	name;
	std::string	music;
	int				musicID;
	std::string	tileset;
	int				tilesetID;

	vector2f		startPoint;

	bool			furriesAvailibility[4];

	// objects
	std::list<GameStaticObject*>	statics;
	std::list<GameStaticObject*>	backgrounds;
	std::list<GameStaticObject*>	foregrounds;
	std::list<CollisionShape*>		collisions;
	std::list<GameDynamicObject*>	dynamics;
	std::list<GameEffect*>			effects;

	b2BodyDef	dummyBodyDef;
	b2Body*		dummyBody;

	std::map<std::string, GamePoint*>			points;

	BackgroundRenderer*	backgroundImage;

	GamePlayer*		player;

//	int serialize();
	int load( const std::string& file );

	bool saved() const;
	bool loaded() const;

	void update(const float& deltaTime);
	void render();

//	GameStaticObject*	select( const hgeVector& point );
//	CollisionShape*		selectCollision( const hgeVector& point );

	void tile_add( GameStaticObject* obj );
	void tile_remove( GameStaticObject* obj );

	void collision_add( CollisionShape* obj );
	void collision_remove( CollisionShape* obj );

	void effect_add( GameEffect* effect );

	void run();

private:
	
	bool changesSaved;
	bool is_loaded;
};
//-----------------------------------------------------------------------------