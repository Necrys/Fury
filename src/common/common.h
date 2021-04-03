#pragma once
//-----------------------------------------------------------------------------

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_NO_WARNINGS  1

#include <windows.h>
#include "types.h"

#define ASSERT( X )    ( (void) ( (X) || ( __debugbreak(), 1 ) ) )
#define INLINE         inline
#define RESTRICT       __restrict
#define LOG( format, ... ) hge->System_Log( format, __VA_ARGS__ )
//-----------------------------------------------------------------------------