#include <hgeanim.h>
#include <hgefont.h>
#include "ui_skin.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "str_helpers.h"

using namespace std;
using namespace pugi;
//-----------------------------------------------------------------------------

namespace gui
{

#define READ_NODE( node, name, sprite ) { \
xml_node name_node = node.child(name); \
if( !name_node ) \
	return -1; \
if( parseSpriteNode(name_node, &sprite) ) \
	return -2; \
}
//-----------------------------------------------------------------------------

#define READ_BUTTON_NODE( node, name, pressed, unpressed, mouseover ) { \
xml_node name_node = node.child(name); \
if( !name_node ) \
	return -1; \
if( parseButtonSpriteNode(name_node, &pressed, &unpressed, &mouseover) ) \
	return -2; \
}

//-----------------------------------------------------------------------------

UISkin::UISkin( const string& filePath )
{
	_loaded = false;
	cursor = 0;
	font = 0;
	panel_lefttop = 0;
	panel_top = 0;
	panel_righttop = 0;
	panel_right = 0;
	panel_rightbottom = 0;
	panel_bottom = 0;
	panel_leftbottom = 0;
	panel_left = 0;
	panel_client = 0;

	xml_parse_result res = skin_doc.load_file( filePath.c_str() );
	if( !res )
		return;

	if( parseDoc() )
		return;

	_loaded = true;
}
//-----------------------------------------------------------------------------

UISkin::~UISkin()
{
	if( cursor )
		delete cursor;

	if( font )
		delete font;
}
//-----------------------------------------------------------------------------

int32 UISkin::parseDoc()
{
	assert(skin_doc);

	xml_node skin_node = skin_doc.child("skin");
	if( !skin_node )
		return -1;

	xml_attribute common_border_attr = skin_node.attribute("border");
	if( common_border_attr )
		common_border = common_border_attr.as_uint();
	else
		common_border = 1;

	// font
	xml_node font_node = skin_node.child("font");
	if( !font_node )
		return -2;
	xml_attribute font_file_attr = font_node.attribute("file");
	if( !font_file_attr )
		return -3;
	font = new hgeFont( (Path::fonts + font_file_attr.value()).c_str() );

	// cursor
	xml_node cursor_node = skin_node.child("cursor");
	if( !cursor_node )
		return -4;
	if( parseSpriteNode(cursor_node, &cursor) )
		return -5;

	// panel
	xml_node panel_node = skin_node.child("panel");
	if( !panel_node )
		return -6;
	if( parsePanel( panel_node ) )
		return -7;

	// window
	xml_node window_node = skin_node.child("window");
	if( !window_node )
		return -8;
	if( parseWindow( window_node ) )
		return -9;

	// button
	xml_node button_node = skin_node.child("button");
	if( !button_node )
		return -10;
	if( parseButton( button_node ) )
		return -11;

	// common
	xml_node common_node = skin_node.child("common");
	if( !common_node )
		return -12;
	if( parseCommon( common_node ) )
		return -13;

	return 0;
}
//-----------------------------------------------------------------------------

int32 UISkin::parseSpriteNode( const pugi::xml_node& node, hgeAnimation** sprite )
{
	assert(node);

	xml_attribute texture_attr;
	xml_node t = node;
	while( !texture_attr )
	{
		if( t )
		{
			texture_attr = t.attribute("texture");
			t = t.parent();
		}
		else
		{
			break;
		}
	}
	if( !texture_attr )
		return -1;

	xml_attribute x_attr = node.attribute("x");
	xml_attribute y_attr = node.attribute("y");
	xml_attribute w_attr = node.attribute("w");
	xml_attribute h_attr = node.attribute("h");
	xml_attribute frames_attr = node.attribute("frames");
	xml_attribute fps_attr = node.attribute("fps");

	HTEXTURE texture = textureManager->getTexture( Path::uigfx + texture_attr.value() );
	uint32	x = 0;
	uint32	y = 0;
	uint32	w = hge->Texture_GetWidth(texture);
	uint32	h = hge->Texture_GetHeight(texture);
	uint32	frames = 1;
	float	fps = 1.0f;

	if( x_attr )
		x = x_attr.as_uint();

	if( y_attr )
		y = y_attr.as_uint();

	if( w_attr )
		w = w_attr.as_uint();

	if( h_attr )
		h = h_attr.as_uint();

	if( frames_attr )
		frames = frames_attr.as_uint();

	if( fps_attr )
		fps = x_attr.as_float();

	*sprite = new hgeAnimation(texture, frames, fps, (float)x, (float)y, (float)w, (float)h);

	return 0;
}
//-----------------------------------------------------------------------------

