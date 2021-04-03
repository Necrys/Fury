#pragma once

#include "ui_BaseControl.h"
//-----------------------------------------------------------------------------

namespace gui
{
//-----------------------------------------------------------------------------

class _base;
class UISkin;
//-----------------------------------------------------------------------------

class panel: public _base
{
public:
	panel(system* controller, _base* parent = 0);
	virtual ~panel();

	virtual void update();
	virtual void render();

	// events
	void	(*on_mouse_move)( uint32 x, uint32 y );
	void	(*on_mouse_enter)();
	void	(*on_mouse_leave)();
	void	(*on_show)();
	void	(*on_mouse_click)( uint32 param, uint32 x, uint32 y );
	void	(*on_close)();

protected:
	
	bool handle_event( uint32 msg, uint32 param1, uint32 param2 );
};
//-----------------------------------------------------------------------------

}