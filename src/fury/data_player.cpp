#include "types.h"
#include <hgeanim.h>
#include "data_player.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "sys_vars.h"
#include "helper_functions.h"
#include "data_fireball.h"
#include "data_dynamic_door.h"
#include "data_dynamic_trigger.h"
#include "data_item_key.h"
#include "data_item_coin.h"
#include "system_main.h"
#include "hgefont.h"
#include <list>

using namespace std;
//-----------------------------------------------------------------------------

list<contact_point*> player_contacts;

const float playerDensity			= 8.025f;
const float playerFriction			= 0.4f;
const float playerRestitution		= 0.2f;
const float playerUpImpulse			= 5.95f;
const float playerSideImpulse		= 0.333f;
const float playerSideInAirImpulse	= 0.14f;
const float playerAppliedForce		= 16.0f;
//-----------------------------------------------------------------------------

GamePlayer::GamePlayer()
{
	texture = textureManager->getTexture( Path::particles + "particles.png" );
	hgeSprite* particleSpr = new hgeSprite( texture, 32, 64, 32, 32 );
	particleSpr->SetBlendMode( BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE );
	particleSpr->SetHotSpot(16, 16);

	selectionEffect = new hgeParticleSystem( (Path::particles + "selection.psi").c_str(), particleSpr );

	/* setting physics data */
	b2BodyDef		bodyDef;
	b2CircleShape	shape;
	b2FixtureDef	fix;
	b2Fixture*		f;

	bodyDef.position.Set( position.x / phys_norma, position.y / phys_norma);
	bodyDef.angularDamping = 10000.0f;
	bodyDef.linearDamping = 0.4f;
	bodyDef.bullet = true;
	bodyDef.allowSleep = false;
	bodyDef.type = b2_dynamicBody;
	body = box2d->CreateBody(&bodyDef);

	shape.m_radius = 20.0f / phys_norma;

	fix.density = playerDensity;
	fix.restitution = playerRestitution;
	fix.friction = playerFriction;
	fix.filter.categoryBits = OBJCAT_PLAYER;
	fix.filter.maskBits = OBJCAT_DYNAMIC | OBJCAT_STATIC;
	fix.shape = &shape;

	f = body->CreateFixture(&fix);
	f->SetUserData(&data);

	b2Vec2 pos = body->GetPosition();
	position.x = pos.x * phys_norma;
	position.y = pos.y * phys_norma;

	data.objectPtr	= (void*)this;
	data.type		= OBJECT_PLAYER;

	onGround		= false;
	inWater			= false;
	impulse			= false;
	lastGroundJump	= false;
	action			= false;
	jump_timer		= 0.0f;

	modeSelection = false;
	modeUnavailiable = false;

	endSelectionEffectTimer = 0.0f;
	startSelectionEffectTimer = 0.0f;

	direction = vector2f(1.0f, 0.0f);

	fireballPower = 0.0f;

	crunchContacts[0] = 0;
	crunchContacts[1] = 0;
	crunchContacts[2] = 0;

	move = false;
	lastFrameMove = false;

//	animations[ANIM_STAND] = 
	memset(animations[PLAYER_MODE_YELLOW], 0, sizeof(hgeAnimation*) * ANIM_COUNT);
	memset(animations[PLAYER_MODE_RED], 0, sizeof(hgeAnimation*) * ANIM_COUNT);
	memset(animations[PLAYER_MODE_GREEN], 0, sizeof(hgeAnimation*) * ANIM_COUNT);
	memset(animations[PLAYER_MODE_BLUE], 0, sizeof(hgeAnimation*) * ANIM_COUNT);

	texture = textureManager->getTexture( Path::gfxcommon + "player\\" + "yellow.tga" );
	animations[PLAYER_MODE_YELLOW][ANIM_STAND] = new hgeAnimation(texture, 1, 1.0f, 0.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_STAND]->SetHotSpot(24.0f, 27.0f);

	texture = textureManager->getTexture( Path::gfxcommon + "player\\" + "yellow-anim.tga" );
	animations[PLAYER_MODE_YELLOW][ANIM_PUSH_RIGHT] = new hgeAnimation(texture, 10, 18.0f, 0.0f, 96.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_PUSH_RIGHT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_YELLOW][ANIM_PUSH_LEFT] = new hgeAnimation(texture, 10, 18.0f, 480.0f, 96.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_PUSH_LEFT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_YELLOW][ANIM_TURN_LEFT] = new hgeAnimation(texture, 3, 50.0f, 480.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_TURN_LEFT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_TURN_LEFT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_YELLOW][ANIM_TURN_RIGHT] = new hgeAnimation(texture, 3, 50.0f, 480.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_TURN_RIGHT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_TURN_RIGHT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_YELLOW][ANIM_ENERGY_GROW_LEFT] = new hgeAnimation(texture, 4, 20.0f, 624.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_ENERGY_GROW_LEFT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_ENERGY_GROW_LEFT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_YELLOW][ANIM_ENERGY_GROW_RIGHT] = new hgeAnimation(texture, 4, 20.0f, 624.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_ENERGY_GROW_RIGHT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_ENERGY_GROW_RIGHT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_YELLOW][ANIM_SHOT_LEFT] = new hgeAnimation(texture, 4, 20.0f, 816.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_SHOT_LEFT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_SHOT_LEFT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_YELLOW][ANIM_SHOT_RIGHT] = new hgeAnimation(texture, 4, 20.0f, 816.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_SHOT_RIGHT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_SHOT_RIGHT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_YELLOW][ANIM_WAIT_SHOT_LEFT] = new hgeAnimation(texture, 1, 1.0f, 768.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_WAIT_SHOT_LEFT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_WAIT_SHOT_LEFT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_YELLOW][ANIM_WAIT_SHOT_RIGHT] = new hgeAnimation(texture, 1, 1.0f, 768.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_WAIT_SHOT_RIGHT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_WAIT_SHOT_RIGHT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_YELLOW][ANIM_WALK_LEFT] = new hgeAnimation(texture, 10, 30.0f, 0.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_WALK_LEFT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_YELLOW][ANIM_WALK_RIGHT] = new hgeAnimation(texture, 10, 30.0f, 0.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_YELLOW][ANIM_WALK_RIGHT]->SetHotSpot(24.0f, 23.0f);

	texture = textureManager->getTexture( Path::gfxcommon + "player\\" + "green.tga" );
	animations[PLAYER_MODE_GREEN][ANIM_STAND] = new hgeAnimation(texture, 1, 1.0f, 0.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_GREEN][ANIM_STAND]->SetHotSpot(24.0f, 27.0f);

	texture = textureManager->getTexture( Path::gfxcommon + "player\\" + "green-anim.tga" );
	animations[PLAYER_MODE_GREEN][ANIM_TURN_LEFT] = new hgeAnimation(texture, 3, 50.0f, 480.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_GREEN][ANIM_TURN_LEFT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_GREEN][ANIM_TURN_LEFT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_GREEN][ANIM_PUSH_RIGHT] = new hgeAnimation(texture, 10, 18.0f, 0.0f, 96.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_GREEN][ANIM_PUSH_RIGHT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_GREEN][ANIM_PUSH_LEFT] = new hgeAnimation(texture, 10, 18.0f, 480.0f, 96.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_GREEN][ANIM_PUSH_LEFT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_GREEN][ANIM_TURN_RIGHT] = new hgeAnimation(texture, 3, 50.0f, 480.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_GREEN][ANIM_TURN_RIGHT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_GREEN][ANIM_TURN_RIGHT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_GREEN][ANIM_WALK_LEFT] = new hgeAnimation(texture, 10, 30.0f, 0.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_GREEN][ANIM_WALK_LEFT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_GREEN][ANIM_WALK_RIGHT] = new hgeAnimation(texture, 10, 30.0f, 0.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_GREEN][ANIM_WALK_RIGHT]->SetHotSpot(24.0f, 23.0f);

	texture = textureManager->getTexture( Path::gfxcommon + "player\\" + "red.tga" );
	animations[PLAYER_MODE_RED][ANIM_STAND] = new hgeAnimation(texture, 1, 1.0f, 0.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_RED][ANIM_STAND]->SetHotSpot(24.0f, 27.0f);

	texture = textureManager->getTexture( Path::gfxcommon + "player\\" + "red-anim.tga" );
	animations[PLAYER_MODE_RED][ANIM_TURN_LEFT] = new hgeAnimation(texture, 3, 50.0f, 480.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_RED][ANIM_TURN_LEFT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_RED][ANIM_TURN_LEFT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_RED][ANIM_PUSH_RIGHT] = new hgeAnimation(texture, 10, 18.0f, 0.0f, 96.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_RED][ANIM_PUSH_RIGHT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_RED][ANIM_PUSH_LEFT] = new hgeAnimation(texture, 10, 18.0f, 480.0f, 96.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_RED][ANIM_PUSH_LEFT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_RED][ANIM_TURN_RIGHT] = new hgeAnimation(texture, 3, 50.0f, 480.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_RED][ANIM_TURN_RIGHT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_RED][ANIM_TURN_RIGHT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_RED][ANIM_CRUNCH_DOWN] = new hgeAnimation(texture, 4, 30.0f, 816.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_RED][ANIM_CRUNCH_DOWN]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_RED][ANIM_CRUNCH_DOWN]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_RED][ANIM_CRUNCH_LEFT] = new hgeAnimation(texture, 4, 30.0f, 624.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_RED][ANIM_CRUNCH_LEFT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_RED][ANIM_CRUNCH_LEFT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_RED][ANIM_CRUNCH_RIGHT] = new hgeAnimation(texture, 4, 30.0f, 624.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_RED][ANIM_CRUNCH_RIGHT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_RED][ANIM_CRUNCH_RIGHT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_RED][ANIM_WALK_LEFT] = new hgeAnimation(texture, 10, 30.0f, 0.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_RED][ANIM_WALK_LEFT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_RED][ANIM_WALK_RIGHT] = new hgeAnimation(texture, 10, 30.0f, 0.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_RED][ANIM_WALK_RIGHT]->SetHotSpot(24.0f, 23.0f);

	texture = textureManager->getTexture( Path::gfxcommon + "player\\" + "blue.tga" );
	animations[PLAYER_MODE_BLUE][ANIM_STAND] = new hgeAnimation(texture, 1, 1.0f, 0.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_BLUE][ANIM_STAND]->SetHotSpot(24.0f, 27.0f);

	texture = textureManager->getTexture( Path::gfxcommon + "player\\" + "blue-anim.tga" );
	animations[PLAYER_MODE_BLUE][ANIM_TURN_LEFT] = new hgeAnimation(texture, 3, 50.0f, 480.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_BLUE][ANIM_TURN_LEFT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_BLUE][ANIM_TURN_LEFT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_BLUE][ANIM_PUSH_RIGHT] = new hgeAnimation(texture, 10, 18.0f, 0.0f, 96.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_BLUE][ANIM_PUSH_RIGHT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_BLUE][ANIM_PUSH_LEFT] = new hgeAnimation(texture, 10, 18.0f, 480.0f, 96.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_BLUE][ANIM_PUSH_LEFT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_BLUE][ANIM_TURN_RIGHT] = new hgeAnimation(texture, 3, 50.0f, 480.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_BLUE][ANIM_TURN_RIGHT]->SetHotSpot(24.0f, 23.0f);
	animations[PLAYER_MODE_BLUE][ANIM_TURN_RIGHT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );

	animations[PLAYER_MODE_BLUE][ANIM_WALK_LEFT] = new hgeAnimation(texture, 10, 30.0f, 0.0f, 48.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_BLUE][ANIM_WALK_LEFT]->SetHotSpot(24.0f, 23.0f);

	animations[PLAYER_MODE_BLUE][ANIM_WALK_RIGHT] = new hgeAnimation(texture, 10, 30.0f, 0.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_BLUE][ANIM_WALK_RIGHT]->SetHotSpot(24.0f, 23.0f);

	texture = textureManager->getTexture(Path::gfxcommon + "player\\" + "green_rope.tga");
	animations[PLAYER_MODE_GREEN][ANIM_GREEN_ROPE] = new hgeAnimation(texture, 1, 1.0f, 0.0f, 0.0f, 48.0f, 48.0f);
	animations[PLAYER_MODE_GREEN][ANIM_GREEN_ROPE]->SetHotSpot(24.0f, 23.0f);

	memset(keys, 0, sizeof(bool) * 256);
	keys[255] = true;
	coins = 0;

	setMode(PLAYER_MODE_YELLOW);
	setAnimation(ANIM_STAND);
}
//-----------------------------------------------------------------------------

