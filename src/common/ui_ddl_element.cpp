#include <hgefont.h>
#include "ui_ddl_element.h"
#include "ui_dropdownlist.h"
#include "ui_main.h"
#include "system_interfaces.h"
#include "helper_functions.h"

using namespace std;

//-----------------------------------------------------------------------------

namespace gui
{

drop_list_item::drop_list_item(system* _controller, _base* parent): label(_controller, parent)
{
	basicColor = 0x00FFFFFF;
	selectedColor = 0xFFC0C0C0;
	mouseOverColor = 0xFFDCDCDC;
	font_color = 0xFF000000;
	id = -1;
	mouseOver = false;
	listController = 0;
}
//-----------------------------------------------------------------------------

drop_list_item::~drop_list_item()
{
	
};
//-----------------------------------------------------------------------------

void drop_list_item::render()
{
	if( id < 0 )
		return;

	if( !listController )
		return;

	back_color = basicColor;
	if( mouseOver )
		back_color = mouseOverColor;
	if( listController->getSelectionID() == id )
		back_color = selectedColor;

	label::render();
}
//-----------------------------------------------------------------------------

bool drop_list_item::handle_event( uint32 msg, uint32 param1, uint32 param2 )
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
			listController->setSelection(id);
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