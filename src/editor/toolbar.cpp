#include "toolbar.h"
#include "ui_panel.h"
#include "ui_imagebutton.h"
#include "ui_label.h"
#include "system_paths.h"
#include "system_interfaces.h"
#include "system_main.h"
#include "str_helpers.h"
#include "hgefont.h"
#include "grid.h"
#include "data_level.h"

#include "preferences_dlg.h"
//-----------------------------------------------------------------------------

gui::panel*			pnl_toolbar = 0;
gui::image_button*	btn_new_file = 0;
gui::image_button*	btn_open_file = 0;
gui::image_button*	btn_save_file = 0;
gui::image_button*	btn_preferences = 0;
gui::image_button*	btn_grid_increase = 0;
gui::image_button*	btn_grid_decrease = 0;
gui::label*			lbl_grid_size = 0;
gui::label*			lbl_grid_size_cap = 0;
//-----------------------------------------------------------------------------

void new_file_press( gui::button* btn );
void open_file_press( gui::button* btn );
void save_file_press( gui::button* btn );
void preferences_press( gui::button* btn );
void grid_increase_press( gui::button* btn );
void grid_decrease_press( gui::button* btn );
//-----------------------------------------------------------------------------

void toolbar_init()
{
	pnl_toolbar = new gui::panel( gui_system );
	pnl_toolbar->bounds = hgeRect( 5.0f, 0.0f, 390.0f, 56.0f );

	btn_new_file = new gui::image_button( gui_system, pnl_toolbar );
	btn_new_file->bounds = hgeRect( 3.0f, 3.0f, 68.0f, 53.0f );
	btn_new_file->setImage( (Path::uigfx + "icons\\doc.png").c_str() );
	btn_new_file->caption = "New";
	btn_new_file->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_new_file->on_press = new_file_press;

	btn_open_file = new gui::image_button( gui_system, pnl_toolbar );
	btn_open_file->bounds = hgeRect( 73.0f, 3.0f, 138.0f, 53.0f );
	btn_open_file->setImage( (Path::uigfx + "icons\\hd_upload.png").c_str() );
	btn_open_file->caption = "Open";
	btn_open_file->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_open_file->on_press = open_file_press;

	btn_save_file = new gui::image_button( gui_system, pnl_toolbar );
	btn_save_file->bounds = hgeRect( 143.0f, 3.0f, 208.0f, 53.0f );
	btn_save_file->setImage( (Path::uigfx + "icons\\save.png").c_str() );
	btn_save_file->caption = "Save";
	btn_save_file->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_save_file->on_press = save_file_press;

	btn_preferences = new gui::image_button( gui_system, pnl_toolbar );
	btn_preferences->bounds = hgeRect( 213.0f, 3.0f, 278.0f, 53.0f );
	btn_preferences->setImage( (Path::uigfx + "icons\\pref.png").c_str() );
	btn_preferences->caption = "Preferences";
	btn_preferences->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_preferences->on_press = preferences_press;

	lbl_grid_size_cap = new gui::label( gui_system, pnl_toolbar );
	lbl_grid_size_cap->bounds = hgeRect(295.0f, 5.0f, 375.0f, 15.0f);
	lbl_grid_size_cap->text = "Grid size";
	lbl_grid_size_cap->align = HGETEXT_TOP | HGETEXT_CENTER;

	lbl_grid_size = new gui::label( gui_system, pnl_toolbar );
	lbl_grid_size->bounds = hgeRect(319.0f, 25.0f, 349.0f, 35.0f);
	lbl_grid_size->text = "";
	lbl_grid_size->align = HGETEXT_TOP | HGETEXT_CENTER;

	btn_grid_decrease = new gui::image_button( gui_system, pnl_toolbar );
	btn_grid_decrease->bounds = hgeRect( 299.0f, 25.0f, 315.0f, 41.0f );
	btn_grid_decrease->setImage( (Path::uigfx + "icons\\left.png").c_str() );
	btn_grid_decrease->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_grid_decrease->on_press = grid_decrease_press;

	btn_grid_increase = new gui::image_button( gui_system, pnl_toolbar );
	btn_grid_increase->bounds = hgeRect( 353.0f, 25.0f, 369.0f, 41.0f );
	btn_grid_increase->setImage( (Path::uigfx + "icons\\right.png").c_str() );
	btn_grid_increase->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_grid_increase->on_press = grid_increase_press;
}
//-----------------------------------------------------------------------------

void new_file_press( gui::button* btn )
{
	stateManager->setGlobalState(GS_NEWLEVEL);
}
//-----------------------------------------------------------------------------

void open_file_press( gui::button* btn )
{
	stateManager->setGlobalState(GS_LEVELLOAD);
}
//-----------------------------------------------------------------------------

void save_file_press( gui::button* btn )
{
	if( level )
		level->save();
}
//-----------------------------------------------------------------------------

void preferences_press( gui::button* btn )
{
	pref_dlg_show( false );
}
//-----------------------------------------------------------------------------

void grid_increase_press( gui::button* btn )
{
	grid->increaseStep();
	lbl_grid_size->text = uint2str(grid->gridStep);
}
//-----------------------------------------------------------------------------

void grid_decrease_press( gui::button* btn )
{
	grid->decreaseStep();
	lbl_grid_size->text = uint2str(grid->gridStep);
}
//-----------------------------------------------------------------------------