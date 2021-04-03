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

#pragma once

#include "types.h"

//-----------------------------------------------------------------------------

class SnapGrid
{
public:
	SnapGrid();
	~SnapGrid(){};

	unsigned	gridStep;
	uint32		gridColor;
	uint32		originGridColor;
	bool		enabled;
	bool		visible;

	vector2f	getSnapPoint( const vector2f& point ) const;
	void		update( const float& deltaTime );
	void		render();
	void		increaseStep();
	void		decreaseStep();

	vector2f	firstPoint;

private:
	unsigned	stepsCount;
	unsigned	currentStep;
};
//-----------------------------------------------------------------------------
