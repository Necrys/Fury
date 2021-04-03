#pragma once

#include "hgeanim.h"
#include "types.h"
//-----------------------------------------------------------------------------

int getSystemPath( char* dest, const size_t& size );
//-----------------------------------------------------------------------------

void initFullscreenQuad();
//-----------------------------------------------------------------------------

void fadeOut( const float& timer );
//-----------------------------------------------------------------------------

void fadeIn( const float& timer );
//-----------------------------------------------------------------------------

void fadeUpdate( const float& delta );
//-----------------------------------------------------------------------------

void render_anim( hgeAnimation* animation, const int32& x, const int32& y, const float& rot = 0.0f );
//-----------------------------------------------------------------------------

void init_working_dir();
//-----------------------------------------------------------------------------

void render_rect( hgeRect& rect, uint32 color = 0xff00ff00 );
//-----------------------------------------------------------------------------

void render_rect_deferred( hgeRect& rect, uint32 color = 0xff00ff00 );
//-----------------------------------------------------------------------------

void render_deferred();
//-----------------------------------------------------------------------------