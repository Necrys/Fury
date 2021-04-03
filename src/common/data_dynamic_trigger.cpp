#include "hgefont.h"
#include "hgeanim.h"
#include <list>
#include "data_dynamic_trigger.h"
#include "data_tileset.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "data_level.h"
#include "str_helpers.h"
#include "sys_vars.h"
#include "system_main.h"

using namespace std;

//-----------------------------------------------------------------------------
#ifndef NOEDIT
//TriggerPropsWindow* GameTrigger::propsWindow = 0;
#include "dialog_trigger.h"
#endif
//-----------------------------------------------------------------------------

GameTrigger::GameTrigger( Tile* _tile )
{
	assert(_tile);
	tile = _tile;

	triggeredSprite = 0;
	untriggeredSprite = 0;

	texture = tile->anim->GetTexture();
	icon	= tile->anim->GetTexture();
	untriggeredSprite = new hgeAnimation( *tile->anim );
	scale	= tile->scale;
	sprite	= untriggeredSprite;

	uint32 triggered_x, triggered_y;

	if( tile->params.find( "triggered_x" ) != tile->params.end() )
		triggered_x = str2int( tile->params["triggered_x"] );
	if( tile->params.find( "triggered_y" ) != tile->params.end() )
		triggered_y = str2int( tile->params["triggered_y"] );

	triggeredSprite = new hgeAnimation( tile->anim->GetTexture(), 1, 1.0f, (float)triggered_x, (float)triggered_y, tile->anim->GetWidth(), tile->anim->GetHeight() );

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;
	bounds = hgeRect( 0, 0, half_width * 2.0f , half_height * 2.0f);

#ifdef NOEDIT
//	sprite->SetHotSpot(half_width, half_height);
	body = 0;
#endif

	tilesetSpriteID = tile->id;

	hidden		= false;
	oneWay		= true;
	switched	= false;

	type = DYNAMIC_TRIGGER;

#ifndef NOEDIT
/*	if( !propsWindow )
	{
		propsWindow = new TriggerPropsWindow(gui_system);
		propsWindow->bounds = hgeRect(150.0f, 100.0f, 550.0f, 550.0f );
		propsWindow->caption = "Свойства";
		propsWindow->set_visibility(false);
	}*/
#endif
}
//-----------------------------------------------------------------------------

GameDynamicObject* GameTrigger::clone()
{
	GameTrigger* lift = new GameTrigger( tile );

	lift->setPosition( position );

	return static_cast<GameDynamicObject*>(lift);
}
//-----------------------------------------------------------------------------

void GameTrigger::serialize( FILE* stream )
{
	assert(stream);

	struct dynamic_trigger
	{
		unsigned int	type;
		unsigned int	obj_type;
		unsigned int	tile_id;
		float			position_x;
		float			position_y;
		unsigned int	one_way;
		unsigned int	hidden;
		unsigned int	points_count;
	} obj;

	obj.type = FLF_SECTION_DYNAMIC;
	obj.obj_type = DYNAMIC_TRIGGER;
	obj.tile_id = tilesetSpriteID;
	obj.position_x = position.x;
	obj.position_y = position.y;
	obj.one_way = oneWay ? 1 : 0;
	obj.hidden = hidden ? 1 : 0;
	obj.points_count = (unsigned int)triggeredObjs.size();

	fwrite( &obj, sizeof(dynamic_trigger), 1, stream );

	list<GameDynamicObject*>::iterator it = triggeredObjs.begin();
	while( it != triggeredObjs.end() )
	{
		char id[128];
		memset(id, 0, sizeof(char) * 128);

		strcpy(id, (*it)->id.c_str());
		fwrite( id, sizeof(char), 128, stream );

		it++;
	}
}
//-----------------------------------------------------------------------------

void GameTrigger::setPosition( const vector2f& pos )
{
	position = pos;
#ifdef NOEDIT
	if( body )
		body->SetTransform( b2Vec2(pos.x / phys_norma, pos.y / phys_norma), rotation );
#endif
}
//-----------------------------------------------------------------------------

void GameTrigger::render(const bool& masked)
{
#ifdef NOEDIT
	if( !hidden )
#endif
		GameStaticObject::render(masked);
}
//-----------------------------------------------------------------------------

