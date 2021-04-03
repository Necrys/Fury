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

#pragma once

#include <vector>
#include <string>
#include <pugixml.hpp>
#include <map>
#include "data_dynamicobject_base.h"

class hgeAnimation;

//-----------------------------------------------------------------------------

struct Tile
{
	hgeAnimation*	anim;
	std::string	name;
	std::string	texture;
	float			iconScale;
	float			scale;
	unsigned		id;
	bool			vflip;
	bool			hflip;

	std::map< std::string, std::string >	params;
	DYNAMIC_TYPE	dynamicType;
};
//-----------------------------------------------------------------------------

class Tileset
{
public:
	Tileset();
	~Tileset();

	std::string file;

	std::vector<Tile*>	statics;
	std::vector<Tile*>	backgrounds;
	std::vector<Tile*>	foregrounds;
	std::vector<Tile*>	dynamics;

	bool loadTileset( const std::string& file );

	Tile* getTileByID( const uint32& id, const uint32& obj_type = 4 );	// 4 - static tile
	Tile* getStaticByID( const unsigned& id );
	Tile* getBackByID( const unsigned& id );
	Tile* getForeByID( const unsigned& id );
	Tile* getDynamicByID( const unsigned& id, const DYNAMIC_TYPE& type );

private:
	void clear();

	Tile* parseTile( const pugi::xml_node& node, const std::string& set_name, const std::string& base_texture );
};
//-----------------------------------------------------------------------------
