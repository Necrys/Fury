#include "ui_imagelist.h"
#include "ui_il_element.h"
#include "ui_imagebutton.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "helper_functions.h"
#include "ui_skin.h"

using namespace std;
//-----------------------------------------------------------------------------

namespace gui
{

//-----------------------------------------------------------------------------

void up_btn_pressed( button* ctrl )
{
	image_list* list = dynamic_cast<image_list*>(ctrl->get_parent());
	if( list )
		list->scrollUp();
}
//-----------------------------------------------------------------------------

void down_btn_pressed( button* ctrl )
{
	image_list* list = dynamic_cast<image_list*>(ctrl->get_parent());
	if( list )
		list->scrollDown();
}
//-----------------------------------------------------------------------------

void pg_up_btn_pressed( button* ctrl )
{
	image_list* list = dynamic_cast<image_list*>(ctrl->get_parent());
	if( list )
		list->pageUp();
}
//-----------------------------------------------------------------------------

void pg_dn_btn_pressed( button* ctrl )
{
	image_list* list = dynamic_cast<image_list*>(ctrl->get_parent());
	if( list )
		list->pageDown();
}
//-----------------------------------------------------------------------------

image_list::image_list(system* _controller, _base* parent /* = 0 */): panel(_controller, parent)
{
	canScrollDown = true;

	selectionID = -1;
	ilBackColor = 0xFFFFFFFF;
	scroll		= 0;

	upButton = new image_button(_controller, this);
	upButton->caption = "";
	upButton->on_press = up_btn_pressed;

	downButton = new image_button(_controller, this);
	downButton->caption = "";
	downButton->on_press = down_btn_pressed;

	pgUpButton = new image_button(_controller, this);
	pgUpButton->caption = "";
	pgUpButton->on_press = pg_up_btn_pressed;

	pgDownButton = new image_button(_controller, this);
	pgDownButton->caption = "";
	pgDownButton->on_press = pg_dn_btn_pressed;

	if( skin )
	{
		pgUpButton->setImage( skin->pg_up->GetTexture() );
		pgDownButton->setImage( skin->pg_down->GetTexture() );
		upButton->setImage( skin->arrow_up->GetTexture() );
		downButton->setImage( skin->arrow_down->GetTexture() );
	}

	columns = 1;
	on_select = 0;
}
//-----------------------------------------------------------------------------

int image_list::getSelectionID() const
{
	return selectionID;
}
//-----------------------------------------------------------------------------

void image_list::setSelection(const int &id)
{
	if( id < -1 || id > (int)(elements.size() - 1) )
		return;

	if( selectionID != id )
	{
		selectionID = id;
		onSelectionChanged( selectionID );
		if( on_select )
		{
			if( selectionID == -1 )
				on_select( 0 );
			else
				on_select( elements[selectionID] );
		}
	}
}
//-----------------------------------------------------------------------------

void image_list::render()
{
	// render back
	if( !skin )
	{
		log_msg(COLOR_ERROR, "no skin set for panel, can't render");
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

	bool filtration = hge->System_GetState(HGE_TEXTUREFILTER);
	hge->System_SetState(HGE_TEXTUREFILTER, false);

	float vscale = skin->panel_border / skin->panel_lefttop->GetHeight();
	float hscale = skin->panel_border / skin->panel_lefttop->GetWidth();
	skin->panel_lefttop->SetColor(color);
	skin->panel_lefttop->RenderEx(bounds_global.x1 - skin->panel_border, bounds_global.y1 - skin->panel_border, 0.0f, hscale, vscale);

	vscale = skin->panel_border / skin->panel_top->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->panel_top->GetWidth();
	skin->panel_top->SetColor(color);
	skin->panel_top->RenderEx(bounds_global.x1, bounds_global.y1 - skin->panel_border, 0.0f, hscale, vscale);

	vscale = skin->panel_border / skin->panel_righttop->GetHeight();
	hscale = skin->panel_border / skin->panel_righttop->GetWidth();
	skin->panel_righttop->SetColor(color);
	skin->panel_righttop->RenderEx(bounds_global.x2, bounds_global.y1 - skin->panel_border, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->panel_right->GetHeight();
	hscale = skin->panel_border / skin->panel_right->GetWidth();
	skin->panel_right->SetColor(color);
	skin->panel_right->RenderEx(bounds_global.x2, bounds_global.y1, 0.0f, hscale, vscale);

	vscale = skin->panel_border / skin->panel_rightbottom->GetHeight();
	hscale = skin->panel_border / skin->panel_rightbottom->GetWidth();
	skin->panel_rightbottom->SetColor(color);
	skin->panel_rightbottom->RenderEx(bounds_global.x2, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = skin->panel_border / skin->panel_bottom->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->panel_bottom->GetWidth();
	skin->panel_bottom->SetColor(color);
	skin->panel_bottom->RenderEx(bounds_global.x1, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = skin->panel_border / skin->panel_leftbottom->GetHeight();
	hscale = skin->panel_border / skin->panel_leftbottom->GetWidth();
	skin->panel_leftbottom->SetColor(color);
	skin->panel_leftbottom->RenderEx(bounds_global.x1 - skin->panel_border, bounds_global.y2, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->panel_left->GetHeight();
	hscale = skin->panel_border / skin->panel_left->GetWidth();
	skin->panel_left->SetColor(color);
	skin->panel_left->RenderEx(bounds_global.x1 - skin->panel_border, bounds_global.y1, 0.0f, hscale, vscale);

	vscale = (bounds_global.y2 - bounds_global.y1) / skin->panel_client->GetHeight();
	hscale = (bounds_global.x2 - bounds_global.x1) / skin->panel_client->GetWidth();
	skin->panel_client->SetColor(color);
	skin->panel_client->RenderEx(bounds_global.x1, bounds_global.y1, 0.0f, hscale, vscale);

	// draw items
	for( uint32 i=0; i<elements.size(); i++ )
		elements[i]->set_visibility(false);

	uint32 width = (uint32)(bounds.x2 - bounds.x1);
	uint32 height = (uint32)(bounds.y2 - bounds.y1) - 12 - 24 * 2;
	uint32 vline_width = 2;
	uint32 hline_width = 2;
	uint32 vline_count = 2 + (columns - 1);
	uint32 el_width = (width - vline_count * vline_width) / columns;
	uint32 yshift = hline_width + 12 + 24 * 2;
	bool last_item = false;
	pageSize = 0;
	for( uint32 i=scroll; i<elements.size(); i+=columns )
	{
		uint32 xshift = vline_width;
		uint32 el_height = 0;
		vector<image_list_item*> row;
		for( uint32 j=0; j<columns; j++ )
		{
			row.push_back(elements[i+j]);
			el_height = max( el_height, elements[i+j]->getHeight() );
			if( (i+j+1) >= elements.size() )
			{
				last_item = true;
				break;
			}
		}
		if( yshift + el_height > height )
			break;
		for( uint32 j=0; j<row.size(); j++ )
		{
			row[j]->bounds.x1 = (float)xshift;
			row[j]->bounds.x2 = (float)(xshift + el_width);
			row[j]->bounds.y1 = (float)yshift;
			row[j]->bounds.y2 = (float)(yshift + el_height);
			row[j]->set_visibility(true);
			row[j]->render();
			xshift += (el_width + vline_width);
			pageSize++;
		}
		yshift += (el_height + hline_width);
		el_height = 0;
	}
	canScrollDown = !last_item;

	renderButtons();

	hge->System_SetState(HGE_TEXTUREFILTER, filtration);
}
//-----------------------------------------------------------------------------

void image_list::renderButtons()
{
	unsigned border = (24 < (bounds_global.x2 - bounds_global.x1) - 8.0f) ? (unsigned)((bounds_global.x2 - bounds_global.x1) - 24) / 2 : 2;

	pgUpButton->bounds.y1 = 4.0f;
	pgUpButton->bounds.y2 = 24 + 4.0f;
	pgUpButton->bounds.x1 = (float)border;
	pgUpButton->bounds.x2 = bounds_global.x2 - bounds_global.x1 - border;
	pgUpButton->render();

	upButton->bounds.y1 = 8.0f + 24;
	upButton->bounds.y2 = 24 * 2.0f + 8.0f;
	upButton->bounds.x1 = (float)border;
	upButton->bounds.x2 = bounds_global.x2 - bounds_global.x1 - border;
	upButton->render();

	downButton->bounds.y1 = bounds_global.y2 - bounds_global.y1 - 24 * 2 - 8.0f;
	downButton->bounds.y2 = bounds_global.y2 - bounds_global.y1 - 24 - 8.0f;
	downButton->bounds.x1 = (float)border;
	downButton->bounds.x2 = bounds_global.x2 - bounds_global.x1 - border;
	downButton->render();

	pgDownButton->bounds.y1 = bounds_global.y2 - bounds_global.y1 - 24 - 4.0f;
	pgDownButton->bounds.y2 = bounds_global.y2 - bounds_global.y1 - 4.0f;
	pgDownButton->bounds.x1 = (float)border;
	pgDownButton->bounds.x2 = bounds_global.x2 - bounds_global.x1 - border;
	pgDownButton->render();
}
//-----------------------------------------------------------------------------

image_list_item* image_list::insertElement(const std::string& image, 
											   const std::string& text, 
											   const float& x, 
											   const float& y, 
											   const float& w, 
											   const float& h, 
											   const float& imageScale,
											   const uint32& flags )
{
	image_list_item* element = new image_list_item(controller, this, (unsigned)elements.size());
	element->text = text;
	element->setImage(image, x, y, w, h, imageScale, flags);
	elements.insert( elements.end(), element );
	element->bounds = hgeRect(2.0f, 0.0f, (bounds.x2 - bounds.x1)-2.0f, 2.0f);
	return element;
}
//-----------------------------------------------------------------------------

image_list_item* image_list::insertElement(	const hgeSprite* image, 
											const std::string& text,
											const float& imageScale )
{
	image_list_item* element = new image_list_item(controller, this, (unsigned)elements.size());
	element->text = text;
	element->setImage( image, imageScale );
	elements.insert( elements.end(), element );
	element->bounds = hgeRect(2.0f, 0.0f, (bounds.x2 - bounds.x1)-2.0f, 2.0f);
	return element;
}
//-----------------------------------------------------------------------------

bool image_list::handle_event( uint32 msg, uint32 param1, uint32 param2 )
{
	list<_base*>::iterator it = childs.begin();
	while( it != childs.end() )
	{
		_base* ctrl = *it;

		if( ctrl->is_visible() )
		{
			bool mousetest = ctrl->bounds_global.TestPoint(HIWORD(param1), LOWORD(param1));

			switch( msg )
			{
			case MSG_MOUSEENTER:
			case MSG_MOUSELEAVE:
			case MSG_MOUSEMOVE:
				if( param1 != param2 )
				{
					bool lasttest = ctrl->bounds_global.TestPoint(HIWORD(param2), LOWORD(param2));

					if( lasttest && !mousetest )
						ctrl->handle_event(MSG_MOUSELEAVE, param1, param2);

					if( !lasttest && mousetest )
						ctrl->handle_event(MSG_MOUSEENTER, param1, param2);

					if( lasttest && mousetest )
						ctrl->handle_event(MSG_MOUSEMOVE, param1, param2);
				}
				break;
			case MSG_MOUSELDOWN:
			case MSG_MOUSELUP:
			case MSG_MOUSERDOWN:
			case MSG_MOUSERUP:
				if( mousetest )
				{
					ctrl->handle_event(msg, param1, param2);
					return true;
				}
				break;
			}
		}

		if( ctrl->is_dead() )
		{
			delete ctrl;
			it = childs.erase(it);
		}
		else
		{
			it++;
		}
	}

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
		case MSG_CLOSE:
//			this->onClose();
			this->dead = true;
			break;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

void image_list::scrollUp()
{
	if( scroll > 0 )
		scroll-=columns;
	if( scroll < 0 )
		scroll = 0;
}
//-----------------------------------------------------------------------------

void image_list::scrollDown()
{
	if( !canScrollDown )
		return;

	if( (unsigned)scroll < elements.size()-1 && canScrollDown )
		scroll+=columns;
	if( (unsigned)scroll > elements.size() - 1 )
		scroll = (int)elements.size() - 1 - pageSize;
}
//-----------------------------------------------------------------------------

void image_list::pageUp()
{
	scroll -= pageSize;
	if( scroll < 0 )
		scroll = 0;
}
//-----------------------------------------------------------------------------

void image_list::pageDown()
{
	if( !canScrollDown )
		return;

	scroll += pageSize;
	if( (unsigned)scroll > elements.size() - 1 )
		scroll = (int)elements.size() - 1 - pageSize;
	if( (unsigned)scroll < 0 )
		scroll = 0;
}
//-----------------------------------------------------------------------------

void* image_list::getSelectionUserData() const
{
	if( selectionID < -1 || selectionID >= (int)elements.size() )
		return 0;

	return elements[selectionID]->user_data;
}
//-----------------------------------------------------------------------------

image_list_item*	image_list::getSelection()
{
	if( selectionID < -1 || selectionID >= (int)elements.size() )
		return 0;

	return elements[selectionID];
}
//-----------------------------------------------------------------------------

void image_list::removeElement( const unsigned& id )
{
	if( id >= elements.size() )
		return;

	controller->control_remove(elements[id]);
	for( unsigned i = id; i < elements.size()-1; i++ )
	{
		elements[i] = elements[i+1];
		elements[i]->_id = i;
	}
	elements.pop_back();
}
//-----------------------------------------------------------------------------

unsigned image_list::size() const
{
	return (unsigned)elements.size();
}
//-----------------------------------------------------------------------------

image_list_item*& image_list::operator[] (int id)
{
	assert( (unsigned)id < elements.size() );
	return elements[id];
}
//-----------------------------------------------------------------------------

void image_list::clear()
{
	for( unsigned i = 0; i < elements.size(); i++ )
	{
		controller->control_remove(elements[i]);
	}
	elements.clear();
	selectionID = -1;
	scroll = 0;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

}