#include "background.h"
#include "system_interfaces.h"
#include "system_paths.h"

//-----------------------------------------------------------------------------

BackgroundRenderer::BackgroundRenderer()
{
	HTEXTURE texture = textureManager->getTexture( (Path::gfxcommon + "background.jpg").c_str() );
	if( !texture )
		return;

	image = new hgeSprite(texture, 0, 0, 1024, 768);
}
//-----------------------------------------------------------------------------

BackgroundRenderer::~BackgroundRenderer()
{
	if( image )
		delete image;
}
//-----------------------------------------------------------------------------

void BackgroundRenderer::render()
{
	if( image )
	{
		image->RenderEx(0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	}
}