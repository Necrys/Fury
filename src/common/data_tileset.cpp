/*=============================================================================

	The Fury

DESCRIPTION:
	tileset controller

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
17/05/2010	initial version
=============================================================================*/


#include <hgeanim.h>
#include "data_tileset.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "system_main.h"
#include "str_helpers.h"
#include "data_level.h"

using namespace std;
using namespace pugi;
//-----------------------------------------------------------------------------

Tileset::Tileset()
{

}
//-----------------------------------------------------------------------------

Tileset::~Tileset()
{
	this->clear();
}
//-----------------------------------------------------------------------------

bool Tileset::loadTileset(const std::string &_file)
{
	xml_document* doc = xmlCache->getDocument( appDirectory + "\\" + Path::tilesets + _file );
	if( doc )
	{
		statics.clear();
		backgrounds.clear();
		foregrounds.clear();
		dynamics.clear();

		xml_node tilesetNode = doc->child("tileset");
		if( !tilesetNode )
		{
			log_msg(COLOR_ERROR, "not wellformed tileset XML (no \"tileset\" node)");
			return false;
		}

		xml_attribute name_attr = tilesetNode.attribute("name");
		if( !name_attr )
		{
			log_msg(COLOR_ERROR, "not wellformed tileset XML (no \"tileset name\" attribute)");
			return false;
		}
		string _name = name_attr.value();

		xml_node staticNode = tilesetNode.child("static");
		if( !staticNode )
		{
			log_msg(COLOR_ERROR, "not wellformed tileset XML (no \"static\" node)");
			return false;
		}

		string globalStaticTexture;
		xml_attribute static_texture = staticNode.attribute("texture");
		if( static_texture )
			globalStaticTexture = static_texture.value();

		xml_node staticObj = staticNode.first_child();
		while( staticObj )
		{
			Tile* newT = parseTile(staticObj, _name, globalStaticTexture);
			if( newT )
				statics.insert( statics.end(), newT );

			staticObj = staticObj.next_sibling();
		}

		xml_node foregroundNode = tilesetNode.child("foreground");
		if( foregroundNode )
		{
			static_texture = foregroundNode.attribute("texture");
			if( static_texture )
				globalStaticTexture = static_texture.value();

			xml_node obj = foregroundNode.first_child();
			while( obj )
			{
				Tile* newT = parseTile(obj, _name, globalStaticTexture);
				if( newT )
					foregrounds.insert( foregrounds.end(), newT );

				obj = obj.next_sibling();
			}
		}

		xml_node backgroundNode = tilesetNode.child("background");
		if( backgroundNode )
		{
			static_texture = backgroundNode.attribute("texture");
			if( static_texture )
				globalStaticTexture = static_texture.value();

			xml_node obj = backgroundNode.first_child();
			while( obj )
			{
				Tile* newT = parseTile(obj, _name, globalStaticTexture);
				if( newT )
					backgrounds.insert( backgrounds.end(), newT );

				obj = obj.next_sibling();
			}
		}

		xml_node dynamicNode = tilesetNode.child("dynamic");
		if( dynamicNode )
		{
			static_texture = dynamicNode.attribute("texture");
			if( static_texture )
				globalStaticTexture = static_texture.value();

			xml_node obj = dynamicNode.first_child();
			while( obj )
			{
				Tile* newT = parseTile(obj, _name, globalStaticTexture);
				if( newT )
					dynamics.insert( dynamics.end(), newT );

				obj = obj.next_sibling();
			}
		}

		file = _name;
	}
	else
	{
		log_msg(COLOR_ERROR, "no tileset XML file");
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------

void Tileset::clear()
{
	
}
//-----------------------------------------------------------------------------

Tile* Tileset::getTileByID( const uint32& id, const uint32& obj_type )
{
	switch( obj_type )
	{
	case FLF_SECTION_STATIC:
		{
			return getStaticByID( id );
		}
		break;
	case FLF_SECTION_BACKGROUND:
		{
			return getBackByID( id );
		}
		break;
	case FLF_SECTION_FOREGROUND:
		{
			return getForeByID( id );
		}
		break;
	default:
		assert(0);
		return 0;
		break;
	}
}
//-----------------------------------------------------------------------------

Tile* Tileset::getStaticByID( const unsigned& id )
{
	for( unsigned i=0; i<statics.size(); i++ )
	{
		if( statics[i]->id == id )
			return statics[i];
	}
	return 0;
}
//-----------------------------------------------------------------------------

Tile* Tileset::getBackByID( const unsigned& id )
{
	for( unsigned i=0; i<backgrounds.size(); i++ )
	{
		if( backgrounds[i]->id == id )
			return backgrounds[i];
	}
	return 0;
}
//-----------------------------------------------------------------------------

Tile* Tileset::getForeByID( const unsigned& id )
{
	for( unsigned i=0; i<foregrounds.size(); i++ )
	{
		if( foregrounds[i]->id == id )
			return foregrounds[i];
	}
	return 0;
}
//-----------------------------------------------------------------------------

Tile* Tileset::getDynamicByID( const unsigned& id, const DYNAMIC_TYPE& type )
{
	for( unsigned i=0; i<dynamics.size(); i++ )
	{
		if( dynamics[i]->id == id && type == dynamics[i]->dynamicType )
			return dynamics[i];
	}
	return 0;
}
//-----------------------------------------------------------------------------

Tile* Tileset::parseTile(const xml_node& node, const string& set_name, const std::string& base_texture)
{
	HTEXTURE tex;
	unsigned x, y, w, h, nframes, id;
	float fps, scale, icon_scale;
	string spritename, texturePath;

	xml_attribute tex_attr = node.attribute("texture");
	if( !tex_attr )
	{
		if( base_texture == "" )
		{
			log_msg(COLOR_ERROR, "not wellformed tileset XML (no \"texture\" node)");
			return 0;
		}
		texturePath = base_texture;
	}
	else
	{
		texturePath = tex_attr.value();
	}
	texturePath = Path::gfxtiles + set_name + "\\" + texturePath;
	tex = textureManager->getTexture( texturePath );
	if( !tex )
	{
		log_msg(COLOR_ERROR, "no texture: %s",texturePath.c_str());
		return 0;
	}

	xml_attribute name_attr = node.attribute("name");
	if( name_attr )
		spritename = name_attr.value();
	else
		spritename = "default";

	xml_attribute x_attr = node.attribute("x");
	if( x_attr )
		x = x_attr.as_uint();
	else
		x = 0;

	xml_attribute y_attr = node.attribute("y");
	if( y_attr )
		y = y_attr.as_uint();
	else
		y = 0;

	xml_attribute w_attr = node.attribute("w");
	if( w_attr )
		w = w_attr.as_uint();
	else
		w = hge->Texture_GetWidth(tex);

	xml_attribute h_attr = node.attribute("h");
	if( h_attr )
		h = h_attr.as_uint();
	else
		h = hge->Texture_GetHeight(tex);

	xml_attribute nframes_attr = node.attribute("frames");
	if( nframes_attr )
		nframes = nframes_attr.as_uint();
	else
		nframes = 1;

	xml_attribute id_attr = node.attribute("id");
	if( !id_attr )
	{
		log_msg(COLOR_ERROR, "no id node");
		return 0;
	}
	id = id_attr.as_uint();

	xml_attribute fps_attr = node.attribute("fps");
	if( fps_attr )
		fps = fps_attr.as_float();
	else
		fps = (float)nframes;

	xml_attribute scale_attr = node.attribute("scale");
	if( scale_attr )
		scale = scale_attr.as_float();
	else
		scale = 1.0f;

	bool vflip = false;
	bool hflip = false;

	xml_attribute vflip_attr = node.attribute("vflip");
	if( vflip_attr )
		vflip = true;

	xml_attribute hflip_attr = node.attribute("hflip");
	if( hflip_attr )
		hflip = true;

	xml_attribute iconscale_attr = node.attribute("icon_scale");
	if( iconscale_attr )
		icon_scale = iconscale_attr.as_float();
	else
		icon_scale = scale;

	Tile* new_tile = new Tile();

	new_tile->vflip = vflip;
	new_tile->hflip = hflip;

	xml_node obj = node.first_child();
	while( obj )
	{
		string name = obj.name();
		new_tile->params[name] = obj.first_child().value();

		obj = obj.next_sibling();
	}

	new_tile->anim = new hgeAnimation(tex, nframes, fps, (float)x, (float)y, (float)w, (float)h);
	new_tile->anim->SetFlip( hflip, vflip );
	new_tile->name = spritename;
	new_tile->scale = scale;
	new_tile->texture = texturePath;
	new_tile->iconScale = icon_scale;
	new_tile->id = id;
	new_tile->dynamicType = DYNAMIC_UNKNOWN;

	if( string(node.name()) == "movable_box" )
		new_tile->dynamicType = DYNAMIC_MOVEABLE_BOX;
	else if( string(node.name()) == "destructable_box" )
		new_tile->dynamicType = DYNAMIC_DESTRUCTABLE;
	else if( string(node.name()) == "crunch_box" )
		new_tile->dynamicType = DYNAMIC_CRUNCH;
	else if( string(node.name()) == "point" )
		new_tile->dynamicType = DYNAMIC_POINT;
	else if( string(node.name()) == "lift" )
		new_tile->dynamicType = DYNAMIC_LIFT;
	else if( string(node.name()) == "door" )
		new_tile->dynamicType = DYNAMIC_DOOR;
	else if( string(node.name()) == "key" )
		new_tile->dynamicType = DYNAMIC_KEY;
	else if( string(node.name()) == "coin" )
		new_tile->dynamicType = DYNAMIC_COIN;
	else if( string(node.name()) == "trigger" )
		new_tile->dynamicType = DYNAMIC_TRIGGER;
	else if( string(node.name()) == "start_point" )
		new_tile->dynamicType = DYNAMIC_START_POINT;

	return new_tile;
}
//-----------------------------------------------------------------------------
