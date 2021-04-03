#include "hgefont.h"
#include "data_dynamic_lift.h"
#include "data_tileset.h"
#include "data_level.h"
#include "data_dynamic_point.h"
#include "sys_vars.h"
#include "system_interfaces.h"
#include "system_paths.h"

using namespace std;

//-----------------------------------------------------------------------------
#ifndef NOEDIT
//LiftPropsWindow* GameLift::propsWindow = 0;
#include "dialog_lift.h"
#endif
//-----------------------------------------------------------------------------

GameLift::GameLift( Tile* _tile )
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
	sprite->SetHotSpot(half_width, half_height);
	body = 0;
#endif

	tilesetSpriteID = tile->id;

	density = dynamicDensity;
	friction = dynamicFriction;
	restitution = dynamicRestitution;
	speed = 1.0f;
	moveOrder = MOVE_LOOP;
	enabled = true;
	_switchable = true;
	id = "new_lift";

	if( tile->params.find( "density" ) != tile->params.end() )
		density = str2float( tile->params["density"] );
	if( tile->params.find( "friction" ) != tile->params.end() )
		friction = str2float( tile->params["friction"] );
	if( tile->params.find( "restitution" ) != tile->params.end() )
		restitution = str2float( tile->params["restitution"] );
	if( tile->params.find( "speed" ) != tile->params.end() )
		speed = str2float( tile->params["speed"] );

	type = DYNAMIC_LIFT;

#ifndef NOEDIT
/*	if( !propsWindow )
	{
		propsWindow = new LiftPropsWindow(gui_system);
		propsWindow->bounds = hgeRect(150.0f, 100.0f, 550.0f, 550.0f );
		propsWindow->caption = "Свойства лифта";
		propsWindow->set_visibility(false);
	}*/
#else
	tempSpeed = speed;
#endif
}
//-----------------------------------------------------------------------------

GameDynamicObject* GameLift::clone()
{
	GameLift* lift = new GameLift( tile );

	lift->setPosition( position );
	lift->id = id;
	lift->density = density;
	lift->friction = friction;
	lift->restitution = restitution;
	lift->moveOrder = moveOrder;
	lift->speed = speed;

	return static_cast<GameDynamicObject*>(lift);
}
//-----------------------------------------------------------------------------

void GameLift::serialize( FILE* stream )
{
	assert(stream);

	struct dynamic_lift
	{
		unsigned int	type;
		unsigned int	obj_type;
		unsigned int	tile_id;
		float			position_x;
		float			position_y;
		float			density;
		float			friction;
		float			restitution;
		float			speed;
		unsigned int	points_count;
		unsigned int 	move_order;
		unsigned int	enabled;
		char			id[128];
	} obj;

	obj.type = FLF_SECTION_DYNAMIC;
	obj.obj_type = DYNAMIC_LIFT;
	obj.tile_id = tilesetSpriteID;
	obj.position_x = position.x;
	obj.position_y = position.y;
	obj.density = density;
	obj.friction = friction;
	obj.restitution = restitution;
	obj.points_count = (unsigned int)points.size();
	obj.move_order = (unsigned char)moveOrder;
	obj.speed = speed;
	memset(obj.id, 0, sizeof(char)*128);
	strcpy(obj.id, id.c_str());
	obj.enabled = enabled ? 1 : 0;

	fwrite( &obj, sizeof(dynamic_lift), 1, stream );

	list<GamePoint*>::iterator it = points.begin();
	while( it != points.end() )
	{
		char id[128];
		memset(id, 0, sizeof(char) * 128);
		
		strcpy_s(id, 128, (*it)->id.c_str());
		fwrite( id, sizeof(char), 128, stream );

		it++;
	}
}
//-----------------------------------------------------------------------------

void GameLift::setPosition( const vector2f& pos )
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
LiftPropsWindow::LiftPropsWindow(gui::system* _controller, gui::_base* parent) : window(_controller, parent)
{
	id_label = new gui::label(_controller, this);
	id_label->text = "id";
	id_label->bounds = hgeRect(20.0f, 20.0f, 60.0f, 40.0f);
	id_label->align = HGETEXT_RIGHT;

	id_edit = new gui::edit_box(_controller, this);
	id_edit->bounds = hgeRect(80.0f, 20.0f, 280.0f, 40.0f);

	all_points_list = new gui::image_list(_controller, this);
	all_points_list->bounds = hgeRect(20.0f, 60.0f, 180.0f, 380.0f);

	lift_points_list = new gui::image_list(_controller, this);
	lift_points_list->bounds = hgeRect(220.0f, 60.0f, 380.0f, 380.0f);

	ok_btn = new LiftPropsWindowOKBtn(_controller, this);
	ok_btn->bounds = hgeRect( 141.0f, 400.0f, 191.0f, 430.0f );
	ok_btn->setImage( Path::uigfx + "icons\\ok.png" );
	ok_btn->caption = "OK";
	ok_btn->align = HGETEXT_TOP | HGETEXT_CENTER;

	cancel_btn = new LiftPropsWindowCancelBtn(_controller, this);
	cancel_btn->bounds = hgeRect( 196.0f, 400.0f, 246.0f, 430.0f );
	cancel_btn->setImage( Path::uigfx + "icons\\cancel.png" );
	cancel_btn->caption = "Отмена";
	cancel_btn->align = HGETEXT_TOP | HGETEXT_CENTER;

	add_point_btn = new AddPointBtn(_controller, this);
	add_point_btn->bounds = hgeRect( 185.0f, 100.0f, 215.0f, 130.0f );
	add_point_btn->setImage( Path::uigfx + "icons\\right.png" );
//	add_point_btn->caption = "Add";
	add_point_btn->align = HGETEXT_TOP | HGETEXT_CENTER;

	remove_point_btn = new RemovePointBtn(_controller, this);
	remove_point_btn->bounds = hgeRect( 185.0f, 140.0f, 215.0f, 170.0f );
	remove_point_btn->setImage( Path::uigfx + "icons\\left.png" );
//	remove_point_btn->caption = "Remove";
	remove_point_btn->align = HGETEXT_TOP | HGETEXT_CENTER;

	lbl_enabled = new gui::label(_controller, this);
	lbl_enabled->set_position_size(20, 385, 45, 20);
	lbl_enabled->text = "Enabled";

	check_enabled = new gui::checkbox(_controller, this);
	check_enabled->set_position_size( 75, 390, 12, 12 );
}
*/
//-----------------------------------------------------------------------------