int32 UISkin::parseButtonSpriteNode( const pugi::xml_node& node, hgeAnimation** pressed_spr, 
								hgeAnimation** unpressed_spr, hgeAnimation** mouseover_spr )
{
	assert(node);

	xml_node unpressed_node = node.child("unpressed");
	if( !unpressed_node )
		return -1;

	if( parseSpriteNode(unpressed_node, unpressed_spr) )
		return -2;

	xml_node pressed_node = node.child("pressed");
	if( !pressed_node )
		return -3;

	if( parseSpriteNode(pressed_node, pressed_spr) )
		return -4;

	xml_node mouseover_node = node.child("mouseover");
	if( !mouseover_node )
		return -5;

	if( parseSpriteNode(mouseover_node, mouseover_spr) )
		return -6;

	return 0;
}
//-----------------------------------------------------------------------------

int32 UISkin::parsePanel( const pugi::xml_node& node )
{
	xml_attribute panel_border_attr = node.attribute("border");
	if( panel_border_attr )
		panel_border = panel_border_attr.as_uint();
	else
		panel_border = common_border;

	READ_NODE( node, "lefttop", panel_lefttop );
	READ_NODE( node, "top", panel_top );
	READ_NODE( node, "righttop", panel_righttop );
	READ_NODE( node, "right", panel_right );
	READ_NODE( node, "rightbottom", panel_rightbottom );
	READ_NODE( node, "bottom", panel_bottom );
	READ_NODE( node, "leftbottom", panel_leftbottom );
	READ_NODE( node, "left", panel_left );
	READ_NODE( node, "client", panel_client );

	return 0;
}
//-----------------------------------------------------------------------------

int32 UISkin::parseWindow( const pugi::xml_node& node )
{
	xml_attribute border_attr = node.attribute("border");
	if( border_attr )
		window_border = border_attr.as_uint();
	else
		window_border = common_border;

	READ_NODE( node, "lefttop", window_lefttop );
	READ_NODE( node, "top", window_top );
	READ_NODE( node, "righttop", window_righttop );
	READ_NODE( node, "right", window_right );
	READ_NODE( node, "rightbottom", window_rightbottom );
	READ_NODE( node, "bottom", window_bottom );
	READ_NODE( node, "leftbottom", window_leftbottom );
	READ_NODE( node, "left", window_left );
	READ_NODE( node, "client", window_client );
	READ_NODE( node, "header", window_header );

	xml_attribute header_width_attr = node.child("header").attribute("width");
	if( !header_width_attr )
		return -1;
	else
		window_header_width = header_width_attr.as_uint();

	return 0;
}
//-----------------------------------------------------------------------------

int32 UISkin::parseButton( const pugi::xml_node& node )
{
	xml_attribute border_attr = node.attribute("border");
	if( border_attr )
		button_border = border_attr.as_uint();
	else
		button_border = common_border;

	READ_BUTTON_NODE( node, "lefttop", button_lefttop[PRESSED], button_lefttop[UNPRESSED], button_lefttop[MOUSEOVER] );
	READ_BUTTON_NODE( node, "top", button_top[PRESSED], button_top[UNPRESSED], button_top[MOUSEOVER] );
	READ_BUTTON_NODE( node, "righttop", button_righttop[PRESSED], button_righttop[UNPRESSED], button_righttop[MOUSEOVER] );
	READ_BUTTON_NODE( node, "right", button_right[PRESSED], button_right[UNPRESSED], button_right[MOUSEOVER] );
	READ_BUTTON_NODE( node, "rightbottom", button_rightbottom[PRESSED], button_rightbottom[UNPRESSED], button_rightbottom[MOUSEOVER] );
	READ_BUTTON_NODE( node, "bottom", button_bottom[PRESSED], button_bottom[UNPRESSED], button_bottom[MOUSEOVER] );
	READ_BUTTON_NODE( node, "leftbottom", button_leftbottom[PRESSED], button_leftbottom[UNPRESSED], button_leftbottom[MOUSEOVER] );
	READ_BUTTON_NODE( node, "left", button_left[PRESSED], button_left[UNPRESSED], button_left[MOUSEOVER] );
	READ_BUTTON_NODE( node, "client", button_client[PRESSED], button_client[UNPRESSED], button_client[MOUSEOVER] );

	return 0;
}
//-----------------------------------------------------------------------------

int32 UISkin::parseCommon( const pugi::xml_node& node )
{
	READ_NODE( node, "arrowdown", arrow_down );
	READ_NODE( node, "arrowup", arrow_up );
	READ_NODE( node, "pgdown", pg_down);
	READ_NODE( node, "pgup", pg_up );
	READ_NODE( node, "checkicon", check_icon);

	return 0;
}
//-----------------------------------------------------------------------------

}