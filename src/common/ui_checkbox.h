#pragma once

#include "ui_panel.h"
//-----------------------------------------------------------------------------

namespace gui
{
	class checkbox: public panel
	{
	public:
		checkbox(system* controller, _base* parent = 0);
		virtual ~checkbox();

		bool	checked;

		virtual void update(){};
		virtual void render();

		void	(*on_change)( bool cheked );

	protected:
		bool	handle_event( uint32 msg, uint32 param1, uint32 param2 );
	};

}