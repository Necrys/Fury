#include "hgefont.h"
#include "ui_dropdownlist.h"
#include "system_interfaces.h"
#include "ui_ddl_element.h"
#include "str_helpers.h"
#include "ui_skin.h"
#include "hgeanim.h"

using namespace std;
//-----------------------------------------------------------------------------

namespace gui
{

drop_list::drop_list(system* _controller, _base* parent): panel(_controller, parent)
{
	ddlBackColor			= 0xFFFFFFFF;
	ddlBorderColor			= 0xFF000000;
	ddlTextColor			= 0xFF000000;
	ddlDisabledTextColor	= 0xFF1F1F1F;
	ddlDisabledBackColor	= 0xFF3F3F3F;
	can_get_focus				= true;

	dropped					= false;
	selectionID				= -1;

	ddlDropIconQuad.v[0].tx = 0.0f;	ddlDropIconQuad.v[0].ty = 0.0f;	ddlDropIconQuad.v[0].z = 0.5f;	ddlDropIconQuad.v[0].col=0xFFFFFFFF;
	ddlDropIconQuad.v[1].tx = 1.0f;	ddlDropIconQuad.v[1].ty = 0.0f;	ddlDropIconQuad.v[1].z = 0.5f;	ddlDropIconQuad.v[1].col=0xFFFFFFFF;
	ddlDropIconQuad.v[2].tx = 1.0f;	ddlDropIconQuad.v[2].ty = 1.0f;	ddlDropIconQuad.v[2].z = 0.5f;	ddlDropIconQuad.v[2].col=0xFFFFFFFF;
	ddlDropIconQuad.v[3].tx = 0.0f;	ddlDropIconQuad.v[3].ty = 1.0f;	ddlDropIconQuad.v[3].z = 0.5f;	ddlDropIconQuad.v[3].col=0xFFFFFFFF;
	ddlDropIconQuad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;

	ddlListPanel.v[0].tx = 0.0f;	ddlListPanel.v[0].ty = 0.0f;	ddlListPanel.v[0].z = 0.5f;	ddlListPanel.v[0].col=ddlBackColor;
	ddlListPanel.v[1].tx = 1.0f;	ddlListPanel.v[1].ty = 0.0f;	ddlListPanel.v[1].z = 0.5f;	ddlListPanel.v[1].col=ddlBackColor;
	ddlListPanel.v[2].tx = 1.0f;	ddlListPanel.v[2].ty = 1.0f;	ddlListPanel.v[2].z = 0.5f;	ddlListPanel.v[2].col=ddlBackColor;
	ddlListPanel.v[3].tx = 0.0f;	ddlListPanel.v[3].ty = 1.0f;	ddlListPanel.v[3].z = 0.5f;	ddlListPanel.v[3].col=ddlBackColor;
	ddlListPanel.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;

	on_change = 0;
}
//-----------------------------------------------------------------------------

drop_list::~drop_list()
{

}
//-----------------------------------------------------------------------------

void drop_list::render()
{
	// render back
	if( !skin )
	{
		log_msg(COLOR_ERROR, "no skin set for dropdown list, can't render");
		return;
	}

	{
		uint32 iconH = (uint32)skin->arrow_down->GetHeight();
		uint32 iconW = (uint32)skin->arrow_down->GetWidth();
		uint32 iconS = max(iconH, iconW);

		if( bounds.y2 - bounds.y1 < iconS )
			bounds.y2 = bounds.y1 + iconS;
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

	bool filtration = hge->System_GetState(HGE_TEXTUREFILTER);
	hge->System_SetState(HGE_TEXTUREFILTER, false);

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

	undroppedBounds = bounds_global;

	hgeRect b = bounds;
	if( parent )
	{
		b.x1 += parent->bounds_client.x1;
		b.x2 += parent->bounds_client.x1;
		b.y1 += parent->bounds_client.y1;
		b.y2 += parent->bounds_client.y1;
	}
	bounds_global = b;

	if( selectionID >= 0 )
	{
		if( enabled )
			skin->font->SetColor(ddlTextColor);
		else
			skin->font->SetColor(ddlDisabledTextColor);
		skin->font->printfb(b.x1 + 5.0f, b.y1, b.x2 - b.x1 - 5.0f, b.y2 - b.y1, HGETEXT_LEFT | HGETEXT_MIDDLE, "%s\0", getSelectionText().c_str());
	}

	uint32 iconH, iconW, iconS;
	iconH = (uint32)skin->arrow_down->GetHeight();
	iconW = (uint32)skin->arrow_down->GetHeight();
	iconS = max(iconH, iconW);
	
	uint32 borderH, borderW;
	borderH = (uint32)((bounds_global.y2 - bounds_global.y1) - iconH) / 2;
	borderW = (uint32)((bounds_global.y2 - bounds_global.y1) - iconW) / 2;

	skin->arrow_down->RenderEx(bounds_global.x2 - iconS + borderW, bounds_global.y1 + borderH, 0.0f );

	if( dropped )
	{
		ddlListPanel.v[0].x = bounds_global.x1;
		ddlListPanel.v[0].y = bounds_global.y2;
		ddlListPanel.v[1].x = bounds_global.x2;
		ddlListPanel.v[1].y = bounds_global.y2;
		ddlListPanel.v[3].x = bounds_global.x1;
		ddlListPanel.v[3].y = bounds_global.y2 + elements.size() * (skin->font->GetHeight() + 4);
		ddlListPanel.v[2].x = bounds_global.x2;
		ddlListPanel.v[2].y = bounds_global.y2 + elements.size() * (skin->font->GetHeight() + 4);
		ddlListPanel.tex = 0;
		hge->Gfx_RenderQuad(&ddlListPanel);
		hge->Gfx_RenderLine(ddlListPanel.v[0].x, ddlListPanel.v[0].y, ddlListPanel.v[1].x, ddlListPanel.v[1].y, ddlBorderColor);
		hge->Gfx_RenderLine(ddlListPanel.v[2].x, ddlListPanel.v[2].y, ddlListPanel.v[1].x, ddlListPanel.v[1].y, ddlBorderColor);
		hge->Gfx_RenderLine(ddlListPanel.v[2].x, ddlListPanel.v[2].y, ddlListPanel.v[3].x, ddlListPanel.v[3].y, ddlBorderColor);
		hge->Gfx_RenderLine(ddlListPanel.v[0].x, ddlListPanel.v[0].y, ddlListPanel.v[3].x, ddlListPanel.v[3].y, ddlBorderColor);

		droppedBounds = bounds_global;
		droppedBounds.y2 = bounds_global.y2 + elements.size() * (skin->font->GetHeight() + 4);

		bounds_global = droppedBounds;
	}

	/* render child controls */
	list<_base*>::iterator it = childs.begin();
	while( it != childs.end() )
	{
		_base* ctrl = *it;

		if( ctrl->is_visible() )
			ctrl->render();

		it++;
	}

	hge->System_SetState(HGE_TEXTUREFILTER, filtration);
}
//-----------------------------------------------------------------------------

string drop_list::getSelectionText() const
{
	if( selectionID < 0 )
		return "";

	if( selectionID > (int)elements.size() - 1 )
		return "";

	return elements[selectionID]->text;
}
//-----------------------------------------------------------------------------

int drop_list::getSelectionID() const
{
	return selectionID;
}
//-----------------------------------------------------------------------------

bool drop_list::handle_event( uint32 msg, uint32 param1, uint32 param2)
{
	list<_base*>::iterator it = childs.begin();
	while( it != childs.end() )
	{
		_base* ctrl = *it;

		if( ctrl->is_visible() )
		{
			bool mousetest = ctrl->bounds_global.TestPoint(HIWORD(param1), LOWORD(param1));

			switch( msg )
			{
			case MSG_MOUSEENTER:
			case MSG_MOUSELEAVE:
			case MSG_MOUSEMOVE:
				if( param1 != param2 )
				{
					bool lasttest = ctrl->bounds_global.TestPoint(HIWORD(param2), LOWORD(param2));

					if( lasttest && !mousetest )
						ctrl->handle_event(MSG_MOUSELEAVE, param1, param2);

					if( !lasttest && mousetest )
						ctrl->handle_event(MSG_MOUSEENTER, param1, param2);

					if( lasttest && mousetest )
						ctrl->handle_event(MSG_MOUSEMOVE, param1, param2);
				}
				break;
			case MSG_MOUSELDOWN:
			case MSG_MOUSELUP:
			case MSG_MOUSERDOWN:
			case MSG_MOUSERUP:
				if( mousetest )
				{
					ctrl->handle_event(msg, param1, param2);
					// SHIT!!!
					if( msg == MSG_MOUSELDOWN )
						drop(!dropped);
					return true;
				}
				break;
			}
		}

		if( ctrl->is_dead() )
		{
			delete ctrl;
			it = childs.erase(it);
		}
		else
		{
			it++;
		}
	}

	if( enabled )
	{
		switch( msg )
		{
		case MSG_MOUSEENTER:
//			onMouseEnter(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSELEAVE:
			mouseLDown = false;
			mouseRDown = false;
			drop(false);
//			onMouseLeave(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSEMOVE:
//			onMouseMove(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSELDOWN:
			if( !in_focus )
				controller->set_focus(this);
			mouseLDown = true;
			drop(!dropped);
			break;
		case MSG_MOUSELUP:
			if( mouseLDown )
//				onMouseLeftClick(HIWORD(param1), LOWORD(param1));
			mouseLDown = false;
			break;
		case MSG_MOUSERDOWN:
			if( !in_focus )
				controller->set_focus(this);
			mouseRDown = true;
			break;
		case MSG_MOUSERUP:
			if( mouseRDown )
//				onMouseRightClick(HIWORD(param1), LOWORD(param1));
			mouseRDown = false;
			break;
		case MSG_CLOSE:
//			this->onClose();
			this->dead = true;
			break;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

void drop_list::insertElement( const string& text )
{
	assert(skin);

	drop_list_item* element = new drop_list_item(controller, this);
	element->text = text;
	element->listController = this;
	element->bounds = hgeRect( 1.0f, bounds.y2 - bounds.y1 + elements.size() * (skin->font->GetHeight()+4), bounds.x2 - bounds.x1 - 1.0f, bounds.y2 - bounds.y1 + (elements.size() + 1) * (skin->font->GetHeight() + 4) - 1.0f );
	element->visible = dropped;
	elements.insert( elements.end(), element );
	element->id = (int)elements.size() - 1;
//	controller->addControl(element, this);
}
//-----------------------------------------------------------------------------

void drop_list::drop( const bool& flag )
{
	dropped = flag;
	for( unsigned i=0; i<elements.size(); i++ )
		elements[i]->visible = dropped;
}
//-----------------------------------------------------------------------------

void drop_list::setSelection(const unsigned int &id)
{
	if( id > elements.size() - 1 )
		return;

	if( selectionID != id )
	{
		selectionID = id;
		if( on_change )
			on_change( selectionID );
	}
}
//-----------------------------------------------------------------------------

void drop_list::clear()
{
	for( unsigned i=0; i<elements.size(); i++ )
		controller->control_remove(elements[i]);
	elements.clear();
	selectionID = -1;
}
//-----------------------------------------------------------------------------

}