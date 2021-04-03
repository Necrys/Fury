#pragma once

#include "ui_panel.h"
#include <string>
//-----------------------------------------------------------------------------

namespace gui
{
	class button: public panel
	{
	public:
		button(system* controller, _base* parent = 0);
		virtual ~button();
		
		std::string	caption;
		int			align;
		uint32		fontColor;
		uint32		fontDisabledColor;

		virtual void update();

		virtual void render();

		// events
		void	(*on_press)( button* ctrl );

	protected:
		bool		mouseOver;
		bool		handle_event( uint32 msg, uint32 param1, uint32 param2 );
	};

}