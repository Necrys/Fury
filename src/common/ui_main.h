#pragma once
//-----------------------------------------------------------------------------

#include <list>
#include <assert.h>
#include "common.h"
#include <string>
//-----------------------------------------------------------------------------

class hgeAnimation;
class hgeFont;
//-----------------------------------------------------------------------------

namespace gui
{

class _base;
class window;
class panel;
class edit_box;
class UISkin;

class system
{
public:
	system();
	~system();

	bool	mouse_over_ui;
	bool	text_edit_mode;

	void	set_focus( _base* item );
	void	set_dragged_item( _base* item );
	void	set_active_window( window* win );
	void	send_msg( uint32 msg, uint32 param1, uint32 param2 );
	void	setSkin( const std::string& file );

	void	show_modal_window( window* window );

	void	control_add( _base* control, _base* parent = 0 );
	void	control_remove( _base* control );

	void	update();
	void	render();

	UISkin*	skin_get() const { return skin; };

private:
	std::list<_base*>	controls;
	std::list<_base*>	controls_z_ordered;
	hgeAnimation*				cursor;
	_base*				focused_item;
	_base*				dragged_item;
	vector2f					cursor_last_position;
	vector2f					cursor_position;
	window*					active_window;
	window*					modal_window;
	hgeFont*					font;
	UISkin*						skin;
};
//-----------------------------------------------------------------------------

}