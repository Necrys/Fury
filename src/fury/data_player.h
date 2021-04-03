/*=============================================================================

	The Fury

DESCRIPTION:
	player class

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
21/05/2010	initial version
=============================================================================*/

#pragma once

#include "types.h"
#include <hgeparticle.h>
#include "data_staticobject.h"
#include "data_objectdata.h"
#include "data_dynamic_crunch.h"
#include "rope.h"

//-----------------------------------------------------------------------------

enum PLAYER_MODE
{
	PLAYER_MODE_YELLOW	= 0,
	PLAYER_MODE_RED		= 1,
	PLAYER_MODE_GREEN	= 2,
	PLAYER_MODE_BLUE	= 3,
};
//-----------------------------------------------------------------------------

enum ANIMATIONS
{
	ANIM_STAND = 0,
	ANIM_WALK_LEFT = 1,
	ANIM_WALK_RIGHT = 2,
	ANIM_TURN_LEFT = 3,
	ANIM_TURN_RIGHT = 4,

	//	RED SPECIFIC
	ANIM_CRUNCH_DOWN = 5,
	ANIM_CRUNCH_LEFT = 6,
	ANIM_CRUNCH_RIGHT = 7,

	//  YELLOW SPECIFIC
	ANIM_ENERGY_GROW_LEFT = 5,
	ANIM_ENERGY_GROW_RIGHT = 6,
	ANIM_SHOT_LEFT = 7,
	ANIM_SHOT_RIGHT = 8,
	ANIM_WAIT_SHOT_LEFT = 9,
	ANIM_WAIT_SHOT_RIGHT = 10,

	ANIM_PUSH_RIGHT	= 11,
	ANIM_PUSH_LEFT	= 12,

	// green specific
	ANIM_GREEN_ROPE = 5,

	ANIM_COUNT = 20,
};

class GamePlayer: public GameStaticObject
{
public:
	GamePlayer();
	~GamePlayer();

	hgeAnimation*	animations[4][ANIM_COUNT];

	// physics
	b2Body*		body;

	bool		onGround;
	bool		inWater;
	bool		impulse;
	bool		lastGroundJump;
	bool		action;

	float		jump_timer;

	float		fireballPower;

	vector2f	groundNormal;
	vector2f	direction;

	PLAYER_MODE		mode;
	ANIMATIONS		currentAnim;
	bool			modeUnavailiable;
	bool			modeSelection;
	bool			move;
	bool			lastFrameMove;

	hgeParticleSystem*	selectionEffect;
	float			startSelectionEffectTimer;
	float			endSelectionEffectTimer;

	GameCrunch*		crunchContacts[3];	// 0 - left, 1 - right, 2 - down

	bool	contactLeft;
	bool	contactRight;

	bool	keys[256];

	unsigned int coins;

	ObjectData	data;

	rope	rope_object;

	virtual void update( const float& delta );

	virtual void setPosition( const vector2f& pos );

	virtual void onCollision( b2Fixture* fix, contact_point* point );

	virtual void moveLeft();

	virtual void moveRight();

	virtual void jump();

	virtual void jumpBounce();

	virtual void setMode( const PLAYER_MODE& mode );

	virtual void render();

	virtual void toggleSelection();

	virtual void selection( const bool& left );

	virtual void shotFireball( const float& power );

	virtual void actionBegin();

	virtual void actionEnd();

	void draw_player_data();

private:
	virtual PLAYER_MODE getNextMode( const PLAYER_MODE& current, const bool& left );

	virtual void setAnimation( const ANIMATIONS& anim );
};