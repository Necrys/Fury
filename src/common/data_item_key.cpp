#include "hgeanim.h"
#include "data_item_key.h"
#include "system_interfaces.h"
#include "sys_vars.h"
#include "str_helpers.h"
#include "data_level.h"
#include "system_main.h"

//-----------------------------------------------------------------------------

GameItemKey::GameItemKey( Tile* _tile )
{
	assert(_tile);
	tile = _tile;

	_switchable = true;

	texture = tile->anim->GetTexture();
	icon	= tile->anim->GetTexture();
	sprite	= new hgeAnimation( *tile->anim );
	scale	= tile->scale;

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;
	bounds = hgeRect( 0, 0, half_width * 2.0f , half_height * 2.0f);

#ifdef NOEDIT
//	sprite->SetHotSpot(half_width, half_height);
	body = 0;
#endif

	tilesetSpriteID = tile->id;

	group = 0;

	if( tile->params.find( "group" ) != tile->params.end() )
		group = str2int( tile->params["group"] );

	id = "new_key";
}
//-----------------------------------------------------------------------------

GameItemKey::~GameItemKey()
{
#ifdef NOEDIT
	if( body )
		box2d->DestroyBody(body);
#endif
}
//-----------------------------------------------------------------------------

GameDynamicObject* GameItemKey::clone()
{
	GameItemKey* key = new GameItemKey(tile);
	key->setPosition(position);

	key->group = group;
	return static_cast<GameDynamicObject*>(key);
}
//-----------------------------------------------------------------------------

void GameItemKey::serialize( FILE* stream )
{
	assert(stream);

	struct item_key
	{
		unsigned int	type;
		unsigned int	obj_type;
		unsigned int	tile_id;
		float			position_x;
		float			position_y;
		unsigned int	group;
	} obj;

	obj.type = FLF_SECTION_DYNAMIC;
	obj.obj_type = DYNAMIC_KEY;
	obj.tile_id = tilesetSpriteID;
	obj.position_x = position.x;
	obj.position_y = position.y;
	obj.group = group;

	fwrite( &obj, sizeof(item_key), 1, stream );
}
//-----------------------------------------------------------------------------

void GameItemKey::setPosition( const vector2f& pos )
{
	position = pos;
#ifdef NOEDIT
	if( body )
		body->SetTransform( b2Vec2(pos.x / phys_norma, pos.y / phys_norma), rotation );
#endif
}
//-----------------------------------------------------------------------------

#ifdef NOEDIT
void GameItemKey::initInBox2d()
{
	b2BodyDef		bodyDef;
	b2CircleShape	shape;
	b2FixtureDef	fix;
	b2Fixture*		f;

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;

	bodyDef.position.Set( (position.x + half_width) / phys_norma, (position.y + half_height) / phys_norma);
	bodyDef.linearDamping = 0.0f;
	bodyDef.type = b2_staticBody;
	body = box2d->CreateBody(&bodyDef);

	shape.m_radius = 16.0f / phys_norma;

	fix.isSensor = true;
	//	shapeDef.density = 0.0f;
	fix.filter.categoryBits = OBJCAT_DYNAMIC;
	fix.filter.maskBits = OBJCAT_PLAYER;
	fix.shape = &shape;

	f = body->CreateFixture(&fix);
	//	body->SetMassFromShapes();
	f->SetUserData(&data);

	data.objectPtr	= (void*)this;
	data.isDynamic	= true;
	data.type		= OBJECT_KEY;
	data.isSensor	= true;
}
//-----------------------------------------------------------------------------

void GameItemKey::onCollision()
{
	assert( level );
	assert( level->player );

	level->player->keys[group] = true;
	dead = true;
}
//-----------------------------------------------------------------------------

#endif
//-----------------------------------------------------------------------------
