#include "system_interfaces.h"
#include "ui_main.h"
#include "ui_panel.h"
#include "ui_skin.h"
#include "hgeanim.h"

using namespace std;

namespace gui
{

bool panel::handle_event( uint32 msg, uint32 param1, uint32 param2)
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
			if( !in_focus )
				controller->set_focus(this);
			mouseLDown = true;
			break;
		case MSG_MOUSELUP:
			if( mouseLDown && on_mouse_click )
				on_mouse_click( 1, HIWORD(param1), LOWORD(param1) );
			mouseLDown = false;
			break;
		case MSG_MOUSERDOWN:
			if( !in_focus )
				controller->set_focus(this);
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
			break;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

panel::panel(system* _controller, _base* parent) : _base(_controller, parent)
{
	can_get_focus = true;

	on_close = 0;
	on_mouse_click = 0;
	on_show = 0;
	on_mouse_leave = 0;
	on_mouse_enter = 0;
	on_mouse_move = 0;
}
//-----------------------------------------------------------------------------

panel::~panel()
{
}
//-----------------------------------------------------------------------------

void panel::update()
{
	list<_base*>::iterator it = childs.begin();
	while( it != childs.end() )
	{
		_base* ctrl = *it;

		ctrl->update();

		it++;
	}
}
//-----------------------------------------------------------------------------

void panel::render()
{
	if( !is_visible() )
		return;

	if( !skin )
	{
		log_msg(COLOR_ERROR, "no skin set for panel, can't render");
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

	// render child controls
	list<_base*>::reverse_iterator it = childs.rbegin();
	while( it != childs.rend() )
	{
		_base* ctrl = *it;

		if( ctrl->is_visible() )
			ctrl->render();

		it++;
	}

	hge->System_SetState(HGE_TEXTUREFILTER, filtration);
}
//-----------------------------------------------------------------------------

}