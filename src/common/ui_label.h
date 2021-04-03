#pragma once

#include <string>
#include "ui_BaseControl.h"
//-----------------------------------------------------------------------------

namespace gui
{

class label : public _base
{
public:
	label(system* controller, _base* parent = 0);
	virtual ~label();

	std::string	text;
	uint32		back_color;
	uint32		font_color;
	uint32		font_disabled_color;
	int			align;

	virtual void render();

protected:
	bool		handle_event( uint32 msg, uint32 param1, uint32 param2 );
};
//-----------------------------------------------------------------------------

}