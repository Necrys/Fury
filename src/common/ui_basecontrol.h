#pragma once

#include <list>
#include "hgerect.h"
#include "common.h"
//-----------------------------------------------------------------------------

namespace gui
{
//-----------------------------------------------------------------------------

class system;
class UISkin;
//-----------------------------------------------------------------------------

enum UIEVENTMSG
{
	MSG_MOUSEENTER,
	MSG_MOUSEMOVE,
	MSG_MOUSELEAVE,
	MSG_MOUSELDOWN,
	MSG_MOUSELUP,
	MSG_MOUSELCLICK,
	MSG_MOUSERDOWN,
	MSG_MOUSERUP,
	MSG_MOUSERCLICK,
	MSG_KEYPRESS,
	MSG_KEYDOWN,
	MSG_KEYUP,
	MSG_CREATE,
	MSG_CLOSE,
	MSG_SHOW,

	MSG_FORCE_DWORD = 0x77777777,
};
//-----------------------------------------------------------------------------

class _base
{
public:
	_base(system* controller, _base* parent = 0);
	virtual ~_base();

	hgeRect	bounds_global;
	hgeRect	bounds_client;
	hgeRect	bounds;
	bool	enabled;
	bool	can_get_focus;
	float	transparency;
	void*	user_data;

	virtual bool handle_event( uint32 msg, uint32 param1, uint32 param2) = 0;
	virtual void update();
	virtual void render();

	virtual void set_visibility( const bool& visible );
	virtual bool is_visible() const;

	virtual void set_size( const unsigned& w, const unsigned& h );
	virtual void set_position( const unsigned& x, const unsigned& y );
	virtual void set_position_size( const unsigned& x, const unsigned& y, const unsigned& w, const unsigned& h );

/*	// mouse events
	virtual void onMouseMove(const int& x, const int& y){};
	virtual void onMouseEnter(const int& x, const int& y){};
	virtual void onMouseLeave(const int& x, const int& y){};
	virtual void onMouseLeftClick(const int& x, const int& y){};
	virtual void onMouseRightClick(const int& x, const int& y){};
	virtual void onMouseWheel(const int& x, const int& y, const int& wheel){};

	// keyboard events
	virtual void onKeyDown(const int& key, const int& ch){};
	virtual void onKeyUp(const int& key, const int& ch){};
	virtual void onKeyPress(const int& key, const int& ch){};

	// other events
	virtual void onCreate(){};
	virtual void onClose(){};
	virtual void onShow(){};
	virtual void onFocusGet(){};
	virtual void onFocusLost(){};
*/
	// events
	void	(*on_lost_focus)();
	void	(*on_take_focus)();
	void	(*on_create)();

	virtual bool is_dead(){ return dead; };
	_base* get_parent()const { return parent; };

	friend class system;

protected:
	system*				controller;
	UISkin*				skin;

	std::list<_base*>	childs;
	_base*				parent;
	bool				mouseLDown;
	bool				mouseRDown;
	bool				in_focus;
	bool				dead;
	bool				recieve_user_input;
	bool				visible;
	bool				visible_local;
};
//-----------------------------------------------------------------------------

}