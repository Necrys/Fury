#pragma once

#include <pugixml.hpp>
#include <string>
#include "types.h"
//-----------------------------------------------------------------------------

class hgeAnimation;
class hgeFont;
//-----------------------------------------------------------------------------

namespace gui
{

enum BUTTONSTATE
{
	MOUSEOVER = 0,
	PRESSED	= 1,
	UNPRESSED = 2,
};
//-----------------------------------------------------------------------------

class UISkin
{
public:
	UISkin( const std::string& filePath );
	~UISkin();

	bool loaded() const { return _loaded; };

	hgeAnimation*	cursor;
	hgeFont*		font;
	uint32			common_border;

	// common sprites
	hgeAnimation*	arrow_up;
	hgeAnimation*	arrow_down;
	hgeAnimation*	arrow_left;
	hgeAnimation*	arrow_right;
	hgeAnimation*	pg_down;
	hgeAnimation*	pg_up;
	hgeAnimation*	check_icon;

	// panel sprites
	hgeAnimation*	panel_lefttop;
	hgeAnimation*	panel_top;
	hgeAnimation*	panel_righttop;
	hgeAnimation*	panel_right;
	hgeAnimation*	panel_rightbottom;
	hgeAnimation*	panel_bottom;
	hgeAnimation*	panel_leftbottom;
	hgeAnimation*	panel_left;
	hgeAnimation*	panel_client;
	uint32			panel_border;

	// window sprites
	hgeAnimation*	window_lefttop;
	hgeAnimation*	window_top;
	hgeAnimation*	window_righttop;
	hgeAnimation*	window_right;
	hgeAnimation*	window_rightbottom;
	hgeAnimation*	window_bottom;
	hgeAnimation*	window_leftbottom;
	hgeAnimation*	window_left;
	hgeAnimation*	window_client;
	hgeAnimation*	window_header;
	uint32			window_header_width;
	uint32			window_border;

	// button sprites
	hgeAnimation*	button_lefttop[3];
	hgeAnimation*	button_top[3];
	hgeAnimation*	button_righttop[3];
	hgeAnimation*	button_right[3];
	hgeAnimation*	button_rightbottom[3];
	hgeAnimation*	button_bottom[3];
	hgeAnimation*	button_leftbottom[3];
	hgeAnimation*	button_left[3];
	hgeAnimation*	button_client[3];
	uint32			button_border;

private:

	int32 parseDoc();
	int32 parsePanel( const pugi::xml_node& node );
	int32 parseWindow( const pugi::xml_node& node );
	int32 parseButton( const pugi::xml_node& node );
	int32 parseCommon( const pugi::xml_node& node );
	int32 parseSpriteNode( const pugi::xml_node& node, hgeAnimation** sprite );
	int32 parseButtonSpriteNode( const pugi::xml_node& node, hgeAnimation** pressed_spr, 
								hgeAnimation** unpressed_spr, hgeAnimation** mouseover_spr );

	pugi::xml_document	skin_doc;
	bool				_loaded;
};
//-----------------------------------------------------------------------------

}