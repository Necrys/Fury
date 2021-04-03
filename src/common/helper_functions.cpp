#include <vector>

#include "helper_functions.h"
#include "system_main.h"
#include "sys_vars.h"
#include "system_interfaces.h"

using namespace std;
//-----------------------------------------------------------------------------

struct rect_def
{
	hgeRect	rect;
	uint32  color;
};
//-----------------------------------------------------------------------------

bool	fade_in;
float	fade_timer = 0.0f;
float	fade_time = 0.0f;

vector<rect_def> deferred_rects;
//-----------------------------------------------------------------------------

int getSystemPath( char* dest, const size_t& size )
{
	if( !dest )
		return -1;
	if( size == 0 )
		return -2;

	GetModuleFileName(0, dest, (uint32)size);

	for( size_t i = strlen(dest) - 1; i > 0; i-- )
	{
		char c = dest[i];
		dest[i] = 0;
		if (c == '\\' || c == '/')
			break;
	}

	return 0;
}
//-----------------------------------------------------------------------------

void initFullscreenQuad()
{
	fullscreenQuad.v[0].x = 0.0f;
	fullscreenQuad.v[0].y = 0.0f;
	fullscreenQuad.v[1].x = (float)screen_width;
	fullscreenQuad.v[1].y = 0.0f;
	fullscreenQuad.v[2].x = (float)screen_width;
	fullscreenQuad.v[2].y = (float)screen_height;
	fullscreenQuad.v[3].x = 0.0f;
	fullscreenQuad.v[3].y = (float)screen_height;
	fullscreenQuad.v[0].col = 0xFF000000;
	fullscreenQuad.v[1].col = 0xFF000000;
	fullscreenQuad.v[2].col = 0xFF000000;
	fullscreenQuad.v[3].col = 0xFF000000;
	fullscreenQuad.tex = 0;
	fullscreenQuad.blend = BLEND_DEFAULT;
}
//-----------------------------------------------------------------------------

void fadeOut( const float& timer )
{
	fade_in = false;
	fade_time = fade_timer = timer;
}
//-----------------------------------------------------------------------------

void fadeIn( const float& timer )
{
	fade_in = true;
	fade_time = fade_timer = timer;
}
//-----------------------------------------------------------------------------

void fadeUpdate( const float& delta )
{
	if( fade_timer <= 0.0f )
		return;

	fade_timer -= delta;
	if( fade_timer < 0.0f )
		fade_timer = 0.0f;

	float d = (fade_timer / fade_time);
	if( !fade_in )
		d = 1.0f - d;
	d *= 255;

	uint32 dwAlpha = (((uint32)d))<<24;

	fullscreenQuad.v[0].col = dwAlpha;
	fullscreenQuad.v[1].col = dwAlpha;
	fullscreenQuad.v[2].col = dwAlpha;
	fullscreenQuad.v[3].col = dwAlpha;
}
//-----------------------------------------------------------------------------

void render_anim( hgeAnimation* animation, const int32& x, const int32& y, const float& rot )
{
	assert(animation);

	animation->RenderEx((float)x, (float)y, rot);
}
//-----------------------------------------------------------------------------

void init_working_dir()
{
	wchar_t path[512], drive[10], dir[MAX_PATH], name[MAX_PATH], ext[128];
	GetModuleFileNameW( 0, path, 512 );
	_wsplitpath( path, drive, dir, name, ext );
	swprintf( path, 512, L"%s%s", drive, dir );
	if( !SetCurrentDirectoryW( path ) )
	{
		ASSERT( !"Could not set exe directory!" );
	}
}
//-----------------------------------------------------------------------------

void render_rect( hgeRect& rect, uint32 color )
{
	ASSERT(hge);

	hge->Gfx_RenderLine( rect.x1, rect.y1, rect.x2, rect.y1, color );
	hge->Gfx_RenderLine( rect.x2, rect.y1, rect.x2, rect.y2, color );
	hge->Gfx_RenderLine( rect.x2, rect.y2, rect.x1, rect.y2, color );
	hge->Gfx_RenderLine( rect.x1, rect.y2, rect.x1, rect.y1, color );
}
//-----------------------------------------------------------------------------

void render_rect_deferred( hgeRect& rect, uint32 color )
{
	rect_def def = {rect, color};
	deferred_rects.push_back( def );
}
//-----------------------------------------------------------------------------

void render_deferred()
{
	for( uint32 i=0; i<deferred_rects.size(); i++ )
	{
		render_rect( deferred_rects[i].rect, deferred_rects[i].color );
	}
	deferred_rects.clear();
}
//-----------------------------------------------------------------------------