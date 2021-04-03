#pragma once
//-----------------------------------------------------------------------------

#include <string>
#include <list>
#include <vector>
#include "hgesprite.h"
#include "ui_panel.h"
#include "common.h"
//-----------------------------------------------------------------------------

namespace gui
{
//-----------------------------------------------------------------------------

class image_list_item;
class image_button;
//-----------------------------------------------------------------------------

enum IMAGEFLAGS
{
	IF_NONE				= 0x00000000,
	IF_VERTICAL_FLIP	= 0x00000001,
	IF_HORIZONTAL_FLIP	= 0x00000002,

	IF_FORCE_DWORD		= 0x7FFFFFFF
};
//-----------------------------------------------------------------------------

class image_list : public panel
{
public:
	image_list(system* controller, _base* parent = 0);
	virtual ~image_list(){};

	uint32 ilBackColor;
	uint32 columns;

	virtual int					getSelectionID() const;
	virtual void				setSelection(const int& id);
	virtual void*				getSelectionUserData() const;
	virtual image_list_item*	getSelection();
	virtual unsigned			size() const;
	virtual image_list_item*& operator[] (int id);
	virtual void				clear();

	virtual void		update(){};

//	virtual void		setSkin( const std::string& filePath );

	virtual void		render();
	virtual void		scrollUp();
	virtual void		scrollDown();
	virtual void		pageUp();
	virtual void		pageDown();

	virtual image_list_item* insertElement(	const std::string& image, 
												const std::string& text, 
												const float& x = 0.0f, 
												const float& y = 0.0f, 
												const float& w = 0.0f, 
												const float& h = 0.0f, 
												const float& imageScale = 1.0f,
												const uint32& flags = 0);

	virtual image_list_item* insertElement(	const hgeSprite* image, 
												const std::string& text,
												const float& imageScale = 1.0f);

	virtual void removeElement( const unsigned& id );


	virtual void onSelectionChanged(const unsigned& id){};

	void (*on_select)( image_list_item* item );

protected:
	bool	handle_event( uint32 msg, uint32 param1, uint32 param2);

	void	renderButtons();

	std::vector<image_list_item*> elements;
	int selectionID;
	int			scroll;
	unsigned	buttonHeight;
	unsigned	buttonWidth;

	bool canScrollDown;

	image_button* upButton;
	image_button* downButton;
	image_button* pgUpButton;
	image_button* pgDownButton;

	unsigned pageSize;
};
//-----------------------------------------------------------------------------

}