#ifndef NOEDIT
/*
TriggerPropsWindow::TriggerPropsWindow(gui::system* _controller, gui::_base* parent) : window(_controller, parent)
{
	il_all_objs = new gui::image_list(_controller, this);
	il_all_objs->set_position_size(20, 20, 160, 320);
	
	il_triggered_objs = new gui::image_list(_controller, this);
	il_triggered_objs->set_position_size(220, 20, 160, 320);

	btn_ok = new TriggerPropsWindowOKBtn(_controller, this);
	btn_ok->bounds = hgeRect( 141, 400, 191, 430 );
	btn_ok->setImage( Path::uigfx + "icons\\ok.png" );
	btn_ok->caption = "OK";
	btn_ok->align = HGETEXT_TOP | HGETEXT_CENTER;

	btn_cancel = new TriggerPropsWindowCancelBtn(_controller, this);
	btn_cancel->bounds = hgeRect( 196.0f, 400.0f, 246.0f, 430.0f );
	btn_cancel->setImage( Path::uigfx + "icons\\cancel.png" );
	btn_cancel->caption = "Отмена";
	btn_cancel->align = HGETEXT_TOP | HGETEXT_CENTER;

	btn_add_object = new AddObjectBtn(_controller, this);
	btn_add_object->bounds = hgeRect( 185.0f, 60.0f, 215.0f, 90.0f );
	btn_add_object->setImage( Path::uigfx + "icons\\right.png" );
	btn_add_object->align = HGETEXT_TOP | HGETEXT_CENTER;

	btn_remove_object = new RemoveObjectBtn(_controller, this);
	btn_remove_object->bounds = hgeRect( 185.0f, 100.0f, 215.0f, 130.0f );
	btn_remove_object->setImage( Path::uigfx + "icons\\left.png" );
	btn_remove_object->align = HGETEXT_TOP | HGETEXT_CENTER;

	lbl_twoWays	= new gui::label(_controller, this);
	lbl_twoWays->set_position_size(20, 345, 60, 20);
	lbl_twoWays->text = "Two ways";

	check_twoWays = new gui::checkbox(_controller, this);
	check_twoWays->set_position_size(90, 350, 12, 12);

	lbl_hidden	= new gui::label(_controller, this);
	lbl_hidden->set_position_size(130, 345, 50, 20);
	lbl_hidden->text = "Hidden";

	check_hidden = new gui::checkbox(_controller, this);
	check_hidden->set_position_size(190, 350, 12, 12);
}
*/
//-----------------------------------------------------------------------------

void GameTrigger::showPropsWindow()
{
	/*
	propsWindow->set_visibility(true);
	propsWindow->caller = this;

	propsWindow->il_all_objs->clear();
	propsWindow->il_triggered_objs->clear();

	propsWindow->check_hidden->checked = hidden;
	propsWindow->check_twoWays->checked = !oneWay;

	std::list<GameDynamicObject*>::iterator pit = level->dynamics.begin();
	while( pit != level->dynamics.end() )
	{
		if( (*pit)->switchable() )
		{
			std::list<GameDynamicObject*>::iterator pit2 = triggeredObjs.begin();
			bool found = false;
			while( pit2 != triggeredObjs.end() )
			{
				if( (*pit2)->id == (*pit)->id )
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

			gui::image_list_item* element = propsWindow->il_all_objs->insertElement("", (*pit)->id);
			element->user_data = (void*)(*pit);
		}

		pit++;
	}

	std::list<GameDynamicObject*>::iterator pit2 = triggeredObjs.begin();
	while( pit2 != triggeredObjs.end() )
	{
		gui::image_list_item* element = propsWindow->il_triggered_objs->insertElement("", (*pit2)->id);
		element->user_data = (void*)(*pit2);

		pit2++;
	}

	gui_system->show_modal_window( propsWindow );
	*/
	trigger_dlg_show(this);
}
//-----------------------------------------------------------------------------

#else

void GameTrigger::initInBox2d()
{
	b2BodyDef		bodyDef;
	b2CircleShape	shape;
	b2FixtureDef	fix;
	b2Fixture*		f;

	float half_width = tile->anim->GetWidth() * scale / 2.0f;
	float half_height = tile->anim->GetHeight() * scale / 2.0f;

	bodyDef.position.Set( (position.x + half_width) / phys_norma, (position.y + half_height) / phys_norma);
	bodyDef.type = b2_staticBody;
	body = box2d->CreateBody(&bodyDef);

	shape.m_radius = 24.0f / phys_norma;

	fix.isSensor = true;
	fix.filter.categoryBits = OBJCAT_DYNAMIC;
	fix.filter.maskBits = OBJCAT_PLAYER;
	fix.shape = &shape;

	f = body->CreateFixture(&fix);
	f->SetUserData(&data);

	data.objectPtr	= (void*)this;
	data.isDynamic	= true;
	data.type		= OBJECT_TRIGGER;
	data.isSensor	= true;
}
//-----------------------------------------------------------------------------

void GameTrigger::toggle()
{
	if( !switched )
	{
		switched = !switched;
		sprite = triggeredSprite;

		list<GameDynamicObject*>::iterator it = triggeredObjs.begin();
		while( it != triggeredObjs.end() )
		{
			GameDynamicObject* obj = static_cast<GameDynamicObject*>(*it);
			obj->toggle();

			it++;
		}
	}
	else
	{
		if( !oneWay )
		{
			switched = !switched;
			sprite = untriggeredSprite;

			list<GameDynamicObject*>::iterator it = triggeredObjs.begin();
			while( it != triggeredObjs.end() )
			{
				GameDynamicObject* obj = static_cast<GameDynamicObject*>(*it);
				obj->toggle();

				it++;
			}
		}
	}
}
//-----------------------------------------------------------------------------

#endif