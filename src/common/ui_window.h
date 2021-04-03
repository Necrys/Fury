#pragma once

#include "types.h"
#include "ui_panel.h"
#include <string>
//-----------------------------------------------------------------------------

namespace gui
{

class window: public panel
{
public:
	std::string	caption;
	uint32			fontColor;

	window(system* controller, _base* parent = 0);
	virtual ~window();

	virtual void render();
	virtual void update();

	// events
	void	(*on_activate)();
	void	(*on_deactivate)();

	bool modal;

	friend class system;

protected:

	bool		dragged;
	vector2f	draggedPoint;
	hgeRect		headerRect;
	bool		active;

	bool		handle_event( uint32 msg, uint32 param1, uint32 param2);
};
//-----------------------------------------------------------------------------

}