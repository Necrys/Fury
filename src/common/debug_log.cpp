
#include <list>
#include "hgefont.h"
#include "debug_log.h"
#include "sys_vars.h"
#include "str_helpers.h"
#include "system_interfaces.h"

using namespace std;
//-----------------------------------------------------------------------------

struct msg_struct
{
	string	text;
	uint32	color;
	float	timer;
};
//-----------------------------------------------------------------------------

list<msg_struct>	msg_list;

char				wbuffer[1024];
char				buffer[1024];
//-----------------------------------------------------------------------------

void log_msg( const uint32& color, const char* format, ... )
{
	assert(hge);

	memset(wbuffer, 0, sizeof(char) * 1024);

	va_list ap;
	va_start(ap, format);
	vsprintf_s(wbuffer, format, ap);
	va_end(ap);

	hge->System_Log( wbuffer );

	msg_struct new_msg;
	new_msg.color = color;
	new_msg.text = wbuffer;
	new_msg.timer = log_msg_lifetime;
	msg_list.push_front(new_msg);
}
//-----------------------------------------------------------------------------

void log_msg( const char* format, ... )
{
	assert(hge);

	memset(wbuffer, 0, sizeof(char) * 1024);

	va_list ap;
	va_start(ap, format);
	vsprintf_s(wbuffer, format, ap);
	va_end(ap);

	hge->System_Log( wbuffer );

	msg_struct new_msg;
	new_msg.color = COLOR_MESSAGE;
	new_msg.text = wbuffer;
	new_msg.timer = log_msg_lifetime;
	msg_list.push_front(new_msg);
}
//-----------------------------------------------------------------------------

void log_update( const float& delta )
{
	list<msg_struct>::iterator it = msg_list.begin();
	while( it != msg_list.end() )
	{
		(*it).timer -= delta;
		if( (*it).timer <= 0.0f )
			it = msg_list.erase( it );
		else
			it++;
	}
}
//-----------------------------------------------------------------------------

void log_render()
{
	assert(hge);
	assert(default_font);

	hge->Gfx_SetTransform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	uint32 counter = 0;

	list<msg_struct>::iterator it = msg_list.begin();
	while( it != msg_list.end() && counter < log_msg_count )
	{
		if( (*it).timer < log_msg_fadetime )
		{
			float alpha = ((*it).timer / log_msg_fadetime) * 255;
			uint32 dwAlpha = (((uint32)alpha))<<24;
			uint32 color = ((*it).color & 0x00FFFFFF) + dwAlpha;
			default_font->SetColor(color);
		}
		else
		{
			default_font->SetColor((*it).color);
		}

		default_font->printf((float)5, (float)(screen_height - ((counter + 1) * default_font->GetHeight())), HGETEXT_LEFT, "%s", (*it).text.c_str());

		it++;
		counter++;
	}
}
//-----------------------------------------------------------------------------
