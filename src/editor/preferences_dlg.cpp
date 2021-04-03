#include "preferences_dlg.h"

#include "ui_dropdownlist.h"
#include "ui_window.h"
#include "ui_label.h"
#include "ui_editbox.h"

#include "system_interfaces.h"
#include "system_main.h"
#include "system_paths.h"
#include "data_level.h"
#include "data_tileset.h"

#include "hgefont.h"
//-----------------------------------------------------------------------------

gui::window*			wnd_pref_dlg		= 0;
gui::label*			lbl_level_name		= 0;
gui::edit_box*		edb_level_name		= 0;
gui::label*			lbl_level_music		= 0;
gui::drop_list*		ddl_level_music		= 0;
gui::label*			lbl_level_tileset	= 0;
gui::drop_list*		ddl_level_tileset	= 0;
gui::image_button*	btn_pref_ok			= 0;
gui::image_button*	btn_pref_cancel		= 0;
//-----------------------------------------------------------------------------

void pref_ok_press( gui::button* btn );
void pref_cancel_press( gui::button* btn );
//-----------------------------------------------------------------------------

void pref_dlg_init()
{
	wnd_pref_dlg = new gui::window( gui_system );
	wnd_pref_dlg->bounds = hgeRect( 200.0f, 200.0f, 550.0f, 400.0f );
	wnd_pref_dlg->caption = "Свойства уровня";
	wnd_pref_dlg->set_visibility(false);

	btn_pref_ok = new gui::image_button( gui_system, wnd_pref_dlg );
	btn_pref_ok->bounds = hgeRect( 141.0f, 150.0f, 191.0f, 180.0f );
	btn_pref_ok->setImage( (Path::uigfx + "icons\\ok.png").c_str() );
	btn_pref_ok->caption = "OK";
	btn_pref_ok->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_pref_ok->on_press = pref_ok_press;

	btn_pref_cancel = new gui::image_button( gui_system, wnd_pref_dlg );
	btn_pref_cancel->bounds = hgeRect( 196.0f, 150.0f, 246.0f, 180.0f );
	btn_pref_cancel->setImage( (Path::uigfx + "icons\\cancel.png").c_str() );
	btn_pref_cancel->caption = "Отмена";
	btn_pref_cancel->align = HGETEXT_TOP | HGETEXT_CENTER;
	btn_pref_cancel->on_press = pref_cancel_press;

	lbl_level_name = new gui::label( gui_system, wnd_pref_dlg );
	lbl_level_name->bounds = hgeRect(10.0f, 10.0f, 80.0f, 22.0f);
	lbl_level_name->text = "Название";
	lbl_level_name->align = HGETEXT_TOP | HGETEXT_RIGHT;

	edb_level_name = new gui::edit_box( gui_system, wnd_pref_dlg );
	edb_level_name->bounds = hgeRect(90.0f, 10.0f, 340.0f, 22.0f);
	edb_level_name->text = "Новый уровень The Fury";

	lbl_level_music = new gui::label( gui_system, wnd_pref_dlg );
	lbl_level_music->bounds = hgeRect(10.0f, 45.0f, 110.0f, 57.0f);
	lbl_level_music->text = "Фоновая музыка";
	lbl_level_music->align = HGETEXT_TOP | HGETEXT_RIGHT;

	ddl_level_music = new gui::drop_list( gui_system, wnd_pref_dlg );
	ddl_level_music->bounds = hgeRect(120.0f, 45.0f, 340.0f, 57.0f);

	lbl_level_tileset = new gui::label( gui_system, wnd_pref_dlg );
	lbl_level_tileset->bounds = hgeRect(10.0f, 80.0f, 110.0f, 92.0f);
	lbl_level_tileset->text = "Набор спрайтов";
	lbl_level_tileset->align = HGETEXT_TOP | HGETEXT_RIGHT;

	ddl_level_tileset = new gui::drop_list( gui_system, wnd_pref_dlg );
	ddl_level_tileset->bounds = hgeRect(120.0f, 80.0f, 340.0f, 100.0f);

	WIN32_FIND_DATA findData;
	char pt[MAX_PATH];

	strcpy(pt, Path::tilesets.c_str());
	strcat(pt, "*.xml");

	HANDLE findHandle = FindFirstFile(pt, &findData);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if( !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				ddl_level_tileset->insertElement(findData.cFileName);
		} 
		while(FindNextFile(findHandle, &findData));
	}
}
//-----------------------------------------------------------------------------

void pref_dlg_show( bool new_level )
{
	wnd_pref_dlg->set_visibility(true);
	btn_pref_cancel->enabled = !new_level;
	gui_system->show_modal_window(wnd_pref_dlg);
}
//-----------------------------------------------------------------------------

void pref_dlg_hide()
{
	wnd_pref_dlg->set_visibility(false);
	gui_system->set_focus(0);
}
//-----------------------------------------------------------------------------

void pref_ok_press( gui::button* btn )
{
	ASSERT(level);

	level->name = edb_level_name->text;
	level->music = ddl_level_music->getSelectionText();
	level->musicID = ddl_level_music->getSelectionID();
	level->tileset = ddl_level_tileset->getSelectionText();
	level->tilesetID = ddl_level_tileset->getSelectionID();

	if( g_tileset )
	{
		if( g_tileset->file != level->tileset )
			g_tileset->loadTileset(level->tileset);
	}
	else
	{
		g_tileset = new Tileset();
		g_tileset->loadTileset(level->tileset);
	}

	ddl_level_tileset->enabled = false;
//	rightPanel->ilStaticObjectsList->reloadTemplates();
//	rightPanel->ilDynamicsList->reloadTemplates();
//	rightPanel->ilBackgroundObjects->reloadTemplates();
//	rightPanel->ilForegroundObjects->reloadTemplates();
	stateManager->setEditLayerState(ES_STATICS);

	log_msg("Свойства уровня успешно сохранены");

	pref_dlg_hide();
}
//-----------------------------------------------------------------------------

void pref_cancel_press( gui::button* btn )
{
	if( level )
	{
		edb_level_name->text = level->name;
		ddl_level_music->setSelection(level->musicID);
		ddl_level_tileset->setSelection(level->tilesetID);
	}

	pref_dlg_hide();
}
//-----------------------------------------------------------------------------