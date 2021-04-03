#include "system_interfaces.h"

#include "ui_main.h"
#include "ui_basecontrol.h"
#include "ui_window.h"
#include "ui_skin.h"

#include "hgeanim.h"
#include "hgefont.h"
//-----------------------------------------------------------------------------

using namespace std;
//-----------------------------------------------------------------------------

namespace gui
{
//-----------------------------------------------------------------------------

system::system()
{
	cursor			= 0;
	font			= 0;
	focused_item	= 0;
	dragged_item	= 0;
	active_window	= 0;
	text_edit_mode	= false;
	modal_window	= 0;
	skin			= 0;
}
//-----------------------------------------------------------------------------

system::~system()
{
	controls_z_ordered.clear();

	list<_base*>::iterator it = controls.begin();
	while( it != controls.end() )
	{
		_base* ctrl = *it;
		delete ctrl;
		*it = 0;

		it++;
	}

	if( cursor )
		delete cursor;

	if( skin )
		delete skin;
}
//-----------------------------------------------------------------------------

void system::update()
{
	hge->Input_GetMousePos(&cursor_position.x, &cursor_position.y);
	BYTE key	= hge->Input_GetKey();
	BYTE shift	= hge->Input_GetKeyState(HGEK_SHIFT) ? 1 : 0;
	BYTE ctrl	= hge->Input_GetKeyState(HGEK_CTRL) ? 1 : 0;
	BYTE alt	= hge->Input_GetKeyState(HGEK_ALT) ? 1 : 0;
	char ch		= hge->Input_GetChar();

	// provide keypress to focused item
	if( (ch != '\0') || ( key == HGEK_BACKSPACE )	|| ( key == HGEK_ENTER ) 
					 || ( key == HGEK_LEFT )		|| ( key == HGEK_RIGHT )
					 || ( key == HGEK_END			|| ( key == HGEK_HOME )
					 || ( key == HGEK_DELETE		|| ( key == HGEK_INSERT ) )))
		if( focused_item )
		{
			uint32 param1 = MAKELONG(MAKEWORD(key, shift), MAKEWORD(ctrl, alt));
			focused_item->handle_event(MSG_KEYPRESS, param1, ch);
		}

	bool bLPressed  = hge->Input_KeyDown(HGEK_LBUTTON);
	bool bLReleased = hge->Input_KeyUp(HGEK_LBUTTON);
	bool bRPressed  = hge->Input_KeyDown(HGEK_RBUTTON);
	bool bRReleased = hge->Input_KeyUp(HGEK_RBUTTON);

	mouse_over_ui = false;

	bool focusedMouseTest;

	if( focused_item )
		focusedMouseTest = focused_item->bounds_global.TestPoint(cursor_position.x, cursor_position.y);

	if( cursor_last_position != cursor_position ) 
		if( dragged_item )
		{
			int dx = (int)(cursor_position.x - cursor_last_position.x);
			int dy = (int)(cursor_position.y - cursor_last_position.y);
			dragged_item->bounds.x1 += dx;
			dragged_item->bounds.x2 += dx; 
			dragged_item->bounds.y1 += dy;
			dragged_item->bounds.y2 += dy;
			dragged_item->bounds_global.x1 += dx;
			dragged_item->bounds_global.x2 += dx; 
			dragged_item->bounds_global.y1 += dy;
			dragged_item->bounds_global.y2 += dy;
		}

	list<_base*>::iterator it = controls.begin();
	while( it != controls.end() )
	{
		_base* ctrl = *it;

		ctrl->update();

		if( ctrl->is_dead() )
		{
			controls_z_ordered.remove(ctrl);
			delete ctrl;
			it = controls.erase(it);
		}
		else
		{
			it++;
		}
	}

	list<_base*>::reverse_iterator itZ = controls_z_ordered.rbegin();
	while( itZ != controls_z_ordered.rend() )
	{
		_base* ctrl = *itZ;

		if( ctrl->visible )
		{
			bool mousetest = ctrl->bounds_global.TestPoint(cursor_position.x, cursor_position.y);
			WORD posX = (WORD)cursor_position.x;
			WORD posY = (WORD)cursor_position.y;
			uint32 mousePosLParam = MAKELONG(posY, posX);

			if( cursor_last_position != cursor_position )
			{
				bool lasttest = ctrl->bounds_global.TestPoint(cursor_last_position.x, cursor_last_position.y);

				WORD lposX = (WORD)cursor_last_position.x;
				WORD lposY = (WORD)cursor_last_position.y;

				uint32 lastPos = MAKELONG(lposY, lposX);

				if( lasttest && !mousetest )
					ctrl->handle_event(MSG_MOUSELEAVE, mousePosLParam, lastPos);

				if( !lasttest && mousetest )
					ctrl->handle_event(MSG_MOUSEENTER, mousePosLParam, lastPos);

				if( lasttest && mousetest )
					ctrl->handle_event(MSG_MOUSEMOVE, mousePosLParam, lastPos);
			}

			if( mousetest )
			{
				if( modal_window )
					if( ctrl != modal_window )
					{
						itZ++;
						continue;
					}

				mouse_over_ui = true;
				if( bLPressed )
					ctrl->handle_event(MSG_MOUSELDOWN, mousePosLParam, 0);
				if( bLReleased )
					ctrl->handle_event(MSG_MOUSELUP, mousePosLParam, 0);
				if( bRPressed )
					ctrl->handle_event(MSG_MOUSERDOWN, mousePosLParam, 0);
				if( bRReleased )
					ctrl->handle_event(MSG_MOUSERUP, mousePosLParam, 0);
				break;
			}
		}

		itZ++;
	}

	if( !mouse_over_ui )
		if( bLPressed || bRPressed )
		{
			set_focus(0);
			set_active_window(0);
		}

	cursor_last_position = cursor_position;
}
//-----------------------------------------------------------------------------

void system::render()
{
	/* render controls */
	list<_base*>::iterator it = controls_z_ordered.begin();
	while( it != controls_z_ordered.end() )
	{
		_base* ctrl = *it;

		if( ctrl->visible )
			ctrl->render();

		it++;
	}

	if( cursor )
	{
		vector2f cur_pos;
		hge->Input_GetMousePos(&cur_pos.x, &cur_pos.y);
		cursor->Render(cur_pos.x, cur_pos.y);
	}
}
//-----------------------------------------------------------------------------

void system::set_focus(_base* item)
{
	if( focused_item )
	{
		focused_item->in_focus = false;
		if( focused_item->on_lost_focus )
			focused_item->on_lost_focus();
	}
	if( item && item->can_get_focus )
	{
		item->in_focus = true;
		if( item->on_take_focus )
			item->on_take_focus();
		focused_item = item;
		if( item->recieve_user_input )
			text_edit_mode = true;
		else
			text_edit_mode = false;
	}
	if( !item )
	{
		focused_item = 0;
		text_edit_mode = false;
	}

	_base* ctrl = item;
	if( ctrl )
	{
		while( ctrl->parent )
		{
			ctrl->parent->childs.remove(ctrl);
			ctrl->parent->childs.push_front(ctrl);

			window* win = dynamic_cast<window*>(ctrl->parent);
			if( win )
				set_active_window( win );

			ctrl = ctrl->parent;
		}
		controls_z_ordered.remove(ctrl);
		controls_z_ordered.push_back(ctrl);
	}
}
//-----------------------------------------------------------------------------

void system::control_add(_base* control, _base* parent)
{
	assert(control);
	assert( !control->parent );

	if( parent )
	{
		parent->childs.push_front(control);
		control->parent = parent;
	}
	else
	{
		controls.push_front(control);
		controls_z_ordered.push_front(control);
	}
	control->controller = this;

	if( control->on_create )
		control->on_create();
}
//-----------------------------------------------------------------------------

void system::send_msg( uint32 msg, uint32 param1, uint32 param2 )
{
	list<_base*>::iterator it = controls.begin();
	while( it != controls.end() )
	{
		_base* ctrl = *it;

		if( ctrl->handle_event(msg, param1, param2) )
			break;

		it++;
	}
}
//-----------------------------------------------------------------------------

void system::set_dragged_item(_base* item)
{
	dragged_item = item;
}
//-----------------------------------------------------------------------------

void system::set_active_window(window* win)
{
	if( active_window )
	{
		active_window->active = false;

		if( active_window->on_deactivate )
			active_window->on_deactivate();
	}
	active_window = win;
	if( active_window )
	{
		active_window->active = true;

		if( active_window->on_activate )
			active_window->on_activate();
	}

	_base* ctrl = win;
	if( ctrl )
	{
		while( ctrl->parent )
		{
			ctrl->parent->childs.remove(ctrl);
			ctrl->parent->childs.push_back(ctrl);

			window* win = dynamic_cast<window*>(ctrl->parent);
			if( win )
				set_active_window( win );

			ctrl = ctrl->parent;
		}
		controls_z_ordered.remove(ctrl);
		controls_z_ordered.push_back(ctrl);
	}
}
//-----------------------------------------------------------------------------

void system::control_remove(_base* control)
{
	if( control )
	{
		if( control->in_focus )
			set_focus(0);
		if( control == active_window )
			set_active_window(0);
		if( focused_item == control )
			focused_item = 0;

		control->handle_event(MSG_CLOSE, 0, 0);

		if( control->parent )
			control->parent->childs.remove(control);

		delete control;
	}
}
//-----------------------------------------------------------------------------

void system::show_modal_window( window* window )
{
	if( modal_window )
		modal_window->modal = false;
	modal_window = window;
	set_focus(window);
	if( modal_window )
		modal_window->modal = true;
}
//-----------------------------------------------------------------------------

void system::setSkin( const std::string& file )
{
	skin = new UISkin(file);
	if( !skin->loaded() )
	{
		delete skin;
		skin = 0;
	}

	cursor	= skin->cursor;
	font	= skin->font;
}
//-----------------------------------------------------------------------------

}