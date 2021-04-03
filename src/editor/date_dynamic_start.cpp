/*=============================================================================

	The Fury

DESCRIPTION:
	start point object

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
22/05/2010	initial version
=============================================================================*/


#include <hgeanim.h>
#include "data_dynamic_start.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "data_level.h"

//-----------------------------------------------------------------------------
GameStartPoint::GameStartPoint()
{
	type = DYNAMIC_START_POINT;

	texture = icon = textureManager->getTexture( Path::gfxcommon + "player\\fury.tga" );
	if( !texture )
		return;

	sprite = new hgeAnimation(texture, 1, 1.0f, 0.0f, 0.0f, 64.0f, 64.0f);
	if( !sprite )
		return;

	scale = 48.0f / 64.0f;

	bounds = hgeRect(0.0f, 0.0f, 64.0f * scale, 64.0f * scale);
}
//-----------------------------------------------------------------------------

GameDynamicObject* GameStartPoint::clone()
{
	GameStartPoint* cloned = new GameStartPoint();
	cloned->position = position;
	return (GameDynamicObject*)cloned;
};
//-----------------------------------------------------------------------------

void GameStartPoint::serialize(FILE *stream)
{
	if( !stream )
		return;

	struct start_point_struct
	{
		unsigned int	type;
		unsigned int	obj_type;
		float			position_x;
		float			position_y;
	} obj;
	obj.type = FLF_SECTION_DYNAMIC;
	obj.obj_type = DYNAMIC_START_POINT;
	obj.position_x = position.x;
	obj.position_y = position.y;

	fwrite(&obj, sizeof(start_point_struct), 1, stream);
}
//-----------------------------------------------------------------------------