GamePlayer::~GamePlayer()
{
	for( int i=0; i<4; i++ )
		for( int j=0; j<ANIM_COUNT; j++ )
			if( animations[i][j] )
				delete animations[i][j];
}
//-----------------------------------------------------------------------------

void GamePlayer::update( const float &delta )
{
	GameStaticObject::update(delta);

	if( jump_timer > 0.0f )
		jump_timer -= delta;

//	if( !modeSelection )
//	{
		b2Vec2 pos = body->GetPosition();
		position.x = pos.x * phys_norma;
		position.y = pos.y * phys_norma;
//	}
//	else
//	{
//		body->SetXForm( b2Vec2(position.x / physicsNorma, position.y/physicsNorma), 0.0f );
//		body->SetLinearVelocity( b2Vec2(0.0f, 0.0f) );
//		body->SetAngularVelocity(0.0f);
//	}

	b2Vec2 vel = body->GetLinearVelocity();
	if( currentAnim == ANIM_WALK_LEFT || currentAnim == ANIM_WALK_RIGHT )
	{
		if( fabs(vel.x) < 2.0f )
			vel.x = 2.0f;
		sprite->SetSpeed( 10 * fabs(vel.x) );
	}

	if( body->IsAwake() )
		onGround = true;

	float d = delta;
	if( startSelectionEffectTimer > 0.0f )
	{
		d = delta  + delta * 2.0f * (startSelectionEffectTimer / 0.5f);
		startSelectionEffectTimer -= delta;
	}

	if( endSelectionEffectTimer > 0.0f )
	{
		d = delta * 20;
		endSelectionEffectTimer -= delta;
		if( endSelectionEffectTimer <= 0.0f )
			modeSelection = false;
	}
	selectionEffect->Update(d);
	
	if( action )
	{
		if( mode == PLAYER_MODE_YELLOW )
		{
			fireballPower += delta;
			if( fireballPower > 1.0f )
				fireballPower = 1.0f;
		}
	}

	if( mode == PLAYER_MODE_RED )
	if( currentAnim == ANIM_CRUNCH_DOWN || currentAnim == ANIM_CRUNCH_LEFT || currentAnim == ANIM_CRUNCH_RIGHT )
	{
		if( !sprite->IsPlaying() )
			setAnimation(ANIM_STAND);
	}

	if( mode == PLAYER_MODE_YELLOW )
	{
		if( currentAnim == ANIM_SHOT_LEFT || currentAnim == ANIM_SHOT_RIGHT )
			if( !sprite->IsPlaying() )
				setAnimation(ANIM_STAND);
		if( currentAnim == ANIM_ENERGY_GROW_LEFT)
			if( !sprite->IsPlaying() )
				setAnimation(ANIM_WAIT_SHOT_LEFT);
		if( currentAnim == ANIM_ENERGY_GROW_RIGHT)
			if( !sprite->IsPlaying() )
				setAnimation(ANIM_WAIT_SHOT_RIGHT);

		if( currentAnim == ANIM_ENERGY_GROW_LEFT && direction.x > 0.0f )
		{
			int frame = sprite->GetFrame();
			setAnimation( ANIM_ENERGY_GROW_RIGHT );
			sprite->SetFrame(frame);
		}

		if( currentAnim == ANIM_ENERGY_GROW_RIGHT && direction.x < 0.0f )
		{
			int frame = sprite->GetFrame();
			setAnimation( ANIM_ENERGY_GROW_LEFT );
			sprite->SetFrame(frame);
		}

		if( currentAnim == ANIM_WAIT_SHOT_RIGHT && direction.x < 0.0f )
			setAnimation( ANIM_WAIT_SHOT_LEFT );
		if( currentAnim == ANIM_WAIT_SHOT_LEFT && direction.x > 0.0f )
			setAnimation( ANIM_WAIT_SHOT_RIGHT );
	}

	if( !action )
	{
		if( !move )
		{
			if( currentAnim == ANIM_WALK_LEFT || currentAnim == ANIM_PUSH_LEFT )
			{
				if( animations[mode][ANIM_TURN_LEFT] )
					animations[mode][ANIM_TURN_LEFT]->SetMode( HGEANIM_REV | HGEANIM_NOLOOP );
				setAnimation(ANIM_TURN_LEFT);
			}
			else if( currentAnim == ANIM_WALK_RIGHT || currentAnim == ANIM_PUSH_RIGHT )
			{
				if( animations[mode][ANIM_TURN_RIGHT] )
					animations[mode][ANIM_TURN_RIGHT]->SetMode( HGEANIM_REV | HGEANIM_NOLOOP );
				setAnimation(ANIM_TURN_RIGHT);
			}

			if( (currentAnim == ANIM_TURN_LEFT || currentAnim == ANIM_TURN_RIGHT) && !sprite->IsPlaying() )
				setAnimation(ANIM_STAND);
		}
		else
		{
			if( currentAnim == ANIM_STAND )
			{
				if( direction.x > 0.0f )
				{
					if( animations[mode][ANIM_TURN_RIGHT] )
						animations[mode][ANIM_TURN_RIGHT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );
					setAnimation(ANIM_TURN_RIGHT);
				}
				else
				{
					if( animations[mode][ANIM_TURN_LEFT] )
						animations[mode][ANIM_TURN_LEFT]->SetMode( HGEANIM_FWD | HGEANIM_NOLOOP );
					setAnimation(ANIM_TURN_LEFT);
				}
			}
			else if( currentAnim == ANIM_TURN_LEFT || currentAnim == ANIM_TURN_RIGHT )
			{
				if( !sprite->IsPlaying() )
				{
					if( sprite->GetMode() & HGEANIM_REV )
					{
						setAnimation(ANIM_STAND);
					}
					else
					{
						if( direction.x > 0.0f )
							setAnimation(ANIM_WALK_RIGHT);
						else
							setAnimation(ANIM_WALK_LEFT);
					}
				}
			}
			else
			{
				if( direction.x > 0.0f )
				{
					if( currentAnim == ANIM_WALK_LEFT )
					{
						animations[mode][ANIM_TURN_LEFT]->SetMode( HGEANIM_REV | HGEANIM_NOLOOP );
						setAnimation(ANIM_TURN_LEFT);
					}
					else
					{
						if( contactRight && onGround )
							setAnimation(ANIM_PUSH_RIGHT);
						else
							setAnimation(ANIM_WALK_RIGHT);
					}
				}
				else
				{
					if( currentAnim == ANIM_WALK_RIGHT )
					{
						animations[mode][ANIM_TURN_RIGHT]->SetMode( HGEANIM_REV | HGEANIM_NOLOOP );
						setAnimation(ANIM_TURN_RIGHT);
					}
					else
					{
						if( contactLeft && onGround )
							setAnimation(ANIM_PUSH_LEFT);
						else
							setAnimation(ANIM_WALK_LEFT);
					}
				}
			}
		}
	}

	if( mode == PLAYER_MODE_GREEN && currentAnim == ANIM_GREEN_ROPE )
	{
		rotation = rope_object.get_angle();
		log_msg("%f", rotation);
	}
	else
	{
		rotation = 0.0f;
	}

	if( move || impulse )
	{
		crunchContacts[0] = 0;
		crunchContacts[1] = 0;
		crunchContacts[2] = 0;
	}

	contactRight = false;
	contactLeft = false;

	move = false;

	onGround = false;
}
//-----------------------------------------------------------------------------

