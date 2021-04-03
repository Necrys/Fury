#include "hgefont.h"
#include "ui_main.h"
#include "system_interfaces.h"
#include "ui_Label.h"
#include "str_helpers.h"
#include "ui_skin.h"

using namespace std;
//-----------------------------------------------------------------------------
namespace gui
{

label::label(system* _controller, _base* parent) : _base(_controller, parent)
{
	text = "Label";
	back_color = 0x00FFFFFF;
	font_color = 0xFFFFFFFF;
	font_disabled_color = 0xFFAAAAAA;
	align = HGETEXT_LEFT | HGETEXT_MIDDLE;
	can_get_focus = false;
}
//-----------------------------------------------------------------------------

label::~label()
{
}
//-----------------------------------------------------------------------------

void label::render()
{
	hgeQuad quad;
	quad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
	quad.tex = 0;
	quad.v[0].col = back_color;
	quad.v[1].col = back_color;
	quad.v[2].col = back_color;
	quad.v[3].col = back_color;

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

	quad.v[0].x   = b.x1;	quad.v[0].y = b.y1;	quad.v[0].z = 0.5f;
	quad.v[1].x   = b.x2;	quad.v[1].y = b.y1;	quad.v[1].z = 0.5f;
	quad.v[2].x   = b.x2;	quad.v[2].y = b.y2;	quad.v[2].z = 0.5f;
	quad.v[3].x   = b.x1;	quad.v[3].y = b.y2;	quad.v[3].z = 0.5f;
	hge->Gfx_RenderQuad(&quad);

	if( enabled )
		skin->font->SetColor(font_color);
	else
		skin->font->SetColor(font_disabled_color);
	skin->font->printfb(b.x1, b.y1, b.x2 - b.x1, b.y2 - b.y1, align, text.c_str());
}
//-----------------------------------------------------------------------------

bool label::handle_event( uint32 msg, uint32 param1, uint32 param2 )
{
	if( enabled )
	{
		switch( msg )
		{
		case MSG_MOUSEENTER:
//			onMouseEnter(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSELEAVE:
			mouseLDown = false;
			mouseRDown = false;
//			onMouseLeave(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSEMOVE:
//			onMouseMove(HIWORD(param1), LOWORD(param1));
			break;
		case MSG_MOUSELDOWN:
			if( !in_focus )
				controller->set_focus(this);
			mouseLDown = true;
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

}