#include "types.h"
#include <hgeanim.h>
#include "data_level.h"
#include "data_staticobject.h"
#include "data_collisionshape.h"
#include "data_collision_circle.h"
#include "data_collision_square.h"
#include "data_collision_triangle.h"
#include "data_dynamicobject_base.h"
#include "data_dynamic_moveable.h"
#include "data_dynamic_destructable.h"
#include "data_dynamic_crunch.h"
#include "data_dynamic_point.h"
#include "data_dynamic_trigger.h"
#include "data_dynamic_lift.h"
#include "data_dynamic_door.h"
#include "data_item_key.h"
#include "data_item_coin.h"
#include "data_player.h"
#include "system_main.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "sys_vars.h"
#include "helper_functions.h"

using namespace std;
//-----------------------------------------------------------------------------

uint32 parse_static_sprite( FILE*			file, 
							GameLevel*		level, 
							const uint32&	spec_version, 
							const uint32&	obj_type )
{
	assert( file );
	assert( level );
	assert( spec_version > 0 && spec_version <= 2 );
	assert( g_tileset );

	uint32				type_id;
	GameStaticObject*	obj;
	Tile*				tile;
	uint32				v2flags;

	fread(&type_id, sizeof(uint32), 1, file);

	tile = g_tileset->getTileByID(type_id, obj_type);

	if( !tile )
	{
		log_msg("No tile with ID %d", type_id);
		return -1;
	}

	obj = new GameStaticObject();

	obj->texture = tile->anim->GetTexture();
	obj->icon = tile->anim->GetTexture();
	obj->sprite = new hgeAnimation( *tile->anim );
	obj->scale = tile->scale;
	obj->bounds = hgeRect(0.0f, 0.0f, tile->anim->GetWidth() * obj->scale, tile->anim->GetHeight() * obj->scale);
	obj->tilesetSpriteID = type_id;

	fread(&obj->position.x, sizeof(float), 1, file);
	fread(&obj->position.y, sizeof(float), 1, file);

	if( spec_version >= 2 )
	{
		fread(&v2flags, sizeof(uint32), 1, file);
		obj->hflip = (v2flags & SPRITE_FLIP_HORIZONTAL) == 0 ? false : true;
		obj->vflip = (v2flags & SPRITE_FLIP_VERTICAL) == 0 ? false : true;
		uint32 rotopt = v2flags & 0x0000000C;
		if( rotopt == SPRITE_ROTATE_90 )
			obj->rotation = PI / 2.0f;
		else if( rotopt == SPRITE_ROTATE_180 )
			obj->rotation = PI;
		else if( rotopt == SPRITE_ROTATE_270 )
			obj->rotation = PI + PI / 2.0f;

		obj->fix_bounds();
	}

	switch( obj_type )
	{
	case FLF_SECTION_STATIC:
		{
			level->statics.push_back( obj );
		}
		break;
	case FLF_SECTION_BACKGROUND:
		{
			level->backgrounds.push_back( obj );
		}
		break;
	case FLF_SECTION_FOREGROUND:
		{
			level->foregrounds.push_back( obj );
		}
		break;
	default:
		assert(0);
		break;
	}

	return 0;
}
//-----------------------------------------------------------------------------

GameLevel::GameLevel()
{
	file	= "";				// flf - Fury Level File
	name	= "Новый уровень The Fury";
	music	= "default.ogg";
	tileset = "_default.xml";	// fts - Fury Tile Set
	changesSaved = false;
	musicID	= -1;
	tilesetID = -1;
	is_loaded = false;
	player = 0;
	startPoint = vector2f(0.0f, 0.0f);
	backgroundImage = new BackgroundRenderer();

	for( int i=0; i<4; i++ )
		furriesAvailibility[i] = true;
}
//-----------------------------------------------------------------------------

GameLevel::~GameLevel()
{
	list<GameStaticObject*>::iterator sit = statics.begin();
	while( sit != statics.end() )
	{
		GameStaticObject* obj = *sit;
		if( obj )
			delete obj;

		sit++;
	}

	sit = foregrounds.begin();
	while( sit != foregrounds.end() )
	{
		GameStaticObject* obj = *sit;
		if( obj )
			delete obj;

		sit++;
	}

	sit = backgrounds.begin();
	while( sit != backgrounds.end() )
	{
		GameStaticObject* obj = *sit;
		if( obj )
			delete obj;

		sit++;
	}

	if( player )
		delete player;
}
//-----------------------------------------------------------------------------

