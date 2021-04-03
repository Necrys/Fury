#include <hgecolor.h>
#include <hgefont.h>
#include "system_interfaces.h"
#include "system_main.h"
#include "system_paths.h"
#include "helper_debugbox.h"
#include "helper_functions.h"
#include "sys_vars.h"

using namespace std;

/* constructor */
debugBox::debugBox()
{
	font = 0;
	options = 0;
	this->cleanup();

	font = new hgeFont( (Path::fonts + "verdana.fnt").c_str() );
}

/* destructor */
debugBox::~debugBox()
{
	this->cleanup();
}

/* clean all lists */
void debugBox::cleanup()
{
	messagesTimer.clear();
	messagesColor.clear();
	messagesList.clear();

	if( font )
		delete font;
	font = 0;
}

/* adding a new message */
void debugBox::addMessage( const string& message, const uint32& color)
{
//#ifdef _DEBUG
	messagesList.push_front(message);
	messagesTimer.push_front(debugMessagesLifeTime);
	messagesColor.push_front(color);
//#endif // _DEBUG
	hge->System_Log( message.c_str() );
}

/* update message timers */
void debugBox::update(float timeDelta)
{
	list<float>::iterator it;
	list<string>::iterator it_s = messagesList.begin();
	list<uint32>::iterator it_dw = messagesColor.begin();
	for(it = messagesTimer.begin(); it != messagesTimer.end(); it++, it_s++, it_dw++)
	{
		*it -= timeDelta;
		if( *it < 0.0f )
		{
			it = messagesTimer.erase(it);
			it_s = messagesList.erase(it_s);
			it_dw = messagesColor.erase(it_dw);
		}
		if( it == messagesTimer.end() )
			break;
	}
}

/* render messages list */
void debugBox::render()
{
	if( !font )
		return;

	int counter = 0;
	list<float>::iterator it = messagesTimer.begin();
	list<string>::iterator it_s = messagesList.begin();
	list<uint32>::iterator it_dw = messagesColor.begin();

	hge->Gfx_SetTransform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	if( options & DEBBOX_SHOWMESSAGES )
	{
		for(it; it != messagesTimer.end(); it++, it_s++, it_dw++)
		{
			float timer = *it;
			if( timer < 2.0f )
			{
				float alpha = (timer/2.0f) * 255;
				uint32 dwAlpha = (((uint32)alpha))<<24;
				uint32 color = ((*it_dw) & 0x00FFFFFF) + dwAlpha;
				font->SetColor(color);
			}
			else
			{
				font->SetColor(*it_dw);
			}

			font->printf((float)5, (float)(screen_height - ((counter + 1) * font->GetHeight())), HGETEXT_LEFT, "%s", it_s->c_str());

			if( counter > debugMessagesMaxCount )
				break;

			counter++;
		}
	}
	if( options & DEBBOX_SHOWFPS )
	{
		font->SetColor(0xFFFFFFFF);
		font->printf((float)5, (float)5, HGETEXT_LEFT, "%d", hge->Timer_GetFPS());
	}
}
//-----------------------------------------------------------------------------

void debugBox::setOptions(const WORD &_options)
{
	options |= _options;
}
//-----------------------------------------------------------------------------
