#pragma once

#include <hgesprite.h>

//-----------------------------------------------------------------------------

class BackgroundRenderer
{
public:

	BackgroundRenderer();
	~BackgroundRenderer();

	hgeSprite*	image;

	void render();
};
//-----------------------------------------------------------------------------