#include "hgefont.h"
#include "ui_main.h"
#include "ui_imagebutton.h"
#include "system_interfaces.h"
#include "str_helpers.h"
#include "ui_skin.h"
#include "hgeanim.h"

using namespace std;
//-----------------------------------------------------------------------------

namespace gui
{

image_button::image_button(system* _controller, _base* parent): button(_controller, parent)
{
	image = 0;

	imageQuad.v[0].tx = 0.0f;	imageQuad.v[0].ty = 0.0f;	imageQuad.v[0].z = 0.5f;	imageQuad.v[0].col=0xFFFFFFFF;
	imageQuad.v[1].tx = 1.0f;	imageQuad.v[1].ty = 0.0f;	imageQuad.v[1].z = 0.5f;	imageQuad.v[1].col=0xFFFFFFFF;
	imageQuad.v[2].tx = 1.0f;	imageQuad.v[2].ty = 1.0f;	imageQuad.v[2].z = 0.5f;	imageQuad.v[2].col=0xFFFFFFFF;
	imageQuad.v[3].tx = 0.0f;	imageQuad.v[3].ty = 1.0f;	imageQuad.v[3].z = 0.5f;	imageQuad.v[3].col=0xFFFFFFFF;
	imageQuad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
}
//-----------------------------------------------------------------------------

image_button::~image_button()
{

}
//-----------------------------------------------------------------------------

bool image_button::handle_event( uint32 msg, uint32 param1, uint32 param2 )
{
	if( enabled )
	{
		switch( msg )
		{
		case MSG_MOUSEENTER:
			mouseOver = true;
			if( on_mouse_move )
				on_mouse_move( HIWORD(param1), LOWORD(param1) );
			break;
		case MSG_MOUSELEAVE:
			mouseLDown = false;
			mouseRDown = false;
			mouseOver = false;
			if( on_mouse_leave )
				on_mouse_leave();
			break;
		case MSG_MOUSEMOVE:
			if( on_mouse_move )
				on_mouse_move(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSELDOWN:
			mouseLDown = true;
			if( !in_focus )
				controller->set_focus(this);
			if( on_press )
				on_press( this );
			break;
		case MSG_MOUSELUP:
			mouseLDown = false;
			if( mouseLDown && on_mouse_click )
				on_mouse_click( 1, HIWORD(param1), LOWORD(param1) );
			break;
		case MSG_MOUSERDOWN:
			mouseRDown = true;
			if( !in_focus )
				controller->set_focus(this);
			break;
		case MSG_MOUSERUP:
			mouseRDown = false;
			if( mouseRDown && on_mouse_click )
				on_mouse_click( 2, HIWORD(param1), LOWORD(param1) );
			break;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

void image_button::render()
{
	if( !skin )
	{
		log_msg(COLOR_ERROR, "no skin set for image button, can't render");
		return;
	}

	float _transparency = transparency;
	bounds_global = bounds;
	if( parent )
	{
		bounds_global.x1 += parent->bounds_client.x1;
		bounds_global.x2 += parent->bounds_client.x1;
		bounds_global.y1 += parent->bounds_client.y1;
		bounds_global.y2 += parent->bounds_client.y1;
		_transparency *= parent->transparency;
	}
	bounds_client = bounds_global;

	float alpha = _transparency * 255.0f;
	uint32 dwAlpha = (((uint32)alpha))<<24;
	uint32 color = 0x00FFFFFF + dwAlpha;

	uint16	state;

	if( !enabled )
		state = PRESSED;
	else if( mouseOver && mouseLDown )
		state = PRESSED;
	else if( mouseOver )
		state = MOUSEOVER;
	else
	state = UNPRESSED;

	bool filtration = hge->System_GetState(HGE_TEXTUREFILTER);
	hge->System_SetState(HGE_TEXTUREFILTER, false);

	float vscale = skin->button_border / skin->button_lefttop[state]->GetHeight();
	float hscale = skin->button_border / skin->button_lefttop[state]->GetWidth();
	skin->button_lefttop[state]->SetColor(color);
	skin->button_lefttop[state]->RenderEx(bounds_global.x1 - skin->button_border, bounds_global.y1 - skin->button_border, 0.0f, hscale, vscale);

	vscale = skin->button_border / skin->button_top[state]->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->button_top[state]->GetWidth();
	skin->button_top[state]->SetColor(color);
	skin->button_top[state]->RenderEx(bounds_global.x1, bounds_global.y1 - skin->button_border, 0.0f, hscale, vscale);

	vscale = skin->button_border / skin->button_righttop[state]->GetHeight();
	hscale = skin->button_border / skin->button_righttop[state]->GetWidth();
	skin->button_righttop[state]->SetColor(color);
	skin->button_righttop[state]->RenderEx(bounds_global.x2, bounds_global.y1 - skin->button_border, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->button_right[state]->GetHeight();
	hscale = skin->button_border / skin->button_right[state]->GetWidth();
	skin->button_right[state]->SetColor(color);
	skin->button_right[state]->RenderEx(bounds_global.x2, bounds_global.y1, 0.0f, hscale, vscale);

	vscale = skin->button_border / skin->button_rightbottom[state]->GetHeight();
	hscale = skin->button_border / skin->button_rightbottom[state]->GetWidth();
	skin->button_rightbottom[state]->SetColor(color);
	skin->button_rightbottom[state]->RenderEx(bounds_global.x2, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = skin->button_border / skin->button_bottom[state]->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->button_bottom[state]->GetWidth();
	skin->button_bottom[state]->SetColor(color);
	skin->button_bottom[state]->RenderEx(bounds_global.x1, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = skin->button_border / skin->button_leftbottom[state]->GetHeight();
	hscale = skin->button_border / skin->button_leftbottom[state]->GetWidth();
	skin->button_leftbottom[state]->SetColor(color);
	skin->button_leftbottom[state]->RenderEx(bounds_global.x1 - skin->button_border, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->button_left[state]->GetHeight();
	hscale = skin->button_border / skin->button_left[state]->GetWidth();
	skin->button_left[state]->SetColor(color);
	skin->button_left[state]->RenderEx(bounds_global.x1 - skin->button_border, bounds_global.y1, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->button_client[state]->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->button_client[state]->GetWidth();
	skin->button_client[state]->SetColor(color);
	skin->button_client[state]->RenderEx(bounds_global.x1, bounds_global.y1, 0.0f, hscale, vscale);

	hge->System_SetState(HGE_TEXTUREFILTER, filtration);

	unsigned textShift = 0;

	if( image )
	{
		unsigned imageH = hge->Texture_GetHeight(image);
		unsigned imageW = hge->Texture_GetWidth(image);
		unsigned buttonW = (unsigned)(bounds_global.x2 - bounds_global.x1);
		unsigned buttonH = (unsigned)(bounds_global.y2 - bounds_global.y1);

		unsigned clSize;
		if( caption == "" )
			clSize = imageH;
		else
			clSize = imageH + (unsigned)skin->font->GetHeight();
		unsigned borderW = (buttonW / 2 - imageW / 2);
		unsigned borderH = (buttonH / 2 - clSize / 2);

		imageQuad.v[0].x = bounds_global.x1 + borderW;
		imageQuad.v[0].y = bounds_global.y1 + borderH;
		imageQuad.v[1].x = bounds_global.x2 - borderW;
		imageQuad.v[1].y = bounds_global.y1 + borderH;
		imageQuad.v[2].x = bounds_global.x2 - borderW;
		imageQuad.v[2].y = imageQuad.v[0].y + imageH;
		imageQuad.v[3].x = bounds_global.x1 + borderW;
		imageQuad.v[3].y = imageQuad.v[0].y + imageH;

		hge->Gfx_RenderQuad(&imageQuad);

		textShift = borderH + imageH;
	}

	// render text
	if( enabled )
		skin->font->SetColor(fontColor);
	else
		skin->font->SetColor(fontDisabledColor);
	skin->font->printfb(bounds_client.x1, textShift + bounds_client.y1, bounds_client.x2 - bounds_client.x1, bounds_client.y2 - (bounds_client.y1 + textShift), align, caption.c_str());
}
//-----------------------------------------------------------------------------

void image_button::setImage(const string& path)
{
	image = textureManager->getTexture(path);
	imageQuad.tex = image;
}
//-----------------------------------------------------------------------------

void image_button::setImage(const HTEXTURE _image)
{
	image = _image;
	imageQuad.tex = image;
}
//-----------------------------------------------------------------------------

}