int GameLevel::load(const std::string &_file)
{
	// read level data from file

	FILE*	hfile = fopen(_file.c_str(), "rb");;
	char	magic[4];
	uint32	version;

	if( !hfile )
	{
		log_msg(COLOR_ERROR, "Unable to open level file");
		return -1;
	}

	// magic number
	fread(magic, sizeof(char), 4, hfile);

	if( magic[0] == 'F' && magic[1] == 'L' && magic[2] == 'F' )
	{
		if( magic[3] == '1' )
			version = 1;
		else if( ( magic[3] == '2' ) )
			version = 2;
		else
		{
			log_msg(COLOR_ERROR, "Wrong level file header");
			return -1;
		}
	}

	char buffer[48];
	memset(buffer, 0, sizeof(char)*48);

	fread(buffer, sizeof(char), 48, hfile);
	name = buffer;
	memset(buffer, 0, sizeof(char)*48);

	fread(buffer, sizeof(char), 48, hfile);
	music = buffer;
	memset(buffer, 0, sizeof(char)*48);

	fread(buffer, sizeof(char), 48, hfile);
	tileset = buffer;

	if( !g_tileset )
		g_tileset = new Tileset();
	g_tileset->loadTileset(tileset);

	while( !feof(hfile) )
	{
		unsigned int section_id;
		if( fread(&section_id, sizeof(unsigned int), 1, hfile) == 0 )
			break;

		switch( section_id )
		{
		case FLF_SECTION_STATIC:
		case FLF_SECTION_BACKGROUND:
		case FLF_SECTION_FOREGROUND:
			{
				parse_static_sprite( hfile, this, version, section_id );
			}
			break;
		case FLF_SECTION_DYNAMIC:
			{
				unsigned int type_id;
				fread(&type_id, sizeof(unsigned int), 1, hfile);

				switch( type_id )
				{
				case DYNAMIC_START_POINT:
					{
						fread(&startPoint.x, sizeof(float), 1, hfile);
						fread(&startPoint.y, sizeof(float), 1, hfile);
					}
					break;
				case DYNAMIC_MOVEABLE_BOX:
					{
						vector2f	position;
						float		density, friction, restitution;
						unsigned int tile_id;
						fread( &tile_id, sizeof(unsigned int), 1, hfile );
						fread( &position.x, sizeof(float), 1, hfile );
						fread( &position.y, sizeof(float), 1, hfile );
						fread( &density,	sizeof(float), 1, hfile );
						fread( &friction,	sizeof(float), 1, hfile );
						fread( &restitution, sizeof(float), 1, hfile );

						Tile* tile = g_tileset->getDynamicByID(tile_id, (DYNAMIC_TYPE)type_id);
						if( tile )
						{
							GameMovableBox* box = new GameMovableBox( tile );
							box->density = density;
							box->friction = friction;
							box->restitution = restitution;
							box->setPosition( position );

							dynamics.push_back(box);
						}
						else
						{
							log_msg("No dynamic tile with ID %d", tile_id);
						}
					}
					break;
				case DYNAMIC_DESTRUCTABLE:
					{
						vector2f	position;
						float		health, friction, restitution;
						unsigned int tile_id;
						fread( &tile_id, sizeof(unsigned int), 1, hfile );
						fread( &position.x, sizeof(float), 1, hfile );
						fread( &position.y, sizeof(float), 1, hfile );
						fread( &friction,	sizeof(float), 1, hfile );
						fread( &restitution,	sizeof(float), 1, hfile );
						fread( &health, sizeof(float), 1, hfile );

						Tile* tile = g_tileset->getDynamicByID(tile_id, (DYNAMIC_TYPE)type_id);
						if( tile )
						{
							GameDestructable* box = new GameDestructable( tile );
							box->health = health;
							box->friction = friction;
							box->restitution = restitution;

							box->setPosition( position );

							dynamics.push_back(box);
						}
						else
						{
							log_msg("No dynamic tile with ID %d", tile_id);
						}
					}
					break;
				case DYNAMIC_CRUNCH:
					{
						vector2f	position;
						float		friction, restitution;
						unsigned int tile_id;
						unsigned	health;
						fread( &tile_id, sizeof(unsigned int), 1, hfile );
						fread( &position.x, sizeof(float), 1, hfile );
						fread( &position.y, sizeof(float), 1, hfile );
						fread( &friction,	sizeof(float), 1, hfile );
						fread( &restitution,	sizeof(float), 1, hfile );
						fread( &health, sizeof(unsigned), 1, hfile );

						Tile* tile = g_tileset->getDynamicByID(tile_id, (DYNAMIC_TYPE)type_id);
						if( tile )
						{
							GameCrunch* box = new GameCrunch( tile );
							box->health = health;
							box->friction = friction;
							box->restitution = restitution;
							box->setPosition( position );

							dynamics.push_back(box);
						}
						else
						{
							log_msg("No dynamic tile with ID %d", tile_id);
						}
					}
					break;
				case DYNAMIC_POINT:
					{
						vector2f	position;
						unsigned int tile_id;
						char		name[128];
						fread( &tile_id, sizeof(unsigned int), 1, hfile );
						fread( &position.x, sizeof(float), 1, hfile );
						fread( &position.y, sizeof(float), 1, hfile );
						fread( name, sizeof(char), 128, hfile );

						Tile* tile = g_tileset->getDynamicByID(tile_id, (DYNAMIC_TYPE)type_id);
						if( tile )
						{
							GamePoint* box = new GamePoint( tile );
							box->id = name;
							box->position = position;

							if( points.find(box->id) == points.end() )
								points[box->id] = box;
							else
								log_msg( "Point with id %s already exists", name);
						}
						else
						{
							log_msg("No dynamic tile with ID %d", tile_id);
						}
					}
					break;
				case DYNAMIC_LIFT:
					{
						vector2f	position;
						unsigned int tile_id;
						float		friction, restitution, density, speed;
						unsigned int	points_count;
						LIFT_MOVE_ORDER	moveOrder;
						char		name[128];
						unsigned int 	enabled;
						fread( &tile_id, sizeof(unsigned int), 1, hfile );
						fread( &position.x, sizeof(float), 1, hfile );
						fread( &position.y, sizeof(float), 1, hfile );
						fread( &density,	sizeof(float), 1, hfile );
						fread( &friction,	sizeof(float), 1, hfile );
						fread( &restitution, sizeof(float), 1, hfile );
						fread( &speed, sizeof(float), 1, hfile );
						fread( &points_count, sizeof(unsigned int), 1, hfile );
						fread( &moveOrder, sizeof(unsigned int), 1, hfile );
						fread( &enabled, sizeof(unsigned int), 1, hfile);
						fread( name, sizeof(char), 128, hfile );

						Tile* tile = g_tileset->getDynamicByID(tile_id, (DYNAMIC_TYPE)type_id);
						if( tile )
						{
							GameLift* box = new GameLift( tile );
							box->id = name;
							box->setPosition(position);
							box->density = density;
							box->restitution = restitution;
							box->friction = friction;
							box->speed = speed;
							box->moveOrder = moveOrder;
							box->enabled = enabled == 0 ? false : true;

							char point_id[128];
							for( uint32 i=0; i<points_count; i++ )
							{
								memset(point_id, 0, sizeof(char)*128);
								fread(point_id, sizeof(char), 128, hfile);
								string s_point_id = point_id;
								box->pointsTemp.push_back(s_point_id);
							}

							dynamics.push_back(box);
						}
						else
						{
							log_msg("No dynamic tile with ID %d", tile_id);
						}
					}
					break;
				case DYNAMIC_DOOR:
					{
						vector2f		position;
						unsigned int	tile_id, group, direction;
						float			friction, restitution, density;
						char			name[128];
						fread( &tile_id, sizeof(unsigned int), 1, hfile );
						fread( &position.x, sizeof(float), 1, hfile );
						fread( &position.y, sizeof(float), 1, hfile );
						fread( &density,	sizeof(float), 1, hfile );
						fread( &friction,	sizeof(float), 1, hfile );
						fread( &restitution, sizeof(float), 1, hfile );
						fread( &group, sizeof(unsigned int), 1, hfile );
						fread( &direction, sizeof(unsigned int), 1, hfile );
						fread( name, sizeof(char), 128, hfile );

						Tile* tile = g_tileset->getDynamicByID(tile_id, (DYNAMIC_TYPE)type_id);
						if( tile )
						{
							GameDoor* box = new GameDoor( tile );
							box->setPosition(position);
							box->density = density;
							box->restitution = restitution;
							box->friction = friction;
							box->group = group;
							box->direction = (DOOR_OPENING_DIR)direction;
							box->id = name;

							dynamics.push_back(box);
						}
						else
						{
							log_msg("No dynamic tile with ID %d", tile_id);
						}
					}
					break;
				case DYNAMIC_KEY:
					{
						vector2f		position;
						unsigned int	tile_id, group;
						fread( &tile_id,	sizeof(unsigned int), 1, hfile );
						fread( &position.x, sizeof(float), 1, hfile );
						fread( &position.y, sizeof(float), 1, hfile );
						fread( &group, sizeof(unsigned int), 1, hfile );

						Tile* tile = g_tileset->getDynamicByID(tile_id, (DYNAMIC_TYPE)type_id);
						if( tile )
						{
							GameItemKey* box = new GameItemKey( tile );
							box->setPosition(position);
							box->group = group;

							dynamics.push_back(box);
						}
						else
						{
							log_msg("No dynamic tile with ID %d", tile_id);
						}
					}
					break;
				case DYNAMIC_COIN:
					{
						vector2f		position;
						unsigned int	tile_id, value;
						fread( &tile_id,	sizeof(unsigned int), 1, hfile );
						fread( &position.x, sizeof(float), 1, hfile );
						fread( &position.y, sizeof(float), 1, hfile );
						fread( &value, sizeof(unsigned int), 1, hfile );

						Tile* tile = g_tileset->getDynamicByID(tile_id, (DYNAMIC_TYPE)type_id);
						if( tile )
						{
							GameItemCoin* box = new GameItemCoin( tile );
							box->setPosition(position);
							box->value = value;

							dynamics.push_back(box);
						}
						else
						{
							log_msg("No dynamic tile with ID %d", tile_id);
						}
					}
					break;
				case DYNAMIC_TRIGGER:
					{
						vector2f		position;
						unsigned int	tile_id;
						unsigned int	one_way;
						unsigned int	hidden;
						unsigned int	points_count;

						fread( &tile_id, sizeof(unsigned int), 1, hfile );
						fread( &position.x, sizeof(float), 1, hfile );
						fread( &position.y, sizeof(float), 1, hfile );
						fread( &one_way, sizeof(unsigned int), 1, hfile );
						fread( &hidden, sizeof(unsigned int), 1, hfile );
						fread( &points_count, sizeof(unsigned int), 1, hfile );

						Tile* tile = g_tileset->getDynamicByID(tile_id, (DYNAMIC_TYPE)type_id);
						if( tile )
						{
							GameTrigger* box = new GameTrigger( tile );
							box->id = name;
							box->setPosition(position);
							box->oneWay = one_way == 0 ? false : true;
							box->hidden = hidden == 0 ? false : true;

							char point_id[128];
							for( unsigned i=0; i<points_count; i++ )
							{
								memset(point_id, 0, sizeof(char)*128);
								fread(point_id, sizeof(char), 128, hfile);
								string s_point_id = point_id;
								box->tempObjIDs.push_back( s_point_id );
							}

							dynamics.push_back(box);
						}
						else
						{
							log_msg("No dynamic tile with ID %d", tile_id);
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case FLF_SECTION_PHYSIC_GEOMETRY:
			{
				unsigned int type_id;
				fread(&type_id, sizeof(unsigned int), 1, hfile);

				switch( type_id )
				{
				case PHYSIC_OBJ_SQUARE:
					{
						CollisionSquare* square = new CollisionSquare();

						fread(&square->bounds.x1, sizeof(float), 1, hfile);
						fread(&square->bounds.y1, sizeof(float), 1, hfile);
						fread(&square->bounds.x2, sizeof(float), 1, hfile);
						fread(&square->bounds.y2, sizeof(float), 1, hfile);

						phys_world_bounds.x1 = min( square->bounds.x1, phys_world_bounds.x1 );
						phys_world_bounds.y1 = min( square->bounds.y1, phys_world_bounds.y1 );
						phys_world_bounds.x2 = max( square->bounds.x2, phys_world_bounds.x2 );
						phys_world_bounds.y2 = max( square->bounds.y2, phys_world_bounds.y2 );

						collisions.push_back(square);
					}
					break;
				case PHYSIC_OBJ_TRIANGLE:
					{
						CollisionTriangle* triangle = new CollisionTriangle();

						fread(&triangle->vertices[0].x, sizeof(float), 1, hfile);
						fread(&triangle->vertices[0].y, sizeof(float), 1, hfile);
						fread(&triangle->vertices[1].x, sizeof(float), 1, hfile);
						fread(&triangle->vertices[1].y, sizeof(float), 1, hfile);
						fread(&triangle->vertices[2].x, sizeof(float), 1, hfile);
						fread(&triangle->vertices[2].y, sizeof(float), 1, hfile);

						phys_world_bounds.x1 = min( triangle->vertices[0].x, phys_world_bounds.x1 );
						phys_world_bounds.x1 = min( triangle->vertices[1].x, phys_world_bounds.x1 );
						phys_world_bounds.x1 = min( triangle->vertices[2].x, phys_world_bounds.x1 );
						phys_world_bounds.y1 = min( triangle->vertices[0].y, phys_world_bounds.y1 );
						phys_world_bounds.y1 = min( triangle->vertices[1].y, phys_world_bounds.y1 );
						phys_world_bounds.y1 = min( triangle->vertices[2].y, phys_world_bounds.y1 );
						phys_world_bounds.x2 = max( triangle->vertices[0].x, phys_world_bounds.x2 );
						phys_world_bounds.x2 = max( triangle->vertices[1].x, phys_world_bounds.x2 );
						phys_world_bounds.x2 = max( triangle->vertices[2].x, phys_world_bounds.x2 );
						phys_world_bounds.y2 = max( triangle->vertices[0].y, phys_world_bounds.y2 );
						phys_world_bounds.y2 = max( triangle->vertices[1].y, phys_world_bounds.y2 );
						phys_world_bounds.y2 = max( triangle->vertices[2].y, phys_world_bounds.y2 );

						collisions.push_back(triangle);
					}
					break;
				case PHYSIC_OBJ_CIRCLE:
					{
						CollisionCircle* circle = new CollisionCircle();

						fread(&circle->center.x, sizeof(float), 1, hfile);
						fread(&circle->center.y, sizeof(float), 1, hfile);
						fread(&circle->radius.x, sizeof(float), 1, hfile);
						fread(&circle->radius.y, sizeof(float), 1, hfile);

						float radiusLen = (circle->radius - circle->center).Length();
						phys_world_bounds.x1 = min( circle->center.x - radiusLen, phys_world_bounds.x1 );
						phys_world_bounds.y1 = min( circle->center.y - radiusLen, phys_world_bounds.y1 );
						phys_world_bounds.x2 = max( circle->center.x + radiusLen, phys_world_bounds.x2 );
						phys_world_bounds.y2 = max( circle->center.x + radiusLen, phys_world_bounds.y2 );

						circle->rebuildBorders();

						collisions.push_back(circle);
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	// relink id's
	list<GameDynamicObject*>::iterator it = dynamics.begin();
	while( it != dynamics.end() )
	{
		GameDynamicObject* obj = static_cast<GameDynamicObject*>(*it);
		if( obj )
		{
			if( obj->type == DYNAMIC_LIFT )
			{
				GameLift* lift = dynamic_cast<GameLift*>(obj);
				if( lift )
				{
					list<string>::iterator sit = lift->pointsTemp.begin();
					while( sit != lift->pointsTemp.end() )
					{
						if( points.find(*sit) != points.end() )
							lift->points.push_back(points[*sit]);
						else
							log_msg("Unable to find point with id %s", (*sit).c_str());

						sit++;
					}
				}
			}

			if( obj->type == DYNAMIC_TRIGGER )
			{
				GameTrigger* trig = dynamic_cast<GameTrigger*>(obj);
				if( trig )
				{
					list<string>::iterator sit = trig->tempObjIDs.begin();
					while( sit != trig->tempObjIDs.end() )
					{
						list<GameDynamicObject*>::iterator dit = dynamics.begin();
						while( dit != dynamics.end() )
						{
							GameDynamicObject* dyn = static_cast<GameDynamicObject*>(*dit);
							if( dyn->id == *sit )
							{
								trig->triggeredObjs.push_back(dyn);
								break;
							}

							dit++;
						}

						sit++;
					}
				}
			}
		}

		it++;
	}

	fclose(hfile);

	is_loaded = true;

	return 0;
}
//-----------------------------------------------------------------------------

bool GameLevel::saved() const
{
	return changesSaved;
}
//-----------------------------------------------------------------------------

void GameLevel::update( const float &deltaTime )
{
	// update statics
	list<GameStaticObject*>::iterator it = statics.begin();
	while( it != statics.end() )
	{
		(*it)->update( deltaTime );
		it++;
	}

	it = backgrounds.begin();
	while( it != backgrounds.end() )
	{
		(*it)->update( deltaTime );
		it++;
	}

	it = foregrounds.begin();
	while( it != foregrounds.end() )
	{
		(*it)->update( deltaTime );
		it++;
	}

	list<GameDynamicObject*>::iterator dit = dynamics.begin();
	while( dit != dynamics.end() )
	{
		(*dit)->update( deltaTime );

		if( (*dit)->isDead() )
		{
			GameDynamicObject* obj = *dit;
			if( obj )
				delete obj;
			dit = dynamics.erase(dit);
		}
		else
		{
			dit++;
		}
	}

	if( player )
		player->update(deltaTime);

	list<GameEffect*>::iterator eit = effects.begin();
	while( eit != effects.end() )
	{
		(*eit)->update(deltaTime);
		if( (*eit)->isStopped() )
		{
			GameEffect* eff = *eit;
			if( eff )
				delete eff;
			eit = effects.erase(eit);
		}
		else
		{
			eit++;
		}
	}
}
//-----------------------------------------------------------------------------

void GameLevel::render()
{
	list<GameStaticObject*>::iterator it = backgrounds.begin();
	while( it != backgrounds.end() )
	{
		(*it)->render(false);
		it++;
	}

	// render statics
	it = statics.begin();
	while( it != statics.end() )
	{
		(*it)->render(false);
		it++;
	}

	list<GameDynamicObject*>::iterator dit = dynamics.begin();
	while( dit != dynamics.end() )
	{
		(*dit)->render();
		dit++;
	}

	if( player )
		player->render();

	list<GameEffect*>::iterator eit = effects.begin();
	while( eit != effects.end() )
	{
		(*eit)->render();
		eit++;
	}

	it = foregrounds.begin();
	while( it != foregrounds.end() )
	{
		(*it)->render(false);
		it++;
	}
/*
	list<CollisionShape*>::iterator cit = collisions.begin();
	while( cit != collisions.end() )
	{
		(*cit)->render();
		cit++;
	}
*/
}
//-----------------------------------------------------------------------------
/*
GameStaticObject* GameLevel::select( const hgeVector& point )
{
	list<GameStaticObject*>::reverse_iterator it = statics.rbegin();
	while( it != statics.rend() )
	{
		hgeRect bounds = (*it)->bounds;
		bounds.x1 += (*it)->position.x;
		bounds.x2 += (*it)->position.x;
		bounds.y1 += (*it)->position.y;
		bounds.y2 += (*it)->position.y;
		
		if( bounds.TestPoint(point.x, point.y) )
			return (*it);

		it++;
	}
	return 0;
}
//-----------------------------------------------------------------------------
*/
void GameLevel::tile_add( GameStaticObject* obj )
{
	if( obj )
		statics.push_back(obj);
	log_msg("new static with ID %d", obj->tilesetSpriteID);
}
//-----------------------------------------------------------------------------

void GameLevel::tile_remove( GameStaticObject* obj )
{
	if( obj )
		statics.remove(obj);
}
//-----------------------------------------------------------------------------

void GameLevel::run()
{
	initBox2D();

	dummyBodyDef.position.Set(0.0f, 0.0f);
//	dummyBodyDef.massData.center.SetZero();
//	dummyBodyDef.massData.I = 0.0f;
//	dummyBodyDef.massData.mass = 0.0f;
	dummyBodyDef.fixedRotation = true;
	dummyBody = box2d->CreateBody(&dummyBodyDef);

	list<CollisionShape*>::iterator it = collisions.begin();
	while( it != collisions.end() )
	{
		(*it)->addToBox2D(box2d);
		it++;
	}

	list<GameDynamicObject*>::iterator dit = dynamics.begin();
	while( dit != dynamics.end() )
	{
		(*dit)->initInBox2d();
		dit++;
	}

	if( player )
		delete player;
	player = new GamePlayer();
	vector2f hs;
	player->sprite->GetHotSpot( &hs.x, &hs.y );
	player->setPosition( startPoint + hs );
	camera_position = player->position - vector2f( (float)(screen_width / 2), (float)(screen_height / 2) );

	stateManager->setState( GAME_STATE_LEVEL );
}
//-----------------------------------------------------------------------------

bool GameLevel::loaded() const
{
	return is_loaded;
}
//-----------------------------------------------------------------------------

void GameLevel::effect_add( GameEffect* effect )
{
	if( effect )
		effects.push_back(effect);
}
//-----------------------------------------------------------------------------
