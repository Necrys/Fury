#include "hgeanim.h"
#include "data_level.h"
#include "data_dynamic_destructable.h"
#include "sys_vars.h"
#include "system_interfaces.h"
#include "str_helpers.h"

//-----------------------------------------------------------------------------
GameDestructable::GameDestructable( Tile* _tile )
{
	assert( _tile );
	tile = _tile;

	texture = tile->anim->GetTexture();
	icon	= tile->anim->GetTexture();
	sprite	= new hgeAnimation( *tile->anim );
	scale	= tile->scale;

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;
	bounds = hgeRect( 0, 0, half_width * 2.0f , half_height * 2.0f);

#ifdef NOEDIT
	sprite->SetHotSpot(half_width, half_height);
	body = 0;
#endif

	tilesetSpriteID = tile->id;

	friction = dynamicFriction;
	restitution = dynamicRestitution;
	health = 6.0f;

	if( tile->params.find( "friction" ) != tile->params.end() )
		friction = str2float( tile->params["friction"] );
	if( tile->params.find( "restitution" ) != tile->params.end() )
		restitution = str2float( tile->params["restitution"] );
	if( tile->params.find( "health" ) != tile->params.end() )
		health = str2float( tile->params["health"] );

	type = DYNAMIC_DESTRUCTABLE;
}
//-----------------------------------------------------------------------------

GameDestructable::~GameDestructable()
{
#ifdef NOEDIT
	if( body )
		box2d->DestroyBody(body);
#endif
}
//-----------------------------------------------------------------------------

GameDynamicObject* GameDestructable::clone()
{
	GameDestructable* new_box = new GameDestructable( tile );
	new_box->setPosition( position );

	new_box->friction = friction;
	new_box->restitution = restitution;

	return static_cast<GameDynamicObject*>(new_box);
}
//-----------------------------------------------------------------------------

void GameDestructable::serialize( FILE* stream )
{
	assert( stream );

	struct dynamic_destructable
	{
		unsigned int	type;
		unsigned int	obj_type;
		unsigned int	tile_id;
		float			position_x;
		float			position_y;
		float			friction;
		float			restitution;
		float			health;
	} obj;

	obj.type = FLF_SECTION_DYNAMIC;
	obj.obj_type = DYNAMIC_DESTRUCTABLE;
	obj.tile_id = tilesetSpriteID;
	obj.position_x = position.x;
	obj.position_y = position.y;
	obj.health = health;
	obj.friction = friction;
	obj.restitution = restitution;

	fwrite( &obj, sizeof(dynamic_destructable), 1, stream );
}
//-----------------------------------------------------------------------------

void GameDestructable::setPosition( const vector2f& pos )
{
	position = pos;
#ifdef NOEDIT
	if( body )
		body->SetTransform( b2Vec2(pos.x / phys_norma, pos.y / phys_norma), rotation );
#endif
}
//-----------------------------------------------------------------------------
#ifdef NOEDIT

void GameDestructable::initInBox2d()
{
	b2BodyDef		bodyDef;
	b2PolygonShape	shape;
	b2FixtureDef	fix;
	b2Fixture*		f;

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;

	bodyDef.position.Set( (position.x + half_width) / phys_norma, (position.y + half_height) / phys_norma);
	bodyDef.linearDamping = 0.0f;
	bodyDef.type = b2_staticBody;
	body = box2d->CreateBody(&bodyDef);

	shape.SetAsBox( half_width / phys_norma, half_height / phys_norma );

	fix.density = 0.0f;
	fix.restitution = restitution;
	fix.friction = friction;
	fix.filter.categoryBits = OBJCAT_DYNAMIC;
	fix.filter.maskBits = OBJCAT_DYNAMIC | OBJCAT_PLAYER | OBJCAT_FIREBALL | OBJCAT_STATIC;
	fix.shape = &shape;

	f = body->CreateFixture(&fix);
	f->SetUserData(&data);

	b2Vec2 pos = body->GetPosition();
	position.x = pos.x * phys_norma;
	position.y = pos.y * phys_norma;

	data.objectPtr	= (void*)this;
	data.isDynamic	= true;
	data.type		= OBJECT_DESTRUCTABLE;
}
//-----------------------------------------------------------------------------

void GameDestructable::update( const float& delta )
{
	if( health <= 0.0f )
		dead = true;

	if( hitFadeTimer > 0.0f )
		hitFadeTimer -= delta;

	GameDynamicObject::update(delta);
}
//-----------------------------------------------------------------------------

void GameDestructable::hit( const float& power )
{
	health -= power;
	hitFadeTimer = 0.1f;
	log_msg( "Hit power: %f", power );
}
//-----------------------------------------------------------------------------

void GameDestructable::render( const bool& shadowed )
{
	GameDynamicObject::render(shadowed);

	if( hitFadeTimer > 0.0f )
	{
		int blend = sprite->GetBlendMode();
		sprite->SetBlendMode( BLEND_COLORADD | BLEND_ALPHAADD | BLEND_ZWRITE );

		float alpha = (hitFadeTimer/0.1f) * 255;
		uint32 dwAlpha = (((uint32)alpha))<<24;
		uint32 color = 0x00FFFFFF + dwAlpha;
		sprite->SetColor(color);
		
		sprite->RenderEx(position.x, position.y, rotation, scale, scale);

		sprite->SetBlendMode(blend);
	}
}

#endif

//-----------------------------------------------------------------------------
