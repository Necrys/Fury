#pragma once

#include <vector>
#include <string>

#include "ui_panel.h"
#include "hge.h"
//-----------------------------------------------------------------------------

class hgeFont;

namespace gui
{

class drop_list_item;
//-----------------------------------------------------------------------------

class drop_list: public panel
{
public:
	drop_list(system* controller, _base* parent = 0);
	virtual ~drop_list();

	uint32	ddlBackColor;
	uint32	ddlBorderColor;
	uint32	ddlTextColor;
	uint32	ddlDisabledTextColor;
	uint32	ddlDisabledBackColor;

	virtual std::string getSelectionText() const;
	virtual int getSelectionID() const;

	virtual void render();

	virtual void insertElement( const std::string& text );
	virtual void setSelection( const unsigned& id );

	virtual void clear();

	// events
	void	(*on_change)( uint32 selection_id );

//	virtual void onSelectionChanged( const unsigned& id ){};

protected:
	bool	handle_event( uint32 msg, uint32 param1, uint32 param2);
	void	drop( const bool& flag );

	std::vector<drop_list_item*>	elements;
	int	selectionID;

	hgeRect ddlRect;
	hgeQuad ddlDropIconQuad;
	hgeQuad	ddlListPanel;

	hgeRect	droppedBounds;
	hgeRect	undroppedBounds;

	bool		dropped;
};
//-----------------------------------------------------------------------------

}