/*=============================================================================

	The Fury

DESCRIPTION:
	main executable source,

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
04/05/2010	initial version
=============================================================================*/


#include "system_main.h"
#include "system_interfaces.h"
#include "grid.h"
#include "data_level.h"
#include "data_staticobject.h"
#include "data_dynamicobject_base.h"
#include "helper_functions.h"
#include "background.h"
#include "sys_vars.h"

//-----------------------------------------------------------------------------

bool onRenderFunc()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

	if( renderBack )
		if( level )
			level->backgroundImage->render();

	log_render();

	if( renderGrid )
		grid->render();

	hge->Gfx_SetTransform(0.0f, 0.0f, global_position.x, global_position.y, 0.0f, 1.0f, 1.0f);
	camera_position = vector2f(-global_position.x, -global_position.y);
	camera_rect.x1 = camera_position.x;
	camera_rect.y1 = camera_position.y;
	camera_rect.x2 = camera_rect.x1 + screen_width;
	camera_rect.y2 = camera_rect.y1 + screen_height;
	if( level )
		level->render();
	
	hge->Gfx_SetTransform();
	camera_rect = hgeRect(0.0f, 0.0f, (float)screen_width, (float)screen_height);
	if( selectedStaticTemplate )
	{
		vector2f pos;
		hge->Input_GetMousePos(&pos.x, &pos.y);
		selectedStaticTemplate->position = pos - vector2f( abs((selectedStaticTemplate->bounds.x2 - selectedStaticTemplate->bounds.x1) / 2.0f), 
															-abs((selectedStaticTemplate->bounds.y2 - selectedStaticTemplate->bounds.y1) / 2.0f) );
		selectedStaticTemplate->position = grid->getSnapPoint( selectedStaticTemplate->position ) - vector2f(selectedStaticTemplate->bounds.x1, selectedStaticTemplate->bounds.y2);
		selectedStaticTemplate->vflip = selected_vflip;
		selectedStaticTemplate->hflip = selected_hflip;
		selectedStaticTemplate->rotation = selected_rotation;
		selectedStaticTemplate->render(layerTransparency);
	}
	if( selectedBackgroundTemplate )
	{
		vector2f pos;
		hge->Input_GetMousePos(&pos.x, &pos.y);
		selectedBackgroundTemplate->position = pos - vector2f( abs((selectedBackgroundTemplate->bounds.x2 - selectedBackgroundTemplate->bounds.x1) / 2.0f), 
															-abs((selectedBackgroundTemplate->bounds.y2 - selectedBackgroundTemplate->bounds.y1) / 2.0f) );
		selectedBackgroundTemplate->position = grid->getSnapPoint( selectedBackgroundTemplate->position ) - vector2f(selectedBackgroundTemplate->bounds.x1, selectedBackgroundTemplate->bounds.y2);
		selectedBackgroundTemplate->vflip = selected_vflip;
		selectedBackgroundTemplate->hflip = selected_hflip;
		selectedBackgroundTemplate->rotation = selected_rotation;
		selectedBackgroundTemplate->render(layerTransparency);
	}
	if( selectedForegroundTemplate )
	{
		vector2f pos;
		hge->Input_GetMousePos(&pos.x, &pos.y);
		selectedForegroundTemplate->position = pos - vector2f( abs((selectedForegroundTemplate->bounds.x2 - selectedForegroundTemplate->bounds.x1) / 2.0f), 
															-abs((selectedForegroundTemplate->bounds.y2 - selectedForegroundTemplate->bounds.y1) / 2.0f) );
		selectedForegroundTemplate->position = grid->getSnapPoint( selectedForegroundTemplate->position ) - vector2f(selectedForegroundTemplate->bounds.x1, selectedForegroundTemplate->bounds.y2);
		selectedForegroundTemplate->vflip = selected_vflip;
		selectedForegroundTemplate->hflip = selected_hflip;
		selectedForegroundTemplate->rotation = selected_rotation;
		selectedForegroundTemplate->render(layerTransparency);
	}
	if( selectedDynamicTemplate )
	{
		vector2f pos;
		hge->Input_GetMousePos(&pos.x, &pos.y);
		selectedDynamicTemplate->position = pos - vector2f( selectedDynamicTemplate->bounds.x2 / 2, -selectedDynamicTemplate->bounds.y2 / 2 );
		selectedDynamicTemplate->position = grid->getSnapPoint( selectedDynamicTemplate->position ) - vector2f(0, selectedDynamicTemplate->bounds.y2);
		selectedDynamicTemplate->render(layerTransparency);
	}

	gui_system->render();

	render_deferred();

	hge->Gfx_EndScene();

	return false;
}
//-----------------------------------------------------------------------------
