#include "system_interfaces.h"
#include "ui_main.h"
#include "ui_checkbox.h"
#include "ui_skin.h"
#include "hgeanim.h"

using namespace std;
//-----------------------------------------------------------------------------

namespace gui
{

bool checkbox::handle_event( uint32 msg, uint32 param1, uint32 param2 )
{
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
			if( mouseLDown )
			{
				if( mouseLDown && on_mouse_click )
					on_mouse_click( 1, HIWORD(param1), LOWORD(param1) );
				checked = !checked;
				if( on_change )
					on_change( checked );
			}
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
			this->dead = true;
			break;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

checkbox::checkbox(system* _controller, _base* parent): panel(_controller, parent)
{
	checked = false;
	on_change = 0;
}
//-----------------------------------------------------------------------------

checkbox::~checkbox()
{

}
//-----------------------------------------------------------------------------

void checkbox::render()
{
	panel::render();

	if( checked )
	{
		bool filtration = hge->System_GetState(HGE_TEXTUREFILTER);
		hge->System_SetState(HGE_TEXTUREFILTER, false);

		uint32 x = (uint32)((bounds_global.x2 - bounds_global.x1) - ( skin->check_icon->GetWidth() )) / 2;
		uint32 y = (uint32)((bounds_global.y2 - bounds_global.y1) - ( skin->check_icon->GetHeight() )) / 2;
		skin->check_icon->RenderEx((float)(bounds_global.x1 + x), (float)( bounds_global.y1 + y), 0.0f);

		hge->System_SetState(HGE_TEXTUREFILTER, filtration);
	}
}
//-----------------------------------------------------------------------------

}