void GamePlayer::render()
{
	GameStaticObject::render();

	if( selectionEffect )
		selectionEffect->Render();

	rope_object.render();
}
//-----------------------------------------------------------------------------

void GamePlayer::setPosition( const vector2f& pos )
{
	position = pos;
	body->SetTransform( b2Vec2(pos.x / phys_norma, pos.y / phys_norma), rotation );
}
//-----------------------------------------------------------------------------

void GamePlayer::onCollision( b2Fixture* fix, contact_point* point )
{
	ObjectData* data = (ObjectData*)fix->GetUserData();
	if( !data )
		return;

	player_contacts.push_back(point);

	// check for ground
	if(point->state != CS_REMOVED)
	{
		if( !data->isWater && !data->isExitZone && !data->isSensor )
		{
			b2Vec2 b2PlayerCenter = body->GetWorldCenter();
			b2Vec2 b2contactPoint = point->position;
			b2Vec2 b2contactnormal = b2PlayerCenter - b2contactPoint;
			vector2f contactNormal = vector2f(b2contactnormal.x, b2contactnormal.y);
			/* check angle between contact normal and up vector */
			vector2f upVector(0.0f, -1.0f);
			if( contactNormal.Angle(upVector) < M_PI_4 + M_PI_4 / 3 )
			{
				onGround = true;
				contactNormal.Normalize();
				groundNormal = contactNormal;
			}
		}

		if( data->isDynamic )
		{
			b2Vec2 b2circleCenter = body->GetWorldCenter();
			b2Vec2 b2contactPoint = point->position;
			b2Vec2 b2contactnormal = b2circleCenter - b2contactPoint;
			vector2f contactNormal = vector2f(b2contactnormal.x, b2contactnormal.y);
			vector2f upVector(0.0f, -1.0f);
			float angle = contactNormal.Angle(upVector);
			if( angle > 2 * M_PI / 6.0f && angle < M_PI_2 + M_PI / 6.0f )
			{
				b2Vec2 force = b2Vec2(point->normal.x * -playerAppliedForce, 0.0f);
				fix->GetBody()->ApplyForce(force, b2contactPoint);
			}
		}

		if( data->type == OBJECT_CRUNCH )
		{
			b2Vec2 b2circleCenter = body->GetWorldCenter();
			b2Vec2 b2contactPoint = point->position;
			b2Vec2 b2contactnormal = b2circleCenter - b2contactPoint;
			vector2f contactNormal = vector2f(b2contactnormal.x, b2contactnormal.y);
			vector2f upVector(0.0f, -1.0f);
			float angle = contactNormal.Angle(upVector);
			if( (angle > M_PI_4 && angle < M_PI_4 + M_PI_2) || (angle < -M_PI_4 && angle > -M_PI_4 - M_PI_2) )
			{
				if( b2circleCenter.x < b2contactPoint.x )
					crunchContacts[1] = static_cast<GameCrunch*>(data->objectPtr);
				else
					crunchContacts[0] = static_cast<GameCrunch*>(data->objectPtr);
			}
			if( angle < M_PI_4 )
			{
				if( b2circleCenter.y < b2contactPoint.y )
					crunchContacts[2] = static_cast<GameCrunch*>(data->objectPtr);
			}
		}

		if( !data->isSensor )
		{
			b2Vec2 b2circleCenter = body->GetWorldCenter();
			b2Vec2 b2contactPoint = point->position;
			b2Vec2 b2contactnormal = b2circleCenter - b2contactPoint;
			vector2f contactNormal = vector2f(b2contactnormal.x, b2contactnormal.y);
			vector2f upVector(0.0f, -1.0f);
			float angle = contactNormal.Angle(upVector);
			if( (angle > M_PI_4 && angle < M_PI_4 + M_PI_2) || (angle < -M_PI_4 && angle > -M_PI_4 - M_PI_2) )
			{
				if( b2circleCenter.x < b2contactPoint.x )
					contactRight = true;
				else
					contactLeft = true;
			}
		}
	}

	if(point->state == CS_ADDED)
	{
		if( data->type == OBJECT_DOOR )
		{
			GameDoor* door = static_cast<GameDoor*>(data->objectPtr);
			if( keys[door->group] && !door->opened )
				door->toggle();
		}

		if( data->type == OBJECT_KEY )
		{
			GameItemKey* key = static_cast<GameItemKey*>(data->objectPtr);
			key->onCollision();
		}

		if( data->type == OBJECT_COIN )
		{
			GameItemCoin* key = static_cast<GameItemCoin*>(data->objectPtr);
			key->onCollision();
		}

		if( data->type == OBJECT_TRIGGER )
		{
			GameTrigger* trig = static_cast<GameTrigger*>(data->objectPtr);
			trig->toggle();
		}
	}
}
//-----------------------------------------------------------------------------

