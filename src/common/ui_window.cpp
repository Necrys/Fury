#include "hgefont.h"
#include "system_interfaces.h"
#include "ui_main.h"
#include "ui_Window.h"
#include "str_helpers.h"
#include "ui_skin.h"
#include "hgeanim.h"
//-----------------------------------------------------------------------------

using namespace std;
//-----------------------------------------------------------------------------

namespace gui
{

bool window::handle_event( uint32 msg, uint32 param1, uint32 param2)
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
					return true;
				}
				break;
			case MSG_CLOSE:
				ctrl->handle_event(MSG_CLOSE, 0 ,0);
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

		//		if( ctrl->processEvent(msg, param1, param2) )
		//			return true;

		//it++;
	}

	if( enabled )
	{
		switch( msg )
		{
		case MSG_MOUSEENTER:
			if( on_mouse_enter )
				on_mouse_enter();
			break;
		case MSG_MOUSELEAVE:
			mouseLDown = false;
			mouseRDown = false;
			if( on_mouse_leave )
				on_mouse_leave();
			break;
		case MSG_MOUSEMOVE:
			if( on_mouse_move )
				on_mouse_move(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSELDOWN:
			if( !active )
			{
				controller->set_focus( 0 );
				controller->set_active_window(this);
			}
			if( headerRect.TestPoint(HIWORD(param1), LOWORD(param1)) )
			{
				dragged = true;
				controller->set_dragged_item(this);
			}
			mouseLDown = true;
			break;
		case MSG_MOUSELUP:
			if( mouseLDown && on_mouse_click )
				on_mouse_click( 1, HIWORD(param1), LOWORD(param1) );
			if( dragged )
			{
				dragged = false;
				controller->set_dragged_item(0);
			}
			mouseLDown = false;
			break;
		case MSG_MOUSERDOWN:
			if( !active )
			{
				controller->set_focus( 0 );
				controller->set_active_window(this);
			}
			mouseRDown = true;
			break;
		case MSG_MOUSERUP:
			if( mouseRDown && on_mouse_click )
				on_mouse_click( 2, HIWORD(param1), LOWORD(param1) );
			mouseRDown = false;
			break;
		case MSG_CLOSE:
			if( on_close )
				on_close();
			this->dead = true;
			if( modal )
				controller->show_modal_window(0);
			modal = false;
			break;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

window::window(system* _controller, _base* parent) : panel(_controller, parent)
{
	active = false;
	caption = "Form";
	dragged = false;
	fontColor = 0xFFFFFFFF;
	can_get_focus = false;
	modal = false;

	on_activate = 0;
	on_deactivate = 0;
}
//-----------------------------------------------------------------------------

window::~window()
{
}
//-----------------------------------------------------------------------------

void window::render()
{
	if( !skin )
	{
		log_msg(COLOR_ERROR, "no skin set for window, can't render");
		return;
	}

	float _transparency = transparency;
	bounds_global = bounds;
	if( parent )
	{
		bounds_global.x1 += parent->bounds_global.x1;
		bounds_global.x2 += parent->bounds_global.x1;
		bounds_global.y1 += parent->bounds_global.y1;
		bounds_global.y2 += parent->bounds_global.y1;
		_transparency *= parent->transparency;
	}
	bounds_client = bounds_global;
	bounds_client.y1 = bounds_client.y1 + skin->window_header_width;
	headerRect = bounds_global;
	headerRect.y2 = headerRect.y1 + skin->window_header_width;

	float alpha = _transparency * 255.0f;
	uint32 dwAlpha = (((uint32)alpha))<<24;
	uint32 color = 0x00FFFFFF + dwAlpha;

	unsigned topBorder = skin->window_border + skin->window_header_width;

	bool filtration = hge->System_GetState(HGE_TEXTUREFILTER);
	hge->System_SetState(HGE_TEXTUREFILTER, false);

	float vscale = skin->window_border / skin->window_lefttop->GetHeight();
	float hscale = skin->window_border / skin->window_lefttop->GetWidth();
	skin->window_lefttop->SetColor(color);
	skin->window_lefttop->RenderEx(bounds_global.x1 - skin->window_border, bounds_global.y1 - skin->window_border, 0.0f, hscale, vscale);

	vscale = skin->window_border / skin->window_top->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->window_top->GetWidth();
	skin->window_top->SetColor(color);
	skin->window_top->RenderEx(bounds_global.x1, bounds_global.y1 - skin->window_border, 0.0f, hscale, vscale);

	vscale = skin->window_border / skin->window_righttop->GetHeight();
	hscale = skin->window_border / skin->window_righttop->GetWidth();
	skin->window_righttop->SetColor(color);
	skin->window_righttop->RenderEx(bounds_global.x2, bounds_global.y1 - skin->window_border, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->window_right->GetHeight();
	hscale = skin->window_border / skin->window_right->GetWidth();
	skin->window_right->SetColor(color);
	skin->window_right->RenderEx(bounds_global.x2, bounds_global.y1, 0.0f, hscale, vscale);

	vscale = skin->window_border / skin->window_rightbottom->GetHeight();
	hscale = skin->window_border / skin->window_rightbottom->GetWidth();
	skin->window_rightbottom->SetColor(color);
	skin->window_rightbottom->RenderEx(bounds_global.x2, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = skin->window_border / skin->window_bottom->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->window_bottom->GetWidth();
	skin->window_bottom->SetColor(color);
	skin->window_bottom->RenderEx(bounds_global.x1, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = skin->window_border / skin->window_leftbottom->GetHeight();
	hscale = skin->window_border / skin->window_leftbottom->GetWidth();
	skin->window_leftbottom->SetColor(color);
	skin->window_leftbottom->RenderEx(bounds_global.x1 - skin->window_border, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->window_left->GetHeight();
	hscale = skin->window_border / skin->window_left->GetWidth();
	skin->window_left->SetColor(color);
	skin->window_left->RenderEx(bounds_global.x1 - skin->window_border, bounds_global.y1, 0.0f, hscale, vscale);

	vscale = skin->window_header_width / skin->window_header->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->window_header->GetWidth();
	skin->window_header->SetColor(color);
	skin->window_header->RenderEx(bounds_global.x1, bounds_global.y1, 0.0f, hscale, vscale);

	// caption
	uint32 x, y, w, h;
	x = (uint32)bounds_global.x1;
	y = (uint32)bounds_global.y1;
	w = (uint32)(bounds_global.x2 - bounds_global.x1);
	h = (uint32)skin->window_header_width;
	skin->font->SetColor(fontColor);
	skin->font->printfb((float)x, (float)y, (float)w, (float)h, HGETEXT_LEFT | HGETEXT_MIDDLE, caption.c_str());

	vscale = (bounds_client.y2 - bounds_client.y1) / skin->window_client->GetHeight();
	hscale = (bounds_client.x2 - bounds_client.x1) / skin->window_client->GetWidth();
	skin->window_client->SetColor(color);
	skin->window_client->RenderEx(bounds_client.x1, bounds_client.y1, 0.0f, hscale, vscale);

	// render child controls
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

void window::update()
{
	if( !is_visible() && modal )
	{
		controller->show_modal_window(0);
		modal = false;
	}

	panel::update();
}
//-----------------------------------------------------------------------------

}