#pragma once

//-----------------------------------------------------------------------------

#include "box2d/Box2D.h"
//-----------------------------------------------------------------------------

typedef signed	__int8		int8;
typedef signed	__int16		int16;
typedef signed	__int32		int32;
typedef signed	__int64		int64;

typedef unsigned __int8		uint8;
typedef unsigned __int16	uint16;
typedef unsigned __int32	uint32;
typedef unsigned __int64	uint64;
//-----------------------------------------------------------------------------

static const float FLOAT_EPSILON = 0.0001f;
static const float PI = 3.1415f;
static const float PI_2 = PI / 2.0f;
static const float PI_4 = PI / 4.0f;
//-----------------------------------------------------------------------------

#define COLOR_MESSAGE	0xFFFFFFFF
#define COLOR_SUCCESS	0xFF00FF00
#define COLOR_WARNING	0xFFFFFF00
#define COLOR_ERROR		0xFFFF6666
#define COLOR_CRITICAL	0xFFFF0000
//-----------------------------------------------------------------------------

enum OBJ_CATEGORY
{
	OBJCAT_STATIC = 0x0001,
	OBJCAT_DYNAMIC = 0x0002,
	OBJCAT_PLAYER = 0x0004,
	OBJCAT_FIREBALL = 0x0008,
};
//-----------------------------------------------------------------------------