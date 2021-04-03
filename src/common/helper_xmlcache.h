/*=============================================================================

	The Fury

DESCRIPTION:
	XML cache class

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
27/08/2009	initial version
=============================================================================*/
#pragma once

#include <unordered_map>
#include <string>
#include <pugixml.hpp>

// class definition
class XMLCache
{
public:
	/* docs map */
	std::unordered_map<std::string, pugi::xml_document*>	xmlDoc_map;

	/* constructor */
	XMLCache();
	~XMLCache();

	/* get XML doc from cache */
	pugi::xml_document* getDocument(const std::string& doc_path);

	/* clear all docs */
	void clear();
};