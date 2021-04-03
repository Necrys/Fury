#pragma once

#include "ui_label.h"
//-----------------------------------------------------------------------------

namespace gui
{
	class drop_list;

	class drop_list_item: public label
	{
	public:
		drop_list_item(system* controller, _base* parent = 0);
		virtual ~drop_list_item();

		uint32 selectedColor;
		uint32 basicColor;
		uint32 mouseOverColor;

		int id;

		virtual void render();

	protected:
		bool handle_event( uint32 msg, uint32 param1, uint32 param2 );

		drop_list* listController;
		bool		mouseOver;

		friend class drop_list;
	};

}