void GameLift::showPropsWindow()
{
/*
	propsWindow->set_visibility(true);
	propsWindow->caller_point = this;
	propsWindow->id_edit->text = id;

	propsWindow->check_enabled->checked = enabled;

	propsWindow->all_points_list->clear();
	propsWindow->lift_points_list->clear();

	std::map<std::string, GamePoint*>::iterator pit = level->points.begin();
	while( pit != level->points.end() )
	{
		std::list<GamePoint*>::iterator pit2 = points.begin();
		bool found = false;
		while( pit2 != points.end() )
		{
			if( (*pit2)->id == pit->second->id )
			{
				found = true;
				break;
			}
				
			pit2++;
		}
		if( found )
		{
			pit++;
			continue;
		}

		gui::image_list_item* element = propsWindow->all_points_list->insertElement("", pit->second->id);
		element->user_data = (void*)pit->second;

		pit++;
	}

	std::list<GamePoint*>::iterator pit2 = points.begin();
	while( pit2 != points.end() )
	{
		if( level->points.find((*pit2)->id) != level->points.end() )
		{
			gui::image_list_item* element = propsWindow->lift_points_list->insertElement("", (*pit2)->id);
			element->user_data = (void*)(*pit2);
		}

		pit2++;
	}

	gui_system->show_modal_window( propsWindow );
*/
	lift_dlg_show(this);
}
#endif
//-----------------------------------------------------------------------------

#ifdef NOEDIT

void GameLift::initInBox2d()
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
	fix.filter.maskBits = OBJCAT_DYNAMIC | OBJCAT_PLAYER | OBJCAT_FIREBALL | OBJCAT_STATIC;
	fix	.shape = &shape;

	f = body->CreateFixture(&fix);
	f->SetUserData(&data);

	b2Vec2 pos = body->GetPosition();
	position.x = pos.x * phys_norma;
	position.y = pos.y * phys_norma;

	if( points.size() < 2 )
		return;

	std::list<GamePoint*>::iterator point1 = points.begin();
	std::list<GamePoint*>::iterator point2 = points.begin();
	point2++;

	vector2f moveVec = (*point2)->position - (*point1)->position;
	float len = moveVec.Length() / phys_norma;
	moveVec.Normalize();

	jointDef.Initialize(level->dummyBody, body, bodyDef.position, b2Vec2(moveVec.x, moveVec.y));
	jointDef.lowerTranslation = 0;
	jointDef.upperTranslation = len;
	jointDef.enableLimit = true;		
	jointDef.enableMotor = true;
	jointDef.maxMotorForce = 10000000.f;

	joint = (b2PrismaticJoint*)box2d->CreateJoint(&jointDef);

	data.objectPtr	= (void*)this;
	data.isDynamic	= true;
	data.type		= OBJECT_MOVABLE;
}
//-----------------------------------------------------------------------------

void GameLift::update( const float& delta )
{
	assert( body && joint );

	if( joint->GetJointTranslation() <= joint->GetLowerLimit() && enabled )
		joint->SetMotorSpeed( speed );
	if (joint->GetJointTranslation() >= joint->GetUpperLimit() && enabled )
		joint->SetMotorSpeed( -speed );

	b2Vec2 pos = body->GetPosition();
	rotation = body->GetAngle();
	position.x = pos.x * phys_norma;
	position.y = pos.y * phys_norma;

	GameDynamicObject::update(delta);
}
//-----------------------------------------------------------------------------

void GameLift::toggle()
{
	if( joint->GetMotorSpeed() != 0.0f )
	{
		tempSpeed =	joint->GetMotorSpeed();
		joint->SetMotorSpeed(0.0f);
		enabled = false;
	}
	else
	{
		enabled = true;
		joint->SetMotorSpeed(tempSpeed);
	}
}
#endif
//-----------------------------------------------------------------------------