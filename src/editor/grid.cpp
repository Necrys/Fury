/*=============================================================================

	The Fury

DESCRIPTION:
	Snap to grid controller

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
14/05/2010	initial version
=============================================================================*/


#include "system_interfaces.h"
#include "sys_vars.h"
#include "system_ui.h"
#include "grid.h"
#include "system_main.h"

//-----------------------------------------------------------------------------

unsigned steps[] = {0,  1,  2,  4,  8, 12, 16, 24, 32, 48, 64, 96,128,160,256};
unsigned helper[] ={0,  0, 32, 32, 96, 96, 64, 96, 64, 96,128,  0,  0,  0,  0};

//-----------------------------------------------------------------------------

SnapGrid::SnapGrid()
{
	currentStep	= 9;
	stepsCount = sizeof(steps) / sizeof( unsigned );
	gridColor	= 0x3300FF00;
	originGridColor = 0x3300FFFF;
	enabled		= true;
	visible		= true;
	firstPoint	= vector2f(0.0f, 0.0f);
	gridStep	= steps[currentStep];
}
//-----------------------------------------------------------------------------

vector2f SnapGrid::getSnapPoint( const vector2f& point ) const
{
	if( !enabled )
		return point;

	if( gridStep < 2 )
		return point;

	vector2f min_point = firstPoint;
	float min_dist = (min_point - point).Length();

	unsigned hcounter = (unsigned)firstPoint.x;
	unsigned vcounter;
	while( hcounter < screen_width )
	{
		vcounter = (unsigned)firstPoint.y;
		while( vcounter < screen_height )
		{
			vector2f gridPoint = vector2f((float)hcounter, (float)vcounter);
			float dist = (gridPoint - point).Length();
			if( dist < min_dist )
			{
				min_point = gridPoint;
				min_dist = dist;
			}
			vcounter += gridStep;
		}
		hcounter += gridStep;
	}
	return min_point;
}
//-----------------------------------------------------------------------------

void SnapGrid::update( const float &deltaTime )
{
	if( gridStep < 2 )
		return;

	if( global_position.x >= 0 )
		firstPoint.x = (float)((long)global_position.x % gridStep);
	else
		firstPoint.x = (float)(gridStep - abs((long)global_position.x) % gridStep);

	if( global_position.y >= 0 )
		firstPoint.y = (float)((long)global_position.y % gridStep);
	else
		firstPoint.y = (float)(gridStep - abs((long)global_position.y) % gridStep);
}
//-----------------------------------------------------------------------------

void SnapGrid::render()
{
	if( gridStep < 2 )
		return;

	if( visible )
	{
		unsigned point = (unsigned)firstPoint.x;
		while( point < screen_width )
		{
			hge->Gfx_RenderLine((float)point, 0.0f, (float)point, (float)screen_height, gridColor);
			point += gridStep;
		}
		point = (unsigned)firstPoint.y;
		while( point < screen_height )
		{
			hge->Gfx_RenderLine(0.0f, (float)point, (float)screen_width, (float)point, gridColor);
			point += gridStep;
		}

		if( helper[currentStep] > 0 )
		{
			vector2f fpoint;
			if( global_position.x >= 0 )
				fpoint.x = (float)((long)global_position.x % helper[currentStep]);
			else
				fpoint.x = (float)(helper[currentStep] - abs((long)global_position.x) % helper[currentStep]);

			if( global_position.y >= 0 )
				fpoint.y = (float)((long)global_position.y % helper[currentStep]);
			else
				fpoint.y = (float)(helper[currentStep] - abs((long)global_position.y) % helper[currentStep]);

			point = (unsigned)fpoint.x;
			while( point < screen_width )
			{
				hge->Gfx_RenderLine((float)point, 0.0f, (float)point, (float)screen_height, originGridColor);
				point += helper[currentStep];
			}
			point = (unsigned)fpoint.y;
			while( point < screen_height )
			{
				hge->Gfx_RenderLine(0.0f, (float)point, (float)screen_width, (float)point, originGridColor);
				point += helper[currentStep];
			}
		}
	}
}
//-----------------------------------------------------------------------------

void SnapGrid::increaseStep()
{
	if( currentStep == stepsCount-1 )
		return;

	currentStep++;
	gridStep	= steps[currentStep];
}
//-----------------------------------------------------------------------------

void SnapGrid::decreaseStep()
{
	if( currentStep == 0 )
		return;

	currentStep--;
	gridStep	= steps[currentStep];
}
//-----------------------------------------------------------------------------