void GamePlayer::moveLeft()
{
	direction.x = -1.0f;

	if( action && mode == PLAYER_MODE_YELLOW )
		return;

	if( modeSelection )
		return;

	if( onGround )
		body->ApplyLinearImpulse(b2Vec2(-playerSideImpulse * -groundNormal.y, -0.7f * groundNormal.x), body->GetPosition());
	else if( rope_object.is_created() )
		body->ApplyLinearImpulse(b2Vec2(-playerSideInAirImpulse * 1.5f, 0.0f), body->GetPosition());
	else
		body->ApplyLinearImpulse(b2Vec2(-playerSideInAirImpulse, 0.0f), body->GetPosition());

	move = true;
}
//-----------------------------------------------------------------------------

void GamePlayer::moveRight()
{
	direction.x = 1.0f;

	if( action && mode == PLAYER_MODE_YELLOW )
		return;

	if( modeSelection )
		return;

	if( onGround )
		body->ApplyLinearImpulse(b2Vec2(playerSideImpulse * -groundNormal.y, 0.7f * groundNormal.x), body->GetPosition());
	else if( rope_object.is_created() )
		body->ApplyLinearImpulse(b2Vec2(playerSideInAirImpulse * 1.5f, 0.0f), body->GetPosition());
	else
		body->ApplyLinearImpulse(b2Vec2(playerSideInAirImpulse, 0.0f), body->GetPosition());

	move = true;
}
//-----------------------------------------------------------------------------

