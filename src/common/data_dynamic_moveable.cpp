
#include <hgeanim.h>
#include "data_dynamic_moveable.h"
#include "sys_vars.h"
#include "str_helpers.h"
#include "data_level.h"
#include "system_interfaces.h"

//-----------------------------------------------------------------------------
GameMovableBox::GameMovableBox( Tile* _tile )
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

	density = dynamicDensity;
	friction = dynamicFriction;
	restitution = dynamicRestitution;

	if( tile->params.find( "density" ) != tile->params.end() )
		density = str2float( tile->params["density"] );
	if( tile->params.find( "friction" ) != tile->params.end() )
		friction = str2float( tile->params["friction"] );
	if( tile->params.find( "restitution" ) != tile->params.end() )
		restitution = str2float( tile->params["restitution"] );

	type = DYNAMIC_MOVEABLE_BOX;
}
//-----------------------------------------------------------------------------

GameMovableBox::~GameMovableBox()
{

}
//-----------------------------------------------------------------------------

GameDynamicObject* GameMovableBox::clone()
{
	GameMovableBox* new_box = new GameMovableBox( tile );
	new_box->setPosition( position );

	new_box->density = density;
	new_box->friction = friction;
	new_box->restitution = restitution;

	return static_cast<GameDynamicObject*>(new_box);
}
//-----------------------------------------------------------------------------

void GameMovableBox::serialize( FILE* stream )
{
	assert( stream );

	struct dynamic_movable
	{
		unsigned int	type;
		unsigned int	obj_type;
		unsigned int	tile_id;
		float			position_x;
		float			position_y;
		float			density;
		float			friction;
		float			restitution;
	} obj;

	obj.type = FLF_SECTION_DYNAMIC;
	obj.obj_type = DYNAMIC_MOVEABLE_BOX;
	obj.tile_id = tilesetSpriteID;
	obj.position_x = position.x;
	obj.position_y = position.y;
	obj.density = density;
	obj.friction = friction;
	obj.restitution = restitution;

	fwrite( &obj, sizeof(dynamic_movable), 1, stream );
}
//-----------------------------------------------------------------------------

void GameMovableBox::setPosition( const vector2f& pos )
{
	position = pos;
#ifdef NOEDIT
	if( body )
		body->SetTransform( b2Vec2(pos.x / phys_norma, pos.y / phys_norma), rotation );
#endif
}
//-----------------------------------------------------------------------------
#ifdef NOEDIT
void GameMovableBox::update( const float& delta )
{
	if(body)
	{
		b2Vec2 pos = body->GetPosition();
		rotation = body->GetAngle();
		position.x = pos.x * phys_norma;
		position.y = pos.y * phys_norma;
	}
}
//-----------------------------------------------------------------------------

void GameMovableBox::initInBox2d()
{
	b2BodyDef		bodyDef;
	b2PolygonShape	shape;
	b2FixtureDef	fix;
	b2Fixture*		f;

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;

	bodyDef.position.Set( (position.x + half_width) / phys_norma, (position.y + half_height) / phys_norma);
	bodyDef.linearDamping = 0.0f;
//	bodyDef.angularDamping = 50000.0f;
//	bodyDef.fixedRotation = true;
	bodyDef.bullet = true;
	bodyDef.type = b2_dynamicBody;
	body = box2d->CreateBody(&bodyDef);

	shape.SetAsBox( half_width / phys_norma, half_height / phys_norma );

	fix.density = density;
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
	data.type		= OBJECT_MOVABLE;
}
#endif
//-----------------------------------------------------------------------------

