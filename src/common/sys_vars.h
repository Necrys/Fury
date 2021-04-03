#pragma once

#include <hgerect.h>
#include "types.h"

//-----------------------------------------------------------------------------

extern	uint32		screen_width;
extern	uint32		screen_height;
extern	uint8		screen_bpp;
extern	bool		windowed;
extern	bool		show_fps;
const	uint32		FPS = 60;
extern	bool		vsync;
//-----------------------------------------------------------------------------

extern  vector2f	global_position;
extern	vector2f	camera_position;
extern	bool		game_pause;
extern	bool		frame_tick;
extern	hgeRect		camera_rect;
extern	bool		debug_render;
//-----------------------------------------------------------------------------

const	float		log_msg_lifetime = 3.0f;
const	float		log_msg_fadetime = 1.0f;
const	uint32		log_msg_count	 = 20;
//-----------------------------------------------------------------------------

const	uint32		fps_extra_low_threshold = 30;
const	uint32		fps_low_threshold		= 50;
//-----------------------------------------------------------------------------

extern	hgeRect		phys_world_bounds;
extern	vector2f	phys_gravity;
const	uint32		phys_max_contacts	= 100;
const	uint32		phys_hz				= 60;
const	uint32		phys_iterations		= 10;
const	float		phys_norma			= 100.0f;

const	float		basicFriction		= 0.6f;
const	float		basicRestitution	= 0.0f;

const	float		staticDensity		= 0.0f;
const	float		dynamicDensity		= 0.2f;

const	float		staticFriction		= 0.6f;
const	float		dynamicFriction		= 0.4f;

const	float		staticRestitution	= 0.0f;
const	float		dynamicRestitution	= 0.1f;


const	float		fireballPower		= 3.0f;
//-----------------------------------------------------------------------------