void GamePlayer::jump()
{
	if( action && mode == PLAYER_MODE_YELLOW )
		return;

	if( modeSelection )
		return;

	if( jump_timer <= 0.0f )
	if(onGround && !inWater)
	{
		b2Vec2 v = body->GetLinearVelocity();
		body->SetLinearVelocity( b2Vec2(v.x, 0) );
		body->ApplyLinearImpulse(b2Vec2(0.0f, -playerUpImpulse), body->GetPosition());
		impulse = true;
		lastGroundJump = true;
		jump_timer = 0.3f;
	}
}
//-----------------------------------------------------------------------------

void GamePlayer::jumpBounce()
{
	if( modeSelection )
		return;

	if( !impulse )
	{
		if(lastGroundJump & onGround && !inWater)
		{
			b2Vec2 v = body->GetLinearVelocity();
			body->SetLinearVelocity( b2Vec2(v.x, 0) );
			body->ApplyLinearImpulse(b2Vec2(0.0f, -playerUpImpulse / 1.8f), body->GetPosition());
			lastGroundJump = false;
		}
	}
	else
	{
		impulse = false;
	}
}
//-----------------------------------------------------------------------------

void GamePlayer::setMode( const PLAYER_MODE& _mode )
{
	if( level && level->furriesAvailibility[_mode] )
	{
		mode = _mode;
		sprite = animations[mode][ANIM_STAND];

		switch( mode )
		{
		case PLAYER_MODE_YELLOW:
			selectionEffect->info.colColorStart = 0xFFFFFFFF;
			selectionEffect->info.colColorEnd = 0xFFFFFF00;
			break;
		case PLAYER_MODE_RED:
			selectionEffect->info.colColorStart = 0xFFFFFFFF;
			selectionEffect->info.colColorEnd = 0xFFFF0000;
			break;
		case PLAYER_MODE_GREEN:
			selectionEffect->info.colColorStart = 0xFFFFFFFF;
			selectionEffect->info.colColorEnd = 0xFF00FF00;
			break;
		case PLAYER_MODE_BLUE:
			selectionEffect->info.colColorStart = 0xFFFFFFFF;
			selectionEffect->info.colColorEnd = 0xFF0000FF;
			break;
		}
	}
}
//-----------------------------------------------------------------------------

void GamePlayer::toggleSelection()
{
	if( onGround )
	{
		if( modeSelection )
		{
			visible = true;
			selectionEffect->Stop();
			endSelectionEffectTimer = 0.5f;
		}
		else
		{	
			visible = false;
			selectionEffect->FireAt(position.x, position.y);
			startSelectionEffectTimer = 0.3f;
			modeSelection = !modeSelection;
		}
	}
}
//-----------------------------------------------------------------------------

void GamePlayer::selection( const bool& left )
{
	if( action )
		return;

	if( endSelectionEffectTimer > 0.0f )
		return;

	if( modeSelection )
	{
		PLAYER_MODE newMode = getNextMode(mode, left);
		if( newMode != mode )
		{
			startSelectionEffectTimer = 0.5f;
			setMode(newMode);
		}
	}
}
//-----------------------------------------------------------------------------

PLAYER_MODE GamePlayer::getNextMode( const PLAYER_MODE& current, const bool& left )
{
	if( level )
	{
		switch( current )
		{
		case PLAYER_MODE_YELLOW:
			if( left )
			{
				if( level->furriesAvailibility[PLAYER_MODE_BLUE] )
					return PLAYER_MODE_BLUE;
				else
					return getNextMode(PLAYER_MODE_BLUE, left);
			}
			else
			{
				if( level->furriesAvailibility[PLAYER_MODE_RED] )
					return PLAYER_MODE_RED;
				else
					return getNextMode(PLAYER_MODE_RED, left);
			}
			break;
		case PLAYER_MODE_RED:
			if( left )
			{
				if( level->furriesAvailibility[PLAYER_MODE_YELLOW] )
					return PLAYER_MODE_YELLOW;
				else
					return getNextMode(PLAYER_MODE_YELLOW, left);
			}
			else
			{
				if( level->furriesAvailibility[PLAYER_MODE_GREEN] )
					return PLAYER_MODE_GREEN;
				else
					return getNextMode(PLAYER_MODE_GREEN, left);
			}
			break;
		case PLAYER_MODE_GREEN:
			if( left )
			{
				if( level->furriesAvailibility[PLAYER_MODE_RED] )
					return PLAYER_MODE_RED;
				else
					return getNextMode(PLAYER_MODE_RED, left);
			}
			else
			{
				if( level->furriesAvailibility[PLAYER_MODE_BLUE] )
					return PLAYER_MODE_BLUE;
				else
					return getNextMode(PLAYER_MODE_BLUE, left);
			}
			break;
		case PLAYER_MODE_BLUE:
			if( left )
			{
				if( level->furriesAvailibility[PLAYER_MODE_GREEN] )
					return PLAYER_MODE_GREEN;
				else
					return getNextMode(PLAYER_MODE_GREEN, left);
			}
			else
			{
				if( level->furriesAvailibility[PLAYER_MODE_YELLOW] )
					return PLAYER_MODE_YELLOW;
				else
					return getNextMode(PLAYER_MODE_YELLOW, left);
			}
			break;
		}
	}

	return current;
}
//-----------------------------------------------------------------------------

