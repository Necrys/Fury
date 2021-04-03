/*=============================================================================

DESCRIPTION:
	box for debug text

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
05/05/2010	using namespace std - removed
04/08/2009	initial version
=============================================================================*/
#pragma once
#include <list>
#include <string>

class hgeFont;

const float debugMessagesLifeTime = 5.0f;
const	int	debugMessagesMaxCount = 20;

#define COLOR_MESSAGE	0xFFFFFFFF
#define COLOR_SUCCESS	0xFF00FF00
#define COLOR_WARNING	0xFFFFFF00
#define COLOR_ERROR		0xFFFF6666
#define COLOR_CRITICAL	0xFFFF0000

enum OPTIONS
{
	DEBBOX_SHOWMESSAGES = 0x0001,
	DEBBOX_SHOWFPS		= 0x0002,
};

/* debug box class */
class debugBox
{
private:
	std::list<std::string>	messagesList;
	std::list<uint32>	messagesColor;
	std::list<float>	messagesTimer;

	hgeFont*	font;

	WORD	options;

public:
	/* constructor */
	debugBox();
	~debugBox();

	/* update debug box */
	void update(float timeDelta);

	/* render */
	void render();

	/* add message */
	void addMessage(const std::string& message, const uint32& color = COLOR_SUCCESS);

	/* cleanup */
	void cleanup();

	void setOptions( const WORD& options );
};