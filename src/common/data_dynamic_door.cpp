#include "hgefont.h"
#include "hgeanim.h"
#include "data_dynamic_door.h"
#include "sys_vars.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "data_level.h"
#include "system_main.h"

//-----------------------------------------------------------------------------
#ifndef NOEDIT
//DoorPropsWindow* GameDoor::propsWindow = 0;
#include "dialog_door.h"
#endif
//-----------------------------------------------------------------------------

GameDoor::GameDoor( Tile* _tile )
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
	sprite->SetHotSpot(half_width, half_height);
	body = 0;
	opened = false;
#endif

	tilesetSpriteID = tile->id;

	density = dynamicDensity;
	friction = dynamicFriction;
	restitution = dynamicRestitution;
	group = 0;
	direction = DIR_UP;

	if( tile->params.find( "density" ) != tile->params.end() )
		density = str2float( tile->params["density"] );
	if( tile->params.find( "friction" ) != tile->params.end() )
		friction = str2float( tile->params["friction"] );
	if( tile->params.find( "restitution" ) != tile->params.end() )
		restitution = str2float( tile->params["restitution"] );
	if( tile->params.find( "group" ) != tile->params.end() )
		group = str2int( tile->params["group"] );

	id = "new_door";

#ifndef NOEDIT
/*	if( !propsWindow )
	{
		propsWindow = new DoorPropsWindow(gui_system);
		propsWindow->bounds = hgeRect(150.0f, 100.0f, 550.0f, 550.0f );
		propsWindow->caption = "Свойства двери";
		propsWindow->set_visibility(false);
	}*/
#endif
}
//-----------------------------------------------------------------------------

GameDynamicObject* GameDoor::clone()
{
	GameDoor* door = new GameDoor( tile );

	door->setPosition( position );
	door->id = id;
	door->density = density;
	door->friction = friction;
	door->restitution = restitution;
	door->group = group;
	door->direction = direction;

	return static_cast<GameDynamicObject*>(door);
}
//-----------------------------------------------------------------------------

void GameDoor::serialize( FILE* stream )
{
	assert(stream);

	struct
	{
		unsigned int	type;
		unsigned int	obj_type;
		unsigned int	tile_id;
		float			position_x;
		float			position_y;
		float			density;
		float			friction;
		float			restitution;
		unsigned int	group;
		unsigned int	direction;
		char			id[128];
	} door_struct;

	door_struct.type		= FLF_SECTION_DYNAMIC;
	door_struct.obj_type	= DYNAMIC_DOOR;
	door_struct.tile_id		= tilesetSpriteID;
	door_struct.position_x	= position.x;
	door_struct.position_y	= position.y;
	door_struct.density		= density;
	door_struct.friction	= friction;
	door_struct.restitution = restitution;
	door_struct.group		= group;
	door_struct.direction	= (unsigned int)direction;
	memset(door_struct.id, 0, sizeof(char)*128);
	strcpy(door_struct.id, id.c_str());

	fwrite( &door_struct, sizeof(door_struct), 1, stream );
}
//-----------------------------------------------------------------------------

void GameDoor::setPosition( const vector2f& pos )
{
	position = pos;
#ifdef NOEDIT
	if( body )
		body->SetTransform( b2Vec2(pos.x / phys_norma, pos.y / phys_norma), rotation );
#endif
}
//-----------------------------------------------------------------------------

#ifndef NOEDIT
/*
DoorPropsWindow::DoorPropsWindow(gui::system* _controller, gui::_base* parent) : window(_controller, parent)
{
	id_label = new gui::label(_controller, this);
	id_label->text = "id";
	id_label->bounds = hgeRect(20.0f, 20.0f, 60.0f, 40.0f);
	id_label->align = HGETEXT_RIGHT;

	id_edit = new gui::edit_box(_controller, this);
	id_edit->bounds = hgeRect(80.0f, 20.0f, 280.0f, 40.0f);

	btn_ok = new DoorPropsWindowOKBtn(_controller, this);
	btn_ok->bounds = hgeRect( 141.0f, 400.0f, 191.0f, 430.0f );
	btn_ok->setImage( Path::uigfx + "icons\\ok.png" );
	btn_ok->caption = "OK";
	btn_ok->align = HGETEXT_TOP | HGETEXT_CENTER;

	btn_cancel = new DoorPropsWindowCancelBtn(_controller, this);
	btn_cancel->bounds = hgeRect( 196.0f, 400.0f, 246.0f, 430.0f );
	btn_cancel->setImage( Path::uigfx + "icons\\cancel.png" );
	btn_cancel->caption = "Отмена";
	btn_cancel->align = HGETEXT_TOP | HGETEXT_CENTER;

	ddl_direction = new gui::drop_list(_controller, this);
	ddl_direction->bounds = hgeRect(80.0f, 60.0f, 280.0f, 80.0f);
}*/
//-----------------------------------------------------------------------------

void GameDoor::showPropsWindow()
{
	/*
	propsWindow->set_visibility(true);
	propsWindow->caller = this;
	propsWindow->id_edit->text = id;

	propsWindow->ddl_direction->clear();
	propsWindow->ddl_direction->insertElement("up");
	propsWindow->ddl_direction->insertElement("down");
	propsWindow->ddl_direction->insertElement("left");
	propsWindow->ddl_direction->insertElement("right");
	propsWindow->ddl_direction->setSelection( (unsigned int)direction );

	gui_system->show_modal_window( propsWindow );
	*/
	door_dlg_show(this);
}
//-----------------------------------------------------------------------------
#else

void GameDoor::initInBox2d()
{
	b2PrismaticJointDef jointDef;
	b2BodyDef			bodyDef;
	b2PolygonShape		shape;
	b2FixtureDef		fix;
	b2Fixture*			f;

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;

	bodyDef.position.Set( (position.x + half_width) / phys_norma, (position.y + half_height) / phys_norma);
	bodyDef.linearDamping = 0.0f;
	bodyDef.type = b2_dynamicBody;
	body = box2d->CreateBody(&bodyDef);

	shape.SetAsBox( half_width / phys_norma, half_height / phys_norma );

	fix.density = density * 100.0f;
	fix.restitution = restitution;
	fix.friction = friction;
	fix.filter.categoryBits = OBJCAT_DYNAMIC;
	fix.filter.maskBits = OBJCAT_DYNAMIC | OBJCAT_PLAYER | OBJCAT_FIREBALL;
	fix.shape = &shape;

	f = body->CreateFixture(&fix);
	f->SetUserData(&data);

	b2Vec2 pos = body->GetPosition();
	position.x = pos.x * phys_norma;
	position.y = pos.y * phys_norma;

	vector2f moveVec;
	if( direction == DIR_DOWN )
		moveVec = vector2f(0.0f, 1.0f);
	else if( direction == DIR_UP )
		moveVec = vector2f(0.0f, -1.0f);
	else if( direction == DIR_LEFT )
		moveVec = vector2f(-1.0f, 0.0f);
	else if( direction == DIR_RIGHT )
		moveVec = vector2f(1.0f, 0.0f);

	moveVec.Normalize();

	jointDef.Initialize(level->dummyBody, body, bodyDef.position, b2Vec2(moveVec.x, moveVec.y));
	jointDef.lowerTranslation = 0;
	jointDef.upperTranslation = 48.0f / phys_norma;
	jointDef.enableLimit = true;		
	jointDef.enableMotor = true;
	jointDef.maxMotorForce = 10000000.f;

	joint = (b2PrismaticJoint*)box2d->CreateJoint(&jointDef);

	data.objectPtr	= (void*)this;
	data.isDynamic	= true;
	data.type		= OBJECT_DOOR;
}
//-----------------------------------------------------------------------------

void GameDoor::update( const float& delta )
{
	assert(body);

	if( joint->IsMotorEnabled() )
	{
		if( joint->GetJointTranslation() >= joint->GetUpperLimit() && !opened )
		{
			joint->SetMotorSpeed( 0.0f );
			opened = true;
		}
		if( joint->GetJointTranslation() <= joint->GetLowerLimit() && opened )
		{
			joint->SetMotorSpeed( 0.0f );
			opened = false;
		}
	}

	b2Vec2 pos = body->GetPosition();
	rotation = body->GetAngle();
	position.x = pos.x * phys_norma;
	position.y = pos.y * phys_norma;

	GameDynamicObject::update(delta);
}
//-----------------------------------------------------------------------------

void GameDoor::toggle()
{
	if( !opened )
	{
//		joint->EnableMotor(true);
		joint->SetMotorSpeed( 0.5f );
	}
	else
	{
//		joint->EnableMotor(true);
		joint->SetMotorSpeed( -0.5f );
	}
}
//-----------------------------------------------------------------------------
#endif