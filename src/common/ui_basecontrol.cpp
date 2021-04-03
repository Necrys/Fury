#include "ui_main.h"
#include "ui_basecontrol.h"
//-----------------------------------------------------------------------------

using namespace std;
//-----------------------------------------------------------------------------

namespace gui
{
//-----------------------------------------------------------------------------

_base::_base(system* _controller, _base* _parent)
{
	ASSERT( _controller );

	user_data = 0;
	on_lost_focus = 0;
	on_take_focus = 0;
	on_create = 0;

	visible = true;
	visible_local = true;
	enabled = true;
	parent = 0;
	mouseLDown	= false;
	mouseRDown  = false;
	in_focus		= false;
	transparency = 1.0f;
	dead = false;
	recieve_user_input = false;
	controller = _controller;

	controller->control_add(this, _parent);
	skin = _controller->skin_get();
}
//-----------------------------------------------------------------------------

_base::~_base()
{
	list<_base*>::iterator it = childs.begin();
	while( it != childs.end() )
	{
		_base* ctrl = *it;
		delete ctrl;
		*it = 0;

		it++;
	}
}
//-----------------------------------------------------------------------------

void _base::update()
{

}
//-----------------------------------------------------------------------------

void _base::render()
{

}
//-----------------------------------------------------------------------------

void _base::set_visibility( const bool& _visible )
{
	visible_local = _visible;
	visible = _visible;

	list<_base*>::iterator it = childs.begin();
	while( it != childs.end() )
	{
		_base* ctrl = *it;
		
		ctrl->visible = _visible;

		it++;
	}
}
//-----------------------------------------------------------------------------

bool _base::is_visible() const
{
	return visible && visible_local;
}
//-----------------------------------------------------------------------------

void _base::set_position( const unsigned& x, const unsigned& y )
{
	bounds.x2 = x + (bounds.x2 - bounds.x1);
	bounds.y2 = y + (bounds.y2 - bounds.y1);
	bounds.x1 = (float)x;
	bounds.y2 = (float)y;
}
//-----------------------------------------------------------------------------

void _base::set_size( const unsigned& w, const unsigned& h )
{
	bounds.x2 = bounds.x1 + w;
	bounds.y2 = bounds.y1 + h;
}
//-----------------------------------------------------------------------------

void _base::set_position_size( const unsigned& x, const unsigned& y, const unsigned& w, const unsigned& h )
{
	bounds.x1 = (float)x;
	bounds.y1 = (float)y;
	bounds.x2 = (float)(x + w);
	bounds.y2 = (float)(y + h);
}
//-----------------------------------------------------------------------------

}