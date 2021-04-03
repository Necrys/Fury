#pragma once
//-----------------------------------------------------------------------------

namespace gui
{
	class window;
	class label;
	class image_button;
	class image_list;
	class checkbox;
}

class GameTrigger;
//-----------------------------------------------------------------------------

void trigger_dlg_init();
void trigger_dlg_show( GameTrigger* trigger );
void trigger_dlg_hide();
//-----------------------------------------------------------------------------