#pragma once

#include <string>
#include "ui_panel.h"
#include "hge.h"
//-----------------------------------------------------------------------------

namespace gui
{

class edit_box: public panel
{
public:
	edit_box(system* controller, _base* parent = 0);
	virtual ~edit_box();

	std::string	text;
	bool			multiline;
	int				align;
	uint32			fontColor;
	uint32			fontDisabledColor;
	uint32			selectedTextColor;
	uint32			selectedBackColor;
	float			cursorBlinkPeriod;
	unsigned		maxChar;
	bool			readOnly;

	virtual void update();

	virtual void render();

	void	setCaretPosition( unsigned pos, bool shift = false );
	int		getElementPointedByMouse( int mouseX, int mouseY );
	void	copySelectionToClipboard();
	void	deleteSelection();
	void	pasteFromClipboard();

	// events
	void	(*on_key_press)( uint32 key );

protected:
	hgeQuad		caretQuad;
	hgeQuad		selectionQuad;

	// caret
	bool		cursor;
	float		cursorBlinkCounter;
	unsigned	caretPosition;

	hgeRect		textBounds;
	unsigned	textSelectionStart;	// selection start, caret marks selection end
	unsigned	textFirstVisibleChar;

	bool		insertMode;

	bool		mouseDown;

	bool		handle_event( uint32 msg, uint32 param1, uint32 param2);
};
//-----------------------------------------------------------------------------

}