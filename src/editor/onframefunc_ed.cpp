#include "types.h"
#include <time.h>
#include "system_main.h"
#include "system_interfaces.h"
#include "system_ui.h"
#include "grid.h"
#include "data_level.h"
#include "data_staticobject.h"
#include "data_collision_square.h"
#include "data_collision_triangle.h"
#include "data_collision_circle.h"
#include "data_dynamicobject_base.h"
#include "data_dynamic_point.h"
#include "data_dynamic_lift.h"
#include "str_helpers.h"
#include "sys_vars.h"
#include "toolbox.h"

//-----------------------------------------------------------------------------

bool show = false;
vector2f mouseDragPoint = vector2f(0.0f, 0.0f);

long lastClickTime = 0;

void handleMouse();
//-----------------------------------------------------------------------------

bool onFrameFunc()
{
	float timeDelta = hge->Timer_GetDelta();

	handleMouse();

	grid->update(timeDelta);

	if( level )
		level->update(timeDelta);

	gui_system->update();

	log_update(timeDelta);

	return false;
}
//-----------------------------------------------------------------------------

void handleMouse()
{
	vector2f mousePos;
	hge->Input_GetMousePos(&mousePos.x, &mousePos.y);

	EDITSTATE editState = stateManager->getEditLayerState();

	// handle doubleclick
	if( hge->Input_KeyUp( HGEK_LBUTTON ) )
	{
		long new_click_time = clock();
		if( new_click_time - lastClickTime <= 300 )
		{
			// double click
			if( stateManager->getEditLayerState() == ES_DYNAMICS )
			{
				GameDynamicObject* dyn = level->dynamic_select( mousePos - global_position );
				if( dyn )
					dyn->showPropsWindow();
				selectedLevelDynamic = 0;
			}
			lastClickTime = 0;
			return;
		}
		else
		{
			lastClickTime = new_click_time;
		}
	}

	// toggle grid
	if( hge->Input_KeyUp(HGEK_G) )
		renderGrid = !renderGrid;

	// toggle background
	if( hge->Input_KeyUp(HGEK_B) )
		if( level )
			renderBack = !renderBack;

	// toggle layers transparency
	if( hge->Input_KeyUp(HGEK_T) )
		if( level )
			layerTransparency = !layerTransparency;

	// mouse map movement
	if( hge->Input_KeyDown(HGEK_MBUTTON) && !gui_system->mouse_over_ui )
		hge->Input_GetMousePos(&mouseDragPoint.x, &mouseDragPoint.y);

	if( hge->Input_KeyDown(HGEK_RBUTTON) && !gui_system->mouse_over_ui )
	{
		if( editState == ES_STATICS )
		{
			if( iml_tiles->getSelectionID() != -1 )
			{
				if( selectedStaticTemplate )
					delete selectedStaticTemplate;
				selectedStaticTemplate = 0;
				iml_tiles->setSelection(-1);
			}
			else
			{
				if( level )
				{
					selectedLevelObject = level->tile_select( mousePos - global_position, LAYER_STATIC );
					if( selectedLevelObject )
					{
						level->tile_remove(selectedLevelObject);
						delete selectedLevelObject;
						selectedLevelObject = 0;
					}
				}
			}
		}
		if( editState == ES_BACKGROUND )
		{
			if( iml_tiles->getSelectionID() != -1 )
			{
				if( selectedBackgroundTemplate )
					delete selectedBackgroundTemplate;
				selectedBackgroundTemplate = 0;
				iml_tiles->setSelection(-1);
			}
			else
			{
				if( level )
				{
					selectedBackgroundObject = level->tile_select( mousePos - global_position, LAYER_BACKGROUND );
					if( selectedBackgroundObject )
					{
						level->tile_remove(selectedBackgroundObject, LAYER_BACKGROUND);
						delete selectedBackgroundObject;
						selectedBackgroundObject = 0;
					}
				}
			}
		}
		if( editState == ES_FOREGROUND )
		{
			if( iml_tiles->getSelectionID() != -1 )
			{
				if( selectedForegroundTemplate )
					delete selectedForegroundTemplate;
				selectedForegroundTemplate = 0;
				iml_tiles->setSelection(-1);
			}
			else
			{
				if( level )
				{
					selectedForegroundObject = level->tile_select( mousePos - global_position, LAYER_FOREGROUND );
					if( selectedForegroundObject )
					{
						level->tile_remove(selectedForegroundObject, LAYER_FOREGROUND);
						delete selectedForegroundObject;
						selectedForegroundObject = 0;
					}
				}
			}
		}
		if( editState == ES_DYNAMICS )
		{
			if( iml_tiles->getSelectionID() != -1 )
			{
				if( selectedDynamicTemplate )
					delete selectedDynamicTemplate;
				selectedDynamicTemplate = 0;
				iml_tiles->setSelection(-1);
			}
			else
			{
				if( level )
				{
					selectedLevelDynamic = level->dynamic_select( mousePos - global_position );
					if( selectedLevelDynamic )
					{
						level->dynamic_remove(selectedLevelDynamic);
						delete selectedLevelDynamic;
						selectedLevelDynamic = 0;
					}
				}
			}
		}
		if( editState == ES_COLLISIONS )
		{
			if( iml_tiles->getSelectionID() != -1 )
			{
				iml_tiles->setSelection(-1);
			}
			else
			{
				if( level )
				{
					selectedCollision = level->collision_select( mousePos - global_position );
					selectedCollisionCorner = -1;
					if( selectedCollision )
					{
						level->collision_remove(selectedCollision);
						delete selectedCollision;
						selectedCollision = 0;
					}
				}
			}
		}
	}

	if( hge->Input_GetKeyState(HGEK_MBUTTON) && !gui_system->mouse_over_ui )
	{
		vector2f mouseMov;
		mouseMov = mousePos - mouseDragPoint;
		mouseDragPoint = mousePos;
		global_position += mouseMov;
	}

	if( hge->Input_KeyDown(HGEK_LBUTTON) && !gui_system->mouse_over_ui )
	{
		if( editState == ES_STATICS )
		{
			if( selectedStaticTemplate )
			{
				GameStaticObject* newobj = new GameStaticObject(*selectedStaticTemplate);
				newobj->position -= global_position;
				newobj->hflip = selected_hflip;
				newobj->vflip = selected_vflip;
				newobj->rotation = selected_rotation;
				level->tile_add(newobj, LAYER_STATIC, !hge->Input_GetKeyState(HGEK_CTRL));
				if( !hge->Input_GetKeyState(HGEK_SHIFT) )
				{
					selectedStaticTemplate = 0;
					iml_tiles->setSelection(-1);
				}
			}
			else
			{
				if( level )
				{
					selectedLevelObject = level->tile_select( mousePos - global_position, LAYER_STATIC );
					mouseDragPoint = mousePos;
				}
			}
		}
		if( editState == ES_BACKGROUND )
		{
			if( selectedBackgroundTemplate )
			{
				GameStaticObject* newobj = new GameStaticObject(*selectedBackgroundTemplate);
				newobj->position -= global_position;
				newobj->hflip = selected_hflip;
				newobj->vflip = selected_vflip;
				newobj->rotation = selected_rotation;
				level->tile_add(newobj, LAYER_BACKGROUND, !hge->Input_GetKeyState(HGEK_CTRL));
				if( !hge->Input_GetKeyState(HGEK_SHIFT) )
				{
					selectedBackgroundTemplate = 0;
					iml_tiles->setSelection(-1);
				}
			}
			else
			{
				if( level )
				{
					selectedBackgroundObject = level->tile_select( mousePos - global_position, LAYER_BACKGROUND );
					mouseDragPoint = mousePos;
				}
			}
		}
		if( editState == ES_FOREGROUND )
		{
			if( selectedForegroundTemplate )
			{
				GameStaticObject* newobj = new GameStaticObject(*selectedForegroundTemplate);
				newobj->position -= global_position;
				newobj->hflip = selected_hflip;
				newobj->vflip = selected_vflip;
				newobj->rotation = selected_rotation;
				level->tile_add(newobj, LAYER_FOREGROUND, !hge->Input_GetKeyState(HGEK_CTRL));
				if( !hge->Input_GetKeyState(HGEK_SHIFT) )
				{
					selectedForegroundTemplate = 0;
					iml_tiles->setSelection(-1);
				}
			}
			else
			{
				if( level )
				{
					selectedForegroundObject = level->tile_select( mousePos - global_position, LAYER_FOREGROUND );
					mouseDragPoint = mousePos;
				}
			}
		}
		if( editState == ES_DYNAMICS )
		{
			if( selectedDynamicTemplate )
			{
				GameDynamicObject* newobj = selectedDynamicTemplate->clone();
				newobj->position -= global_position;
				level->dynamic_add(newobj);

				if( newobj->type == DYNAMIC_COIN && hge->Input_GetKeyState(HGEK_SHIFT) )
				{

				}
				else
				{
					selectedDynamicTemplate = 0;
					iml_tiles->setSelection(-1);
				}
			}
			else
			{
				if( level )
				{
					selectedLevelDynamic = level->dynamic_select( mousePos - global_position );
					mouseDragPoint = mousePos;
				}
			}
		}
		if( editState == ES_COLLISIONS )
		{
			switch( iml_tiles->getSelectionID() )
			{
			case -1:
				{
					if( selectedCollision )
					{
						int corner = selectedCollision->selectPoint(mousePos - global_position);
						if( corner == -1 )
						{
							CollisionShape* shape = level->collision_select(mousePos - global_position);
							if( shape != selectedCollision )
							{
								if( shape )
									shape->selected = true;
								selectedCollision->selected = false;
								selectedCollision = shape;
							}
						}
						selectedCollisionCorner = corner;
					}
					else
					{
						if( level )
						{
							CollisionShape* shape = level->collision_select(mousePos - global_position);
							if( shape )
								shape->selected = true;
							selectedCollision = shape;
						}
					}
				}
				break;
			case 0:	// SQUARE
				{
					CollisionSquare* shape = new CollisionSquare();
					if( selectedCollision )
						selectedCollision->selected = 0;
					selectedCollision = shape;
					selectedCollisionCorner = 2;
					selectedCollision->selected = true;

					mouseDragPoint = mousePos;
					vector2f point = grid->getSnapPoint( mousePos ) - global_position;

					shape->bounds = hgeRect(point.x, point.y, point.x, point.y);

					level->collision_add(shape);
				}
				break;
			case 1:	// TRIANGLE
				{
					CollisionTriangle* shape = new CollisionTriangle();
					if( selectedCollision )
						selectedCollision->selected = 0;
					selectedCollision = shape;
					selectedCollisionCorner = 2;
					selectedCollision->selected = true;

					mouseDragPoint = mousePos;
					shape->setPointPos(0, grid->getSnapPoint( mousePos ) - global_position);
					shape->setPointPos(1, grid->getSnapPoint( mousePos ) - global_position - vector2f(0.0f, (float)grid->gridStep));
					shape->setPointPos(2, grid->getSnapPoint( mousePos ) - global_position);

					level->collision_add(shape);
				}
				break;
			case 2:	// CIRCLE
				{
					CollisionCircle* shape = new CollisionCircle();
					if( selectedCollision )
						selectedCollision->selected = 0;
					selectedCollision = shape;
					selectedCollisionCorner = 1;
					selectedCollision->selected = true;

					mouseDragPoint = mousePos;
					shape->setPointPos(0, grid->getSnapPoint( mousePos ) - global_position);
					shape->setPointPos(2, grid->getSnapPoint( mousePos ) - global_position);

					level->collision_add(shape);
				}
				break;
			}
			iml_tiles->setSelection(-1);
		}
	}

	if( hge->Input_GetKeyState(HGEK_LBUTTON) && !gui_system->mouse_over_ui )
	{
		vector2f mouseMov;
		mouseMov = mousePos - mouseDragPoint;
		mouseDragPoint = mousePos;
		if( editState == ES_STATICS )
		{
			if( selectedLevelObject )
			{
				selectedLevelObject->position = mousePos - vector2f( selectedLevelObject->bounds.x2 / 2, -selectedLevelObject->bounds.y2 / 2 );
				selectedLevelObject->position = grid->getSnapPoint( selectedLevelObject->position ) - global_position - vector2f(0, selectedLevelObject->bounds.y2);
			}
		}
		if( editState == ES_BACKGROUND )
		{
			if( selectedBackgroundObject )
			{
				selectedBackgroundObject->position = mousePos - vector2f( selectedBackgroundObject->bounds.x2 / 2, -selectedBackgroundObject->bounds.y2 / 2 );
				selectedBackgroundObject->position = grid->getSnapPoint( selectedBackgroundObject->position ) - global_position - vector2f(0, selectedBackgroundObject->bounds.y2);
			}
		}
		if( editState == ES_FOREGROUND )
		{
			if( selectedForegroundObject )
			{
				selectedForegroundObject->position = mousePos - vector2f( selectedForegroundObject->bounds.x2 / 2, -selectedForegroundObject->bounds.y2 / 2 );
				selectedForegroundObject->position = grid->getSnapPoint( selectedForegroundObject->position ) - global_position - vector2f(0, selectedForegroundObject->bounds.y2);
			}
		}
		if( editState == ES_DYNAMICS )
		{
			if( selectedLevelDynamic )
			{
				selectedLevelDynamic->position = mousePos - vector2f( selectedLevelDynamic->bounds.x2 / 2, -selectedLevelDynamic->bounds.y2 / 2 );
				selectedLevelDynamic->position = grid->getSnapPoint( selectedLevelDynamic->position ) - global_position - vector2f(0, selectedLevelDynamic->bounds.y2);
			}
		}
		if( editState == ES_COLLISIONS )
		{
			if( selectedCollision )
			{
				vector2f pos = grid->getSnapPoint(mousePos);
				if( selectedCollisionCorner >= 0 )
				{
					vector2f mouse = grid->getSnapPoint(mousePos) - global_position;
					selectedCollision->setPointPos(selectedCollisionCorner, mouse);
				}
			}
		}
	}

	if( hge->Input_KeyUp(HGEK_LBUTTON) )
	{
		if( editState == ES_STATICS )
		{
			selectedLevelObject = 0;
		}
		if( editState == ES_BACKGROUND )
		{
			selectedBackgroundObject = 0;
		}
		if( editState == ES_FOREGROUND )
		{
			selectedForegroundObject = 0;
		}
		if( editState == ES_COLLISIONS )
		{
			//if( selectedCollision )
			
		}
		if( editState == ES_DYNAMICS )
			selectedLevelDynamic = 0;
	}

	if( hge->Input_KeyUp( HGEK_O ) )
	{
		if( selectedStaticTemplate || selectedBackgroundTemplate || selectedForegroundTemplate )
		{
			selected_rotation += PI / 2.0f;
			if( abs(selected_rotation - PI * 2.0f) < 0.0005f )
				selected_rotation = 0.0f;

			GameStaticObject* selected_object;
			if( selectedStaticTemplate )
				selected_object = selectedStaticTemplate;
			else if( selectedBackgroundTemplate )
				selected_object = selectedBackgroundTemplate;
			else if( selectedForegroundTemplate )
				selected_object = selectedForegroundTemplate;

			hgeRect basic_bounds = hgeRect(0.0f, 0.0f, selected_object->sprite->GetWidth() * selected_object->scale, selected_object->sprite->GetHeight() * selected_object->scale);
			hgeRect new_bounds;

			if( abs(selected_rotation) < 0.0005f )
			{
				new_bounds = basic_bounds;
			}
			else if( abs(selected_rotation - PI_2) < 0.0005f )
			{
				new_bounds = hgeRect( -basic_bounds.y2, 0.0f, 0.0f, basic_bounds.x2 );
			}
			else if( abs(selected_rotation - PI) < 0.0005f )
			{
				new_bounds = hgeRect( -basic_bounds.x2, -basic_bounds.y2, 0.0f , 0.0f );
			}
			else if( abs(selected_rotation - (PI + PI_2)) < 0.0005f )
			{
				new_bounds = hgeRect( 0.0f, -basic_bounds.x2, basic_bounds.y2, 0.0f );
			}

			selected_object->bounds = new_bounds;
		}
	}

	if( hge->Input_KeyUp( HGEK_I ) )
	{
		if( selectedStaticTemplate || selectedBackgroundTemplate || selectedForegroundTemplate )
		{
			selected_rotation -= PI / 2.0f;
			if( abs(selected_rotation + PI_2) < 0.0005f )
				selected_rotation = PI + PI_2;

			GameStaticObject* selected_object;
			if( selectedStaticTemplate )
				selected_object = selectedStaticTemplate;
			else if( selectedBackgroundTemplate )
				selected_object = selectedBackgroundTemplate;
			else if( selectedForegroundTemplate )
				selected_object = selectedForegroundTemplate;

			hgeRect basic_bounds = hgeRect(0.0f, 0.0f, selected_object->sprite->GetWidth() * selected_object->scale, selected_object->sprite->GetHeight() * selected_object->scale);
			hgeRect new_bounds;

			if( abs(selected_rotation) < 0.0005f )
			{
				new_bounds = basic_bounds;
			}
			else if( abs(selected_rotation - PI_2) < 0.0005f )
			{
				new_bounds = hgeRect( -basic_bounds.y2, 0.0f, 0.0f, basic_bounds.x2 );
			}
			else if( abs(selected_rotation - PI) < 0.0005f )
			{
				new_bounds = hgeRect( -basic_bounds.x2, -basic_bounds.y2, 0.0f , 0.0f );
			}
			else if( abs(selected_rotation - (PI + PI_2)) < 0.0005f )
			{
				new_bounds = hgeRect( 0.0f, -basic_bounds.x2, basic_bounds.y2, 0.0f );
			}

			selected_object->bounds = new_bounds;
		}
	}

	if( hge->Input_KeyUp( HGEK_L ) )
	{
		if( selectedStaticTemplate || selectedBackgroundTemplate || selectedForegroundTemplate )
			selected_hflip = !selected_hflip;		
	}

	if( hge->Input_KeyUp( HGEK_K ) )
	{
		if( selectedStaticTemplate || selectedBackgroundTemplate || selectedForegroundTemplate )
			selected_vflip = !selected_vflip;		
	}
}
//-----------------------------------------------------------------------------