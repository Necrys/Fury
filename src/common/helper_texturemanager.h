#pragma once

#include <unordered_map>
#include <string>
#include <hge.h>


// class definition
class TextureManager
{
private:
	// hge interface
	HGE*	hge;

public:
	// map, for storing texture data
	std::unordered_map<std::string, HTEXTURE>	textures_map;

	/* constructor */
	TextureManager();
	~TextureManager();

	/* get texture from manager */
	HTEXTURE	getTexture(const std::string& texture_path);

	/* clean all textures */
	void		cleanup();
};