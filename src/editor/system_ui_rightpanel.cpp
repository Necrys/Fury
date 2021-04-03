/*
#include <string>
#include "system_ui_rightpanel.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "system_main.h"
#include "sys_vars.h"

//-----------------------------------------------------------------------------

RightPanel::RightPanel(gui::system* _controller, gui::_base* parent) : panel(_controller, parent)
{
	ilStaticObjectsList = new StaticsImageList(_controller, this);
	ilStaticObjectsList->bounds = hgeRect( 10.0f, 50.0f, 240.0f, screen_height - 10.0f );

	ilBackgroundObjects = new BackgroundIL(_controller, this);
	ilBackgroundObjects->bounds = hgeRect( 10.0f, 50.0f, 240.0f, screen_height - 10.0f );

	ilForegroundObjects = new ForegroundIL(_controller, this);
	ilForegroundObjects->bounds = hgeRect( 10.0f, 50.0f, 240.0f, screen_height - 10.0f );

	ilCollisionsList = new CollisionsImageList(_controller, this);
	ilCollisionsList->bounds = hgeRect( 10.0f, 50.0f, 240.0f, screen_height - 10.0f );

	ilCollisionsList->insertElement(Path::gfxicons + "square.png", "Прямоугольник");
	ilCollisionsList->insertElement(Path::gfxicons + "triangle.png", "Треугольник");
	ilCollisionsList->insertElement(Path::gfxicons + "circle.png", "Круг");

	ilDynamicsList = new DynamicsImageList(_controller, this);
	ilDynamicsList->bounds = hgeRect( 10.0f, 50.0f, 240.0f, screen_height - 10.0f );

	ddlElementTypes = new EditModesList(_controller, this);
	ddlElementTypes->bounds = hgeRect(10.0f, 10.0f, 240.0f, 30.0f);
}
//-----------------------------------------------------------------------------

void EditModesList::onSelectionChanged(const unsigned int &id)
{
	switch( id )
	{
	case 0:
		stateManager->setEditLayerState(ES_FOREGROUND);
		break;
	case 1:
		stateManager->setEditLayerState(ES_STATICS);
		break;
	case 2:
		stateManager->setEditLayerState(ES_COLLISIONS);
		break;
	case 3:
		stateManager->setEditLayerState(ES_DYNAMICS);
		break;
	case 4:
		stateManager->setEditLayerState(ES_BACKGROUND);
		break;
	default:
		setSelection(1);
		break;
	}
}
//-----------------------------------------------------------------------------

void CollisionsImageList::onSelectionChanged(const unsigned int &id)
{

}
//-----------------------------------------------------------------------------
*/