#include "system_uistatemanager.h"
#include "system_main.h"
#include "data_level.h"
#include "system_interfaces.h"
#include "system_ui.h"
#include "system_paths.h"
#include "ui_editbox.h"
#include "ui_dropdownlist.h"
#include "sys_vars.h"
#include "toolbar.h"
#include "save_dlg.h"
#include "preferences_dlg.h"
#include "toolbox.h"

using namespace std;

//-----------------------------------------------------------------------------

int levelLoadDialog();

//-----------------------------------------------------------------------------

UIStateManager::UIStateManager()
{
	globalState = GS_LOADINTERFACE;
	editState = ES_NOLEVEL;
}
//-----------------------------------------------------------------------------

UIStateManager::~UIStateManager()
{

};
//-----------------------------------------------------------------------------

void UIStateManager::setGlobalState( const GLOBALSTATE& state )
{
	switch( state )
	{
	case GS_LOADINTERFACE:
		break;
	case GS_NOLEVEL:
		switch( globalState )
		{
		case GS_LEVEL:
			if( level && !level->saved() )
			{
				save_dlg_next_state = GS_NOLEVEL;
				save_dlg_show();
				return;
			}
			break;
		default:
			if( level )
				delete level;
			level = 0;
			selectedStaticTemplate = 0;
			selectedLevelObject = 0;
			selectedCollision = 0;
			toolbox_hide();
			btn_preferences->enabled = false;
			btn_save_file->enabled = false;
//			toolBar->preferencesBtn->enabled = false;
//			toolBar->saveFileBtn->enabled = false;
			break;
		}
		break;
	case GS_NEWLEVEL:
		switch( globalState )
		{
		case GS_LEVEL:
			if( level && !level->saved() )
			{
				save_dlg_next_state = GS_NEWLEVEL;
				save_dlg_show();
				return;
			}
		case GS_NOLEVEL:
			if( !level )
			{
				edb_level_name->text = "Новый уровень The Fury";
				ddl_level_tileset->setSelection(0);
				ddl_level_music->setSelection(-1);
				pref_dlg_show( true );
			}
			break;
		}
	case GS_LEVELLOAD:
		switch( globalState )
		{
		case GS_LEVEL:
			if( level && !level->saved() )
			{
				save_dlg_next_state = GS_LEVELLOAD;
				save_dlg_show();
				return;
			}
		case GS_NOLEVEL:
			break;
		}
	}

	globalState = state;

	if( globalState == GS_LEVELLOAD )
	{
		if( levelLoadDialog() )
		{
			log_msg(COLOR_ERROR, "Уровень не был загружен");
			setGlobalState(GS_NOLEVEL);
			return;
		}
		btn_preferences->enabled = true;
		btn_save_file->enabled = true;
//		rightPanel->ilStaticObjectsList->reloadTemplates();
//		rightPanel->ilDynamicsList->reloadTemplates();
//		rightPanel->ilBackgroundObjects->reloadTemplates();
//		rightPanel->ilForegroundObjects->reloadTemplates();
//		rightPanel->set_visibility(true);
//		rightPanel->ddlElementTypes->setSelection(1);
		toolbox_show();
		global_position = vector2f(0.0f, 0.0f);

		log_msg("Уровень загружен");
		setGlobalState(GS_LEVEL);
	}

	if( globalState == GS_NEWLEVEL )
	{
		if( level )
			delete level;
		selectedStaticTemplate = 0;
		selectedLevelObject = 0;
		selectedCollision = 0;
		level = new GameLevel();
		btn_preferences->enabled = true;
		btn_save_file->enabled = true;
		ddl_level_tileset->enabled = true;
//		rightPanel->set_visibility(true);
//		rightPanel->ddlElementTypes->setSelection(1);
		toolbox_show();
		log_msg("Новый уровень создан");
		setGlobalState(GS_LEVEL);
	}
}
//-----------------------------------------------------------------------------

void UIStateManager::setEditLayerState( const EDITSTATE& state )
{
//	rightPanel->ilStaticObjectsList->set_visibility(false);
//	rightPanel->ilCollisionsList->set_visibility(false);
//	rightPanel->ilDynamicsList->set_visibility(false);
//	rightPanel->ilBackgroundObjects->set_visibility(false);
//	rightPanel->ilForegroundObjects->set_visibility(false);
	selectedLevelObject = 0;
	selectedStaticTemplate = 0;
	switch( state )
	{
	case ES_STATICS:
//		rightPanel->ilStaticObjectsList->set_visibility(true);
		break;
	case ES_COLLISIONS:
//		rightPanel->ilCollisionsList->set_visibility(true);
		break;
	case ES_DYNAMICS:
//		rightPanel->ilDynamicsList->set_visibility(true);
		break;
	case ES_BACKGROUND:
//		rightPanel->ilBackgroundObjects->set_visibility(true);
		break;
	case ES_FOREGROUND:
//		rightPanel->ilForegroundObjects->set_visibility(true);
		break;
	default:
		break;
	}
	editState = state;
}
//-----------------------------------------------------------------------------

GLOBALSTATE UIStateManager::getGlobalState() const
{
	return globalState;
}
//-----------------------------------------------------------------------------

EDITSTATE UIStateManager::getEditLayerState() const
{
	return editState;
}
//-----------------------------------------------------------------------------

int levelLoadDialog()
{
	OPENFILENAME	ofn;

	char fileName[MAX_PATH];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = Path::levels.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrFilter = "Fury level files\0*.flf;\0";
	ofn.nFilterIndex = 1;

	if( GetOpenFileName(&ofn) )
	{
		if( level )
			delete level;
		level = new GameLevel();

		if( level->load(ofn.lpstrFile) )
			return -2;
		else
			return 0;
	}
	else
	{
		return -1;
	}
}