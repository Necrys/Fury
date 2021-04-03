
#include "helper_texturemanager.h"
#include "system_interfaces.h"
#include "str_helpers.h"

using namespace std;

/* constructor */
TextureManager::TextureManager()
{
	hge = 0;
	textures_map.clear();
	hge = hgeCreate(HGE_VERSION);
}

/* destructor */
TextureManager::~TextureManager()
{
	/* delete textures */
	this->cleanup();
	if( hge )
		hge->Release();
}

/* get texture from manager */
HTEXTURE TextureManager::getTexture(const std::string &texture_path)
{
	/* search texture in map */

	HTEXTURE result = 0;

	if( textures_map[texture_path] != 0 )
	{
		result = textures_map[texture_path];
	}
	else
	{
		/* if texture not exists, create it */
		HTEXTURE newtex = 0;
		if(hge)
		{
			 newtex = hge->Texture_Load( texture_path.c_str() );
		}
		
		if( newtex )
		{
			textures_map[texture_path] = newtex;
			result = newtex;
		}
		else
		{
			log_msg("Не могу загрузить текстуру: %s", texture_path.c_str());
		}

	}

	return result;
}

/* clean all textures */
void TextureManager::cleanup()
{
	unordered_map<string, HTEXTURE>::iterator it = textures_map.begin();
	while( it != textures_map.end() )
	{
		HTEXTURE tex = it->second;

		if( hge )
			hge->Texture_Free(tex);
		it->second = 0;

		it++;
	}
}