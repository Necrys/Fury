#include "hgefont.h"
#include "data_level.h"
#include "data_dynamic_crunch.h"
#include "sys_vars.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "system_main.h"
#include "helper_functions.h"

//-----------------------------------------------------------------------------
#ifndef NOEDIT
//dlg_crunch_options* GameCrunch::propsWindow = 0;
#endif

//-----------------------------------------------------------------------------
GameCrunch::GameCrunch( Tile* _tile )
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
	health = 4;

	if( tile->params.find( "friction" ) != tile->params.end() )
		friction = str2float( tile->params["friction"] );
	if( tile->params.find( "restitution" ) != tile->params.end() )
		restitution = str2float( tile->params["restitution"] );
	if( tile->params.find( "health" ) != tile->params.end() )
		health = str2int( tile->params["health"] );

	type = DYNAMIC_CRUNCH;

	HTEXTURE mask = textureManager->getTexture( Path::gfxtiles + "default\\masking.tga" );
	masks[2] = new hgeAnimation( mask, 1, 1, 0, 192, 48, 48);
	masks[2]->SetHotSpot(24, 24);
	masks[1] = new hgeAnimation( mask, 1, 1, 48, 192, 48, 48);
	masks[1]->SetHotSpot(24, 24);
	masks[0] = new hgeAnimation( mask, 1, 1, 96, 192, 48, 48);
	masks[0]->SetHotSpot(24, 24);

#ifndef NOEDIT
//	if( !propsWindow )
//	{
//		propsWindow = new dlg_crunch_options(gui_system);
//		propsWindow->bounds = hgeRect(150.0f, 100.0f, 550.0f, 550.0f );
//		propsWindow->caption = "Options";
//		propsWindow->set_visibility(false);
//	}
#endif
}
//-----------------------------------------------------------------------------

GameCrunch::~GameCrunch()
{
#ifdef NOEDIT
	if( body )
		box2d->DestroyBody(body);

	// ÊÎÑÒÛËÜ!!!
	if( level && level->player )
	{
		if( level->player->crunchContacts[0] == this )
			level->player->crunchContacts[0] = 0;
		if( level->player->crunchContacts[1] == this )
			level->player->crunchContacts[1] = 0;		
		if( level->player->crunchContacts[2] == this )
			level->player->crunchContacts[2] = 0;
	}
#endif
}
//-----------------------------------------------------------------------------

GameDynamicObject* GameCrunch::clone()
{
	GameCrunch* new_box = new GameCrunch( tile );
	new_box->setPosition( position );

	new_box->friction = friction;
	new_box->restitution = restitution;

	return static_cast<GameDynamicObject*>(new_box);
}
//-----------------------------------------------------------------------------

void GameCrunch::serialize( FILE* stream )
{
	assert( stream );

	struct dynamic_crunch
	{
		unsigned int	type;
		unsigned int	obj_type;
		unsigned int	tile_id;
		float			position_x;
		float			position_y;
		float			friction;
		float			restitution;
		unsigned		health;
	} obj;

	obj.type = FLF_SECTION_DYNAMIC;
	obj.obj_type = DYNAMIC_CRUNCH;
	obj.tile_id = tilesetSpriteID;
	obj.position_x = position.x;
	obj.position_y = position.y;
	obj.health = health;
	obj.friction = friction;
	obj.restitution = restitution;

	fwrite( &obj, sizeof(dynamic_crunch), 1, stream );
}
//-----------------------------------------------------------------------------

void GameCrunch::setPosition( const vector2f& pos )
{
	position = pos;
#ifdef NOEDIT
	if( body )
		body->SetTransform( b2Vec2(pos.x / phys_norma, pos.y / phys_norma), rotation );
#endif
}
//-----------------------------------------------------------------------------
#ifdef NOEDIT

void GameCrunch::initInBox2d()
{
	b2BodyDef		bodyDef;
	b2PolygonShape	shape;
	b2FixtureDef	fix;
	b2Fixture*		f;

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;

	bodyDef.position.Set( (position.x + half_width) / phys_norma, (position.y + half_height) / phys_norma);
	bodyDef.type = b2_staticBody;
	bodyDef.linearDamping = 0.0f;
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
	data.type		= OBJECT_CRUNCH;
}
//-----------------------------------------------------------------------------

void GameCrunch::update( const float& delta )
{
	if( health == 0 )
		dead = true;

	GameDynamicObject::update(delta);
}
//-----------------------------------------------------------------------------

void GameCrunch::hit()
{
	if( health > 0 )
		health--;
}
//-----------------------------------------------------------------------------

void GameCrunch::render( const bool& shadowed )
{
	if( health > 0 )
		GameDynamicObject::render(shadowed);

	if( health < 4 && health > 0 )
		render_anim(masks[health-1], (int32)position.x, (int32)position.y, rotation);
}
//-----------------------------------------------------------------------------

#else

void GameCrunch::showPropsWindow()
{
//	propsWindow->set_visibility(true);
//	propsWindow->user_data = this;
//	propsWindow->edb_health->text = int2str(health);

//	gui_system->show_modal_window( propsWindow );
	crunch_dlg_show(this);
}
//-----------------------------------------------------------------------------

#endif
//-----------------------------------------------------------------------------
