#pragma once

#include "ui_button.h"
#include "hge.h"
#include <string>
//-----------------------------------------------------------------------------

namespace gui
{

class image_button: public button
{
public:
	image_button(system* controller, _base* parent = 0);
	virtual ~image_button();

	virtual void setImage(const std::string& path);
	virtual void setImage(const HTEXTURE image);

	virtual void render();

protected:
	HTEXTURE	image;
	hgeQuad		imageQuad;

	bool	handle_event( uint32 msg, uint32 param1, uint32 param2);
};
//-----------------------------------------------------------------------------

}