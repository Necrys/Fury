#pragma once
//-----------------------------------------------------------------------------

#include <string>
#include "ui_basecontrol.h"
#include "common.h"
//-----------------------------------------------------------------------------

class hgeFont;
class hgeSprite;
//-----------------------------------------------------------------------------

namespace gui
{
//-----------------------------------------------------------------------------

class image_list;
//-----------------------------------------------------------------------------

class image_list_item : public _base
{
public:
	image_list_item(system* _controller, image_list* listController, const unsigned& id);
	virtual ~image_list_item(){};

	uint32		back_color;
	uint32		back_selected_color;
	uint32		back_mouse_over_color;
	uint32		text_color;

	std::string text;
	int			align;

	virtual void setImage(	const std::string& path, 
							const float& x = 0.0f, 
							const float& y = 0.0f, 
							float w = 0.0f, 
							float h = 0.0f, 
							const float& imageScale = 1.0f,
							const uint32& flags = 0);

	virtual void setImage(	const hgeSprite* sprite, 
							const float& imageScale = 1.0f );

	virtual unsigned getHeight() const;

	virtual void render();

	virtual int id() const { return _id; };

protected:
	bool handle_event( uint32 msg, uint32 param1, uint32 param2 );

	image_list*	listController;

	HTEXTURE	image;
	float		scale;
	hgeSprite*	sprite;
	hgeQuad		imageQuad;
	hgeQuad		backQuad;
	unsigned	_id;
	bool		mouseOver;
	unsigned	height;

	friend class image_list;
};
//-----------------------------------------------------------------------------

typedef image_list_item* LPUIImageListElement;

}