void GamePlayer::actionBegin()
{
	if( modeSelection )
		return;

	if( mode == PLAYER_MODE_RED )
	{
		if( move )
		{
			if( direction.x > 0 )
			{
				setAnimation(ANIM_CRUNCH_RIGHT);
				if( crunchContacts[1] )
					crunchContacts[1]->hit();
			}

			if( direction.x < 0 )
			{
				setAnimation(ANIM_CRUNCH_LEFT);
				if( crunchContacts[0] )
					crunchContacts[0]->hit();
			}
		}
		else
		{
			setAnimation(ANIM_CRUNCH_DOWN);
			if( crunchContacts[2] )
			{
				crunchContacts[2]->hit();
			}
		}
	}
	else if( mode == PLAYER_MODE_YELLOW )
	{
		if( direction.x > 0 )
			setAnimation(ANIM_ENERGY_GROW_RIGHT);

		if( direction.x < 0 )
			setAnimation(ANIM_ENERGY_GROW_LEFT);
	}
	else if( mode == PLAYER_MODE_GREEN )
	{
		if( !rope_object.is_created() )
		{
			rope_raycast rr;
			b2Vec2 dir;
			dir.SetZero();

			if( hge->Input_GetKeyState( HGEK_RIGHT ) )
				dir.x = 1.0f;
			else if( hge->Input_GetKeyState( HGEK_LEFT ) )
				dir.x = -1.0f;

			if( hge->Input_GetKeyState( HGEK_UP ) )
				dir.y = -1.0f;

			dir.Normalize();

			if( dir.LengthSquared() < 0.0001f )
				dir.y = -1.0f;

			b2Vec2 p1 = body->GetPosition() + b2Vec2( dir.x * 19.0f / phys_norma, dir.y * 19.0f / phys_norma );
			b2Vec2 p2 = body->GetPosition() + b2Vec2( dir.x * 1000.0f / phys_norma, dir.y * 1000.0f / phys_norma );

			box2d->RayCast( &rr, p1, p2 );

			if( rr.m_body )
			{
				rope_object.create( rr.m_body, rr.m_point, body );
				setAnimation( ANIM_GREEN_ROPE );
			}
		}
	}

	action = true;
}
//-----------------------------------------------------------------------------

void GamePlayer::actionEnd()
{
 	if( modeSelection )
	{
		toggleSelection();
		return;
	}

	action = false;

	if( mode == PLAYER_MODE_YELLOW )
	{
		shotFireball( fireballPower );

		if( direction.x > 0 )
			setAnimation(ANIM_SHOT_RIGHT);

		if( direction.x < 0 )
			setAnimation(ANIM_SHOT_LEFT);

		fireballPower = 0.0f;
	}
	else if( mode == PLAYER_MODE_GREEN )
	{
		if( rope_object.is_created() )
		{
			rope_object.detach();
			setAnimation( ANIM_STAND );
		}
	}
}
//-----------------------------------------------------------------------------

void GamePlayer::shotFireball(const float &power)
{
	GameFireball* fireball = new GameFireball();
	fireball->direction = vector2f( direction.x * 600.0f, 0.0f );
	fireball->fire(position + vector2f(direction.x * 25.0f, 0.0f ), fireballPower / 2.0f + 0.5f);
	level->effect_add(fireball);
}
//-----------------------------------------------------------------------------

void GamePlayer::setAnimation( const ANIMATIONS& anim )
{
	if( animations[mode][anim] )
	{
		if( sprite == animations[mode][anim] )
			return;

		if( sprite )
		{
			sprite->Stop();
			sprite->SetFrame(0);
		}
		sprite = animations[mode][anim];
		sprite->Play();
		currentAnim = anim;
	}
	else
	{
		log_msg(COLOR_WARNING, "no such animation");
	}
}
//-----------------------------------------------------------------------------

void GamePlayer::draw_player_data()
{
	default_font->SetColor(0xFF000000);
	default_font->printf(15, 5, 0, "on ground: %s", onGround ? "true" : "false");
	default_font->printf(15, 15, 0, "in water: %s", inWater ? "true" : "false");
	default_font->printf(15, 25, 0, "impulse: %s", impulse ? "true" : "false");
	default_font->printf(15, 35, 0, "last ground jump: %s", lastGroundJump ? "true" : "false");
	default_font->printf(15, 45, 0, "action: %s", action ? "true" : "false");
	default_font->printf(15, 55, 0, "ground normal: %f, %f", groundNormal.x, groundNormal.y);
	default_font->printf(15, 65, 0, "direction: %f, %f", direction.x, direction.y);
	default_font->printf(15, 75, 0, "position: %f, %f", position.x, position.y);
	default_font->printf(15, 85, 0, "contact left: %s", contactLeft ? "true" : "false");
	default_font->printf(15, 95, 0, "contact right: %s", contactRight ? "true" : "false");
	default_font->printf(15, 105, 0, "contacts count: %d", player_contacts.size());

	player_contacts.clear();

}
//-----------------------------------------------------------------------------