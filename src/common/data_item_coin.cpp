#include "hgeanim.h"
#include "data_item_coin.h"
#include "system_interfaces.h"
#include "sys_vars.h"
#include "str_helpers.h"
#include "data_level.h"
#include "system_main.h"

//-----------------------------------------------------------------------------

GameItemCoin::GameItemCoin( Tile* _tile )
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

#ifdef NOEDIT
	//	sprite->SetHotSpot(half_width, half_height);
	body = 0;
#endif

	tilesetSpriteID = tile->id;

	value = 1;
	if( tile->params.find( "value" ) != tile->params.end() )
		value = str2int( tile->params["value"] );

	id = "new_coin";

	type = DYNAMIC_COIN;
}
//-----------------------------------------------------------------------------

GameItemCoin::~GameItemCoin()
{
#ifdef NOEDIT
	if( body )
		box2d->DestroyBody(body);
#endif
}
//-----------------------------------------------------------------------------

GameDynamicObject* GameItemCoin::clone()
{
	GameItemCoin* key = new GameItemCoin(tile);
	key->setPosition(position);

	key->value = value;

	return static_cast<GameDynamicObject*>(key);
}
//-----------------------------------------------------------------------------

void GameItemCoin::serialize( FILE* stream )
{
	assert(stream);

	struct item_coin
	{
		unsigned int	type;
		unsigned int	obj_type;
		unsigned int	tile_id;
		float			position_x;
		float			position_y;
		unsigned int	value;
	} obj;

	obj.type = FLF_SECTION_DYNAMIC;
	obj.obj_type = DYNAMIC_COIN;
	obj.tile_id = tilesetSpriteID;
	obj.position_x = position.x;
	obj.position_y = position.y;
	obj.value = value;

	fwrite( &obj, sizeof(item_coin), 1, stream );
}
//-----------------------------------------------------------------------------

void GameItemCoin::setPosition( const vector2f& pos )
{
	position = pos;
#ifdef NOEDIT
	if( body )
		body->SetTransform( b2Vec2(pos.x / phys_norma, pos.y / phys_norma), rotation );
#endif
}
//-----------------------------------------------------------------------------

#ifdef NOEDIT
void GameItemCoin::initInBox2d()
{
	b2BodyDef		bodyDef;
	b2CircleShape	shape;
	b2FixtureDef	fix;
	b2Fixture*		f;

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;

	bodyDef.position.Set( (position.x + half_width) / phys_norma, (position.y + half_height) / phys_norma);
	bodyDef.type = b2_staticBody;
//	bodyDef.linearDamping = 0.0f;
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
	data.type		= OBJECT_COIN;
	data.isSensor	= true;
}
//-----------------------------------------------------------------------------

void GameItemCoin::onCollision()
{
	assert( level );
	assert( level->player );

	level->player->coins++;
	dead = true;
}
//-----------------------------------------------------------------------------

#endif
//-----------------------------------------------------------------------------
