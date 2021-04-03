/*
#include <hgeanim.h>
#include "system_ui.h"
#include "data_staticobject.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "system_main.h"
#include "str_helpers.h"
#include "ui_il_element.h"
#include "ui_skin.h"

using namespace std;

//-----------------------------------------------------------------------------

StaticsImageList::~StaticsImageList()
{
	for( unsigned i=0; i<templates.size(); i++ )
	{
		GameStaticObject* obj = templates[i];
		if( obj )
			delete obj;
	}
}
//-----------------------------------------------------------------------------

void StaticsImageList::reloadTemplates()
{
	if( !g_tileset )
		return;

	templates.clear();
	this->elements.clear();

	for( unsigned i=0; i<g_tileset->statics.size(); i++ )
	{
		GameStaticObject* obj = new GameStaticObject();

		obj->texture = g_tileset->statics[i]->anim->GetTexture();
		obj->icon = g_tileset->statics[i]->anim->GetTexture();
		obj->sprite = new hgeAnimation( *g_tileset->statics[i]->anim );
		obj->scale = g_tileset->statics[i]->scale;
		obj->bounds = hgeRect(0.0f, 0.0f, g_tileset->statics[i]->anim->GetWidth() * obj->scale, g_tileset->statics[i]->anim->GetHeight() * obj->scale);
		obj->tilesetSpriteID = g_tileset->statics[i]->id;
		templates.insert(templates.end(), obj);

		this->insertElement(	g_tileset->statics[i]->anim, 
								g_tileset->statics[i]->name,
								g_tileset->statics[i]->iconScale );
	}
}
//-----------------------------------------------------------------------------

void StaticsImageList::onSelectionChanged(const unsigned int &id)
{
	if( id < 0 || id > templates.size()-1 )
		return;

	selected_hflip = false;
	selected_vflip = false;
	selected_rotation = 0.0f;
	selectedStaticTemplate = templates[id];
	selectedStaticTemplate->rotation = 0.0f;
	selectedStaticTemplate->hflip = false;
	selectedStaticTemplate->vflip = false;
	selectedStaticTemplate->fix_bounds();
}
//-----------------------------------------------------------------------------

void StaticsImageList::render()
{
	// render back
	if( !skin )
	{
		log_msg(COLOR_ERROR, "no skin set for image list, can't render");
		return;
	}

	float _transparency = transparency;
	bounds_global = bounds;
	if( parent )
	{
		bounds_global.x1 += parent->bounds_client.x1;
		bounds_global.x2 += parent->bounds_client.x1;
		bounds_global.y1 += parent->bounds_client.y1;
		bounds_global.y2 += parent->bounds_client.y1;
		_transparency *= parent->transparency;
	}
	bounds_client = bounds_global;

	float alpha = _transparency * 255.0f;
	uint32 dwAlpha = (((uint32)alpha))<<24;
	uint32 color = 0x00FFFFFF + dwAlpha;

	float vscale = skin->panel_border / skin->panel_lefttop->GetHeight();
	float hscale = skin->panel_border / skin->panel_lefttop->GetWidth();
	skin->panel_lefttop->SetColor(color);
	skin->panel_lefttop->RenderEx(bounds_global.x1 - skin->panel_border, bounds_global.y1 - skin->panel_border, 0.0f, hscale, vscale);

	vscale = skin->panel_border / skin->panel_top->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->panel_top->GetWidth();
	skin->panel_top->SetColor(color);
	skin->panel_top->RenderEx(bounds_global.x1, bounds_global.y1 - skin->panel_border, 0.0f, hscale, vscale);

	vscale = skin->panel_border / skin->panel_righttop->GetHeight();
	hscale = skin->panel_border / skin->panel_righttop->GetWidth();
	skin->panel_righttop->SetColor(color);
	skin->panel_righttop->RenderEx(bounds_global.x2, bounds_global.y1 - skin->panel_border, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->panel_right->GetHeight();
	hscale = skin->panel_border / skin->panel_right->GetWidth();
	skin->panel_right->SetColor(color);
	skin->panel_right->RenderEx(bounds_global.x2, bounds_global.y1, 0.0f, hscale, vscale);

	vscale = skin->panel_border / skin->panel_rightbottom->GetHeight();
	hscale = skin->panel_border / skin->panel_rightbottom->GetWidth();
	skin->panel_rightbottom->SetColor(color);
	skin->panel_rightbottom->RenderEx(bounds_global.x2, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = skin->panel_border / skin->panel_bottom->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->panel_bottom->GetWidth();
	skin->panel_bottom->SetColor(color);
	skin->panel_bottom->RenderEx(bounds_global.x1, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = skin->panel_border / skin->panel_leftbottom->GetHeight();
	hscale = skin->panel_border / skin->panel_leftbottom->GetWidth();
	skin->panel_leftbottom->SetColor(color);
	skin->panel_leftbottom->RenderEx(bounds_global.x1 - skin->panel_border, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->panel_left->GetHeight();
	hscale = skin->panel_border / skin->panel_left->GetWidth();
	skin->panel_left->SetColor(color);
	skin->panel_left->RenderEx(bounds_global.x1 - skin->panel_border, bounds_global.y1, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->panel_client->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->panel_client->GetWidth();
	skin->panel_client->SetColor(color);
	skin->panel_client->RenderEx(bounds_global.x1, bounds_global.y1, 0.0f, hscale, vscale);

	// draw items
	for( unsigned i=0; i<elements.size(); i++ )
		elements[i]->set_visibility(false);

	unsigned xshift = 24 * 2 + 12;
	unsigned i = scroll;
	pageSize = 0;
	for( i=scroll; i<elements.size(); i+=2 )
	{
		float h;
		if( i == elements.size() - 1 )
			h = (float)elements[i]->getHeight();
		else
			h = (float)max( elements[i]->getHeight(), elements[i+1]->getHeight() );

		// render left element
		if( xshift + h > (bounds_global.y2 - bounds_global.y1) - 12 - 24 * 2 )
			break;
		elements[i]->bounds.y1 = (float)xshift;
		elements[i]->bounds.y2 = (float)(xshift + h);
		elements[i]->bounds.x1 = 0;
		elements[i]->bounds.x2 = (bounds.x2 - bounds.x1) / 2;
		elements[i]->render();
		elements[i]->set_visibility(true);

		// render right element
		if( i+1 < elements.size() )
		{
			elements[i+1]->bounds.y1 = (float)xshift;
			elements[i+1]->bounds.y2 = (float)(xshift + h);
			elements[i+1]->bounds.x1 = (bounds.x2 - bounds.x1) / 2;;
			elements[i+1]->bounds.x2 = (bounds.x2 - bounds.x1);
			elements[i+1]->render();
			elements[i+1]->set_visibility(true);
		}
		xshift += (unsigned)h + 4;
		pageSize += 2;
	}
	if( i == elements.size() )
		canScrollDown = false;
	else
		canScrollDown = true;

	renderButtons();
}
//-----------------------------------------------------------------------------

void StaticsImageList::scrollUp()
{
	if( scroll > 1 )
		scroll-=2;
}
//-----------------------------------------------------------------------------

void StaticsImageList::scrollDown()
{
	if( scroll < (int)elements.size()-2 && canScrollDown )
		scroll+=2;
}
//-----------------------------------------------------------------------------

void StaticsImageList::pageUp()
{
	scroll -= pageSize;
	if( scroll < 0 )
		scroll = 0;
}
//-----------------------------------------------------------------------------

void StaticsImageList::pageDown()
{
	scroll += pageSize;
	if( scroll > (int)elements.size() - 1 )
	{
		if( elements.size() % 2 == 0 )
			scroll = (int)elements.size() - pageSize;
		else
			scroll = (int)elements.size() - pageSize - 1;
	}
	if( scroll < 0 )
		scroll = 0;
}
//-----------------------------------------------------------------------------

void BackgroundIL::reloadTemplates()
{
	if( !g_tileset )
		return;

	templates.clear();
	this->elements.clear();

	for( unsigned i=0; i<g_tileset->backgrounds.size(); i++ )
	{
		GameStaticObject* obj = new GameStaticObject();

		obj->texture = g_tileset->backgrounds[i]->anim->GetTexture();
		obj->icon = g_tileset->backgrounds[i]->anim->GetTexture();
		obj->sprite = new hgeAnimation( *g_tileset->backgrounds[i]->anim );
		obj->scale = g_tileset->backgrounds[i]->scale;
		obj->bounds = hgeRect(0.0f, 0.0f, g_tileset->backgrounds[i]->anim->GetWidth() * obj->scale, g_tileset->backgrounds[i]->anim->GetHeight() * obj->scale);
		obj->tilesetSpriteID = g_tileset->backgrounds[i]->id;
		templates.insert(templates.end(), obj);

		float x, y, w, h;
		g_tileset->backgrounds[i]->anim->GetTextureRect(&x, &y, &w, &h);

		this->insertElement(g_tileset->backgrounds[i]->texture, 
			g_tileset->backgrounds[i]->name, 
			x, y, w, h,
			g_tileset->backgrounds[i]->iconScale);
	}
}
//-----------------------------------------------------------------------------

void BackgroundIL::onSelectionChanged(const unsigned int &id)
{
	if( id < 0 || id > templates.size()-1 )
		return;

	selected_hflip = false;
	selected_vflip = false;
	selected_rotation = 0.0f;
	selectedBackgroundTemplate = templates[id];
	selectedBackgroundTemplate->rotation = 0.0f;
	selectedBackgroundTemplate->hflip = false;
	selectedBackgroundTemplate->vflip = false;
	selectedBackgroundTemplate->fix_bounds();
}
//-----------------------------------------------------------------------------

void ForegroundIL::reloadTemplates()
{
	if( !g_tileset )
		return;

	templates.clear();
	this->elements.clear();

	for( unsigned i=0; i<g_tileset->foregrounds.size(); i++ )
	{
		GameStaticObject* obj = new GameStaticObject();

		obj->texture = g_tileset->foregrounds[i]->anim->GetTexture();
		obj->icon = g_tileset->foregrounds[i]->anim->GetTexture();
		obj->sprite = new hgeAnimation( *g_tileset->foregrounds[i]->anim );
		obj->scale = g_tileset->foregrounds[i]->scale;
		obj->bounds = hgeRect(0.0f, 0.0f, g_tileset->foregrounds[i]->anim->GetWidth() * obj->scale, g_tileset->foregrounds[i]->anim->GetHeight() * obj->scale);
		obj->tilesetSpriteID = g_tileset->foregrounds[i]->id;
		templates.insert(templates.end(), obj);

		float x, y, w, h;
		g_tileset->foregrounds[i]->anim->GetTextureRect(&x, &y, &w, &h);

		this->insertElement(g_tileset->foregrounds[i]->texture, 
			g_tileset->foregrounds[i]->name, 
			x, y, w, h,
			g_tileset->foregrounds[i]->iconScale);
	}
}
//-----------------------------------------------------------------------------

void ForegroundIL::onSelectionChanged(const unsigned int &id)
{
	if( id < 0 || id > templates.size()-1 )
		return;

	selected_hflip = false;
	selected_vflip = false;
	selected_rotation = 0.0f;
	selectedForegroundTemplate = templates[id];
	selectedForegroundTemplate->rotation = 0.0f;
	selectedForegroundTemplate->hflip = false;
	selectedForegroundTemplate->vflip = false;
	selectedForegroundTemplate->fix_bounds();
}
//-----------------------------------------------------------------------------
*/