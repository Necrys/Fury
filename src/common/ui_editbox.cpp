#include "hgefont.h"
#include "ui_main.h"
#include "system_interfaces.h"
#include "ui_EditBox.h"
#include "str_helpers.h"
#include "ui_skin.h"

using namespace std;

//-----------------------------------------------------------------------------


namespace gui
{


void edit_box::pasteFromClipboard()
{
	if( caretPosition > text.length() )
		setCaretPosition((int)text.length());

	deleteSelection();

	if( OpenClipboard( NULL ) )
	{
		HANDLE handle = GetClipboardData( CF_TEXT );
		if( handle )
		{
			// Convert the ANSI string to Unicode, then
			// insert to our buffer.
			char *pText = (char*)GlobalLock( handle );
			if( pText )
			{
				if( text.length() + strlen(pText) <= maxChar )
				{
					text = text.substr(0, caretPosition) + pText + text.substr(caretPosition, text.length() - caretPosition);
					setCaretPosition( caretPosition + (int)strlen(pText) );
				}
				GlobalUnlock( handle );
			}
		}
		CloseClipboard();
	}
}
//-----------------------------------------------------------------------------

void edit_box::deleteSelection()
{
	if( caretPosition > text.length() || textSelectionStart > text.length() )
		setCaretPosition((int)text.length());

	if( caretPosition != textSelectionStart )
	{
		int selStart, selEnd;
		if( caretPosition > textSelectionStart )
		{
			selStart = textSelectionStart;
			selEnd = caretPosition;
		}
		else
		{
			selStart = caretPosition;
			selEnd = textSelectionStart;
		}
		text = text.substr(0, selStart) + text.substr(selEnd, text.length() - selEnd+1);
		setCaretPosition(selStart);
	}
}
//-----------------------------------------------------------------------------

void edit_box::copySelectionToClipboard()
{
	if( caretPosition > text.length() || textSelectionStart > text.length() )
		setCaretPosition((int)text.length());

	if( caretPosition != textSelectionStart && OpenClipboard( NULL ) )
	{
		EmptyClipboard();
		
		int selStart, selEnd;
		if( caretPosition > textSelectionStart )
		{
			selStart = textSelectionStart;
			selEnd = caretPosition;
		}
		else
		{
			selStart = caretPosition;
			selEnd = textSelectionStart;
		}
		string selection = text.substr(selStart, selEnd - selStart);

		HGLOBAL hBlock = GlobalAlloc( GMEM_MOVEABLE, sizeof(char) * ( selection.length() + 1 ) );
		if( hBlock )
		{
			char *pText = (char*)GlobalLock( hBlock );
			if( pText )
			{
				CopyMemory( pText, selection.c_str(), sizeof(char) * ( selection.length() ) );
				pText[selection.length()] = '\0';
				GlobalUnlock( hBlock );
			}
			SetClipboardData( CF_TEXT, hBlock );
		}
		CloseClipboard();
		if( hBlock )
			GlobalFree( hBlock );
	}
}
//-----------------------------------------------------------------------------

void edit_box::setCaretPosition( unsigned pos, bool shift )
{
	if( pos > text.length() )
		pos = (unsigned)text.length();
	if( pos < 0 )
		pos = 0;
	caretPosition = pos;
	if( !shift )
		textSelectionStart = caretPosition;
}
//-----------------------------------------------------------------------------

int	edit_box::getElementPointedByMouse( int mouseX, int mouseY )
{
	assert(skin);

	if( bounds_global.TestPoint((float)mouseX, (float)mouseY) )
	{
		int x;	// local mouse coords
		x = mouseX - (int)bounds_global.x1;
		//y = mouseY - globalBounds.y1;
		unsigned i;
		for( i = textFirstVisibleChar; i < text.length(); i++ )
		{
			float w = skin->font->GetStringWidth( text.substr(textFirstVisibleChar, i + 1).c_str() );
			if( x < w )
				break;
		}
		return i;
	}
	return 0;
}
//-----------------------------------------------------------------------------

inline bool isValidEditBoxChar( const char& ch )
{
	// unknown thing - cyrilic symbols lies between 0xFFFFFFA0 and 0xFFFFFFFF
	if( (ch > 31 && ch < 127) || (ch > 0xFFFFFFA0 && ch <= 0xFFFFFFFF) )
		return true;
	else 
		return false;
}
//-----------------------------------------------------------------------------

bool edit_box::handle_event( uint32 msg, uint32 param1, uint32 param2)
{
	if( enabled )
	{
		switch( msg )
		{
		case MSG_MOUSEENTER:
			if( on_mouse_enter )
				on_mouse_enter();
			break;
		case MSG_MOUSELEAVE:
			mouseLDown = false;
			mouseRDown = false;
			if( on_mouse_leave )
				on_mouse_leave();
			break;
		case MSG_MOUSEMOVE:
			if( on_mouse_move )
				on_mouse_move(HIWORD(param1), LOWORD(param1));
			if( mouseLDown )
				setCaretPosition( getElementPointedByMouse(HIWORD(param1), LOWORD(param1)), true );
			break;
		case MSG_MOUSELDOWN:
			if( !in_focus )
			{
				controller->set_focus(this);
				cursor = true;
			}
			setCaretPosition( getElementPointedByMouse(HIWORD(param1), LOWORD(param1)) );
			mouseLDown = true;
			break;
		case MSG_MOUSELUP:
			if( mouseLDown && on_mouse_click )
				on_mouse_click( 1, HIWORD(param1), LOWORD(param1) );
			mouseLDown = false;
			break;
		case MSG_MOUSERDOWN:
			if( !in_focus )
				controller->set_focus(this);
			mouseRDown = true;
			break;
		case MSG_MOUSERUP:
			if( mouseRDown && on_mouse_click )
				on_mouse_click( 2, HIWORD(param1), LOWORD(param1) );
			mouseRDown = false;
			break;
		case MSG_KEYPRESS:
			if( LOBYTE(LOWORD(param1)) == HGEK_C && LOBYTE(HIWORD(param1)) )
			{
				/* Ctrl-C */
				copySelectionToClipboard();
			}
			if( !readOnly )
			{
				//if( cursor )
				//{
				//	cursor = false;
					//text = text.substr(0, text.length() - 1);
				//}
				if( LOBYTE(LOWORD(param1)) == HGEK_X && LOBYTE(HIWORD(param1)) )
				{
					/* Ctrl-X */
					copySelectionToClipboard();
					deleteSelection();
				}
				if( LOBYTE(LOWORD(param1)) == HGEK_V && LOBYTE(HIWORD(param1)) )
				{
					/* Ctrl-V */
					pasteFromClipboard();
				}
				if( LOBYTE(LOWORD(param1)) == HGEK_LEFT )
					setCaretPosition(caretPosition - 1, (HIBYTE(LOWORD(param1)) == 0) ? false : true );
				if( LOBYTE(LOWORD(param1)) == HGEK_RIGHT )
					setCaretPosition(caretPosition + 1, (HIBYTE(LOWORD(param1)) == 0) ? false : true );
				if( LOBYTE(LOWORD(param1)) == HGEK_END )
					setCaretPosition((unsigned)text.length(), (HIBYTE(LOWORD(param1)) == 0) ? false : true );
				if( LOBYTE(LOWORD(param1)) == HGEK_HOME )
					setCaretPosition(0, (HIBYTE(LOWORD(param1)) == 0) ? false : true );
				if( LOBYTE(LOWORD(param1)) == HGEK_DELETE )
				{
					if( caretPosition != textSelectionStart )
					{
						deleteSelection();
					}
					else
					{
						if( text.length() > 0 )
							text = text.substr(0, caretPosition) + text.substr(caretPosition + 1, text.length() - caretPosition - 1);
					}
				}
				if( param1 == HGEK_BACKSPACE )
				{
					if( caretPosition != textSelectionStart )
					{
						deleteSelection();
					}
					else
					{
						if( text.length() > 0 )
						{
							text = text.substr(0, caretPosition - 1) + text.substr( caretPosition, text.length() - caretPosition);
							setCaretPosition(caretPosition - 1);
						}
					}
				}
				else if( param1 == HGEK_ENTER )
				{
					if( multiline )
					{
						if( text.length() < maxChar && maxChar != 0 || maxChar == 0 )
							text = text + '\n';
					}
					else
						controller->set_focus(parent);
				}
				else if( text.length() < maxChar && maxChar != 0 || maxChar == 0 )
				{
					char ch = LOBYTE(param2);
					if( isValidEditBoxChar(ch) )
					{
						if( caretPosition > text.length() )
							caretPosition = (unsigned)text.length();
						text = text.substr(0, caretPosition) + ch + text.substr(caretPosition, (unsigned)text.length() - caretPosition);
						setCaretPosition( caretPosition+1 );
					}
				}
				if( on_key_press )
					on_key_press( param2 );
			}

			break;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

edit_box::edit_box(system* _controller, _base* parent) : panel(_controller, parent)
{
	text = "EditBox";
	fontColor = 0xFFFFFFFF;
	fontDisabledColor = 0xFFAAAAAA;
	selectedBackColor = 0xFFc0c0c0;
	align = HGETEXT_LEFT | HGETEXT_TOP;
	can_get_focus = true;
	multiline = false;
	cursor = false;
	cursorBlinkPeriod = 0.8f;
	cursorBlinkCounter = 0.0f;
	maxChar = 0;
	readOnly = false;
	textFirstVisibleChar = 0;
	caretPosition = 0;
	mouseDown = false;

	caretQuad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
	caretQuad.v[0].z = caretQuad.v[1].z = caretQuad.v[2].z = caretQuad.v[3].z = 0.5f;
	caretQuad.tex = 0;

	selectionQuad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
	selectionQuad.v[0].z = selectionQuad.v[1].z = selectionQuad.v[2].z = selectionQuad.v[3].z = 0.5f;
	selectionQuad.tex = 0;

	recieve_user_input = true;

	on_key_press = 0;
}
//-----------------------------------------------------------------------------

edit_box::~edit_box()
{
}
//-----------------------------------------------------------------------------

void edit_box::update()
{
	if( enabled )
	{
		if( caretPosition > text.length() )
			setCaretPosition((unsigned)text.length());
		if( in_focus )
		{
			cursorBlinkCounter += hge->Timer_GetDelta();
			if( cursorBlinkCounter > cursorBlinkPeriod )
			{
				cursor = !cursor;
				cursorBlinkCounter = 0;
			}
		}
		else
		{
			cursor = false;
		}
	}
}
//-----------------------------------------------------------------------------

void edit_box::render()
{
	panel::render();

	/* render back */
	hgeRect b = bounds;
	if( parent )
	{
		b.x1 += parent->bounds_client.x1;
		b.x2 += parent->bounds_client.x1;
		b.y1 += parent->bounds_client.y1;
		b.y2 += parent->bounds_client.y1;
	}
	bounds_global = b;

	// render selection rect
	if( enabled )
	{
		if( !readOnly && textSelectionStart != caretPosition )
		{
			float x1 = skin->font->GetStringWidth(text.substr(textFirstVisibleChar, textSelectionStart - textFirstVisibleChar).c_str());
			float x2 = skin->font->GetStringWidth(text.substr(textFirstVisibleChar, caretPosition - textFirstVisibleChar).c_str());
			if( x2 < x1 )
			{
				float t = x1;
				x1 = x2;
				x2 = t;
			}
			selectionQuad.v[0].col = selectionQuad.v[1].col = selectionQuad.v[2].col = selectionQuad.v[3].col = selectedBackColor;
			selectionQuad.v[0].x = bounds_global.x1 + x1;	selectionQuad.v[0].y = bounds_global.y1 + 1;
			selectionQuad.v[1].x = bounds_global.x1 + x2;	selectionQuad.v[1].y = bounds_global.y1 + 1;
			selectionQuad.v[2].x = bounds_global.x1 + x2;	selectionQuad.v[2].y = bounds_global.y2 - 1;
			selectionQuad.v[3].x = bounds_global.x1 + x1;	selectionQuad.v[3].y = bounds_global.y2 - 1;
			hge->Gfx_RenderQuad(&selectionQuad);
		}

		/* render caret */
		if( !readOnly && cursor )
		{
			float w = skin->font->GetStringWidth(text.substr(textFirstVisibleChar, caretPosition - textFirstVisibleChar).c_str());
			caretQuad.v[0].col = caretQuad.v[1].col = caretQuad.v[2].col = caretQuad.v[3].col = fontColor;
			caretQuad.v[0].x = bounds_global.x1 + w;		caretQuad.v[0].y = bounds_global.y1 + 1;
			caretQuad.v[1].x = bounds_global.x1 + w + 1;	caretQuad.v[1].y = bounds_global.y1 + 1;
			caretQuad.v[2].x = bounds_global.x1 + w + 1;	caretQuad.v[2].y = bounds_global.y2 - 1;
			caretQuad.v[3].x = bounds_global.x1 + w;		caretQuad.v[3].y = bounds_global.y2 - 1;
			hge->Gfx_RenderQuad(&caretQuad);
		}
	}

	if( enabled )
		skin->font->SetColor(fontColor);
	else
		skin->font->SetColor(fontDisabledColor);
	skin->font->printfb(b.x1, b.y1, b.x2 - b.x1, b.y2 - b.y1, align, "%s\0", text.c_str());
}
//-----------------------------------------------------------------------------

}