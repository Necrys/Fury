
#include "system_interfaces.h"
#include "helper_xmlcache.h"
#include "str_helpers.h"

using namespace std;
using namespace stdext;
using namespace pugi;

/* constructor */
XMLCache::XMLCache()
{
	xmlDoc_map.clear();
}

/* destructor */
XMLCache::~XMLCache()
{
	this->clear();
}

/* clear */
void XMLCache::clear()
{
	unordered_map<string, xml_document*>::iterator it = xmlDoc_map.begin();
	while( it != xmlDoc_map.end() )
	{
		xml_document* doc = it->second;

		if(doc)
			delete doc;

		doc = 0;

		it++;
	}
}

/* get document */
xml_document* XMLCache::getDocument(const std::string &doc_path)
{
	xml_document* result = 0;

	if( xmlDoc_map[doc_path] )
	{
		result = xmlDoc_map[doc_path];
	}
	else
	{
		result = new xml_document();
		if(result)
		{
			xml_parse_result res = result->load_file( doc_path.c_str() );
			
			if(!res)
			{
				log_msg( COLOR_ERROR, res.description() );
			}
			else
			{
				xmlDoc_map[doc_path] = result;
			}
		}
		
	}

	return result;
}