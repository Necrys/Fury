#include "hgefont.h"
#include "hgesprite.h"
#include "ui_il_element.h"
#include "ui_label.h"
#include "ui_main.h"
#include "system_interfaces.h"
#include "ui_imagelist.h"
#include "str_helpers.h"
#include "ui_skin.h"
#include "helper_functions.h"
//-----------------------------------------------------------------------------

namespace gui
{

image_list_item::image_list_item(system *_controller, image_list *_listController, const unsigned int &__id) : _base(_controller, (_base*)_listController)
{
	assert( _listController );

	listController		= _listController;
	_id					= __id;
	back_color			= 0x00FFFFFF;
	back_selected_color	= 0xFFC0C0C0;
	back_mouse_over_color	= 0xFFDCDCDC;
	text_color			= 0xFF000000;
	image				= 0;
	mouseOver			= false;
	align				= HGETEXT_CENTER | HGETEXT_MIDDLE;
	height				= 0;
	scale				= 1.0f;

	backQuad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
	backQuad.tex = 0;
	backQuad.v[0].z = 0.5f;
	backQuad.v[1].z = 0.5f;
	backQuad.v[2].z = 0.5f;
	backQuad.v[3].z = 0.5f;

	imageQuad.v[0].tx = 0.0f;	imageQuad.v[0].ty = 0.0f;	imageQuad.v[0].z = 0.5f;	imageQuad.v[0].col=0xFFFFFFFF;
	imageQuad.v[1].tx = 1.0f;	imageQuad.v[1].ty = 0.0f;	imageQuad.v[1].z = 0.5f;	imageQuad.v[1].col=0xFFFFFFFF;
	imageQuad.v[2].tx = 1.0f;	imageQuad.v[2].ty = 1.0f;	imageQuad.v[2].z = 0.5f;	imageQuad.v[2].col=0xFFFFFFFF;
	imageQuad.v[3].tx = 0.0f;	imageQuad.v[3].ty = 1.0f;	imageQuad.v[3].z = 0.5f;	imageQuad.v[3].col=0xFFFFFFFF;
	imageQuad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;

	sprite = 0;
}
//-----------------------------------------------------------------------------

void image_list_item::render()
{
	uint32 color = back_color;
	if( mouseOver )
		color = back_mouse_over_color;
	if( listController->getSelectionID() == _id )
		color = back_selected_color;

	backQuad.v[0].col = color;
	backQuad.v[1].col = color;
	backQuad.v[2].col = color;
	backQuad.v[3].col = color;

	hgeRect b = bounds;
	if( parent )
	{
		b.x1 += parent->bounds_client.x1;
		b.x2 += parent->bounds_client.x1;
		b.y1 += parent->bounds_client.y1;
		b.y2 += parent->bounds_client.y1;
	}
	bounds_global = b;
	bounds_client = bounds_global;

	backQuad.v[0].x   = b.x1;	backQuad.v[0].y = b.y1;
	backQuad.v[1].x   = b.x2;	backQuad.v[1].y = b.y1;
	backQuad.v[2].x   = b.x2;	backQuad.v[2].y = b.y2;
	backQuad.v[3].x   = b.x1;	backQuad.v[3].y = b.y2;
	hge->Gfx_RenderQuad(&backQuad);

	// render image
	unsigned textShift = 0;
	if( image )
	{
		unsigned imageH, imageW;
		float x, y, w, h;
		sprite->GetTextureRect(&x, &y, &w, &h);
		imageH = (unsigned)(h * scale);
		imageW = (unsigned)(w * scale);
		unsigned panelW = (unsigned)(bounds_global.x2 - bounds_global.x1 - 4.0f);
		unsigned panelH = (unsigned)(bounds_global.y2 - bounds_global.y1 - 4.0f);

		unsigned clSize;
		if( text == "" )
			clSize = imageH;
		else
			clSize = imageH + (unsigned)skin->font->GetHeight();
		unsigned borderW = (panelW / 2 - imageW / 2);
		unsigned borderH = (panelH / 2 - clSize / 2);

		if( sprite )
			sprite->RenderEx( bounds_global.x1 + borderW + 2, bounds_global.y1 + borderH + 2, 0.0f, scale, scale );

		textShift = borderH + imageH;

		height = (unsigned)(h * scale + 4);
		if( text.length() > 0 )
			height += (unsigned)skin->font->GetHeight();
	}

	skin->font->SetColor(text_color);
	skin->font->printfb(bounds_client.x1, textShift + bounds_client.y1, bounds_client.x2 - bounds_client.x1, bounds_client.y2 - (bounds_client.y1 + textShift), align, text.c_str());
}
//-----------------------------------------------------------------------------

void image_list_item::setImage(const std::string& path, 
								  const float& x, 
								  const float& y, 
								  float w, 
								  float h, 
								  const float& imageScale,
								  const uint32& flags)
{
	assert(skin);

	if( path != "" )
	{
		image = textureManager->getTexture(path);
		imageQuad.tex = image;

		if( w == 0.0f )
			w = (float)hge->Texture_GetWidth(image);
		if( h == 0.0f )
			h = (float)hge->Texture_GetWidth(image);

		if( sprite )
			delete sprite;
		sprite = new hgeSprite(image, x, y, w, h);
		sprite->SetFlip( (flags & IF_HORIZONTAL_FLIP) == 0 ? false : true, (flags & IF_VERTICAL_FLIP) == 0 ? false : true );
	}

	height = (unsigned)(h * imageScale + 4);
	if( text.length() > 0 )
		height += (unsigned)skin->font->GetHeight();
	scale = imageScale;
}
//-----------------------------------------------------------------------------

void image_list_item::setImage( const hgeSprite* _sprite, const float& image_scale )
{
	assert(skin);
	assert(_sprite);

	if( sprite )
		delete sprite;

	sprite = new hgeSprite(*_sprite);
	image = sprite->GetTexture();

	height = (unsigned)(sprite->GetHeight() * image_scale + 4.0f);
	if( text.length() > 0 )
		height += (unsigned)skin->font->GetHeight();
	scale = image_scale;
}
//-----------------------------------------------------------------------------

bool image_list_item::handle_event( uint32 msg, uint32 param1, uint32 param2)
{
	if( enabled )
	{
		switch( msg )
		{
		case MSG_MOUSEENTER:
			mouseOver = true;
//			onMouseEnter(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSELEAVE:
			mouseLDown = false;
			mouseRDown = false;
			mouseOver  = false;
//			onMouseLeave(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSEMOVE:
//			onMouseMove(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSELDOWN:
			if( !in_focus )
				controller->set_focus(this);
			mouseLDown = true;
			listController->setSelection(_id);
			break;
		case MSG_MOUSELUP:
//			if( mouseLDown )
//				onMouseLeftClick(HIWORD(param1), LOWORD(param1));
			mouseLDown = false;
			break;
		case MSG_MOUSERDOWN:
			if( !in_focus )
				controller->set_focus(this);
			mouseRDown = true;
			break;
		case MSG_MOUSERUP:
//			if( mouseRDown )
//				onMouseRightClick(HIWORD(param1), LOWORD(param1));
			mouseRDown = false;
			break;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

unsigned image_list_item::getHeight() const
{
	return height;
}
//-----------------------------------------------------------------------------

}