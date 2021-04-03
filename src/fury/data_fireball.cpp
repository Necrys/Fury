/*=============================================================================

	The Fury

DESCRIPTION:
	fireball

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
31/05/2010	initial version
=============================================================================*/

#include "types.h"
#include "data_fireball.h"
#include "data_dynamic_destructable.h"
#include "system_interfaces.h"
#include "system_paths.h"
#include "sys_vars.h"
#include "str_helpers.h"
#include "system_main.h"

//-----------------------------------------------------------------------------

GameFireball::GameFireball()
{	
	HTEXTURE texture = textureManager->getTexture( Path::particles + "particles.png" );
	hgeSprite* fireballSprite = new hgeSprite( texture, 0, 64, 32, 32 );
	fireballSprite->SetBlendMode( BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE );
	fireballSprite->SetHotSpot(16, 16);

	hgeParticleSystem* fireball = new hgeParticleSystem( (Path::particles + "fireball.psi").c_str(), fireballSprite );

	effects.insert( effects.end(), fireball );

	b2BodyDef		bodyDef;
	b2CircleShape	shape;
	b2FixtureDef	fix;
	b2Fixture*		f;

	bodyDef.position.Set( 0.0f, 0.0f );
	bodyDef.bullet = true;
	bodyDef.type = b2_dynamicBody;
	body = box2d->CreateBody(&bodyDef);

	shape.m_radius = 5.0f / phys_norma;

//	shapeDef.isSensor = true;
	fix.density = 0.1f;
	fix.filter.categoryBits |= OBJCAT_FIREBALL;
	fix.filter.maskBits = OBJCAT_STATIC | OBJCAT_DYNAMIC;
	fix.shape = &shape;

	f = body->CreateFixture(&fix);
	f->SetUserData(&data);

	data.isFireball = true;
	data.objectPtr = this;
	data.isPhysical = false;
	data.type = OBJECT_EFFECT;

	splash = true;
}
//-----------------------------------------------------------------------------

GameFireball::~GameFireball()
{
	box2d->DestroyBody(body);
}
//-----------------------------------------------------------------------------

void GameFireball::update(const float &delta)
{
//	position += ( direction * delta );
	body->SetLinearVelocity( b2Vec2(50 * delta * direction.x / phys_norma, 50 * delta * direction.y / phys_norma) );

	// gravity compensation
	body->ApplyForce( b2Vec2(-phys_gravity.x / phys_norma * delta / 21.0f, -phys_gravity.y / phys_norma * delta / 21.0f ), body->GetLocalCenter() );
	b2Vec2 pos = body->GetPosition();
	position = vector2f(pos.x * phys_norma, pos.y * phys_norma);

	GameEffect::update(delta);
}
//-----------------------------------------------------------------------------

void GameFireball::onCollision( b2Fixture* fix, contact_point* point )
{
	ObjectData* obj_data = (ObjectData*)fix->GetUserData();
	if( splash && obj_data && obj_data->isPhysical && obj_data->type != OBJECT_PLAYER )
	{
		this->stop();

		// spawn splash effect
		GameEffect* splashfx = new GameEffect();

		HTEXTURE texture = textureManager->getTexture( Path::particles + "particles.png" );
		hgeSprite* fireballSprite = new hgeSprite( texture, 32, 0, 32, 32 );
		fireballSprite->SetBlendMode( BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE );
		fireballSprite->SetHotSpot(16, 16);

		hgeParticleSystem* particles = new hgeParticleSystem( (Path::particles + "fireball_splash.psi").c_str(), fireballSprite );
		splashfx->effects.insert( splashfx->effects.end(), particles );
		splashfx->timeScale = 3.0f;
		splashfx->fire( position );
		level->effect_add(splashfx);

		splash = false;

		if( obj_data->type == OBJECT_MOVABLE )
		{
			b2Vec2 b2circleCenter = body->GetWorldCenter();
			b2Vec2 b2contactPoint = point->position;
			b2Vec2 b2contactnormal = b2circleCenter - b2contactPoint;
			vector2f contactNormal = vector2f(b2contactnormal.x, b2contactnormal.y);
			vector2f upVector(0.0f, -1.0f);
			float angle = contactNormal.Angle(upVector);
			if( angle > 2 * M_PI / 6.0f && angle < M_PI_2 + M_PI / 6.0f )
			{
				b2Vec2 force = b2Vec2(point->normal.x * -8.0f * scale, 0.0f);
				fix->GetBody()->ApplyLinearImpulse(force, b2contactPoint);
			}
		}

		if( obj_data->type == OBJECT_DESTRUCTABLE )
		{
			GameDestructable* destr = static_cast<GameDestructable*>(obj_data->objectPtr);
			if( destr )
				destr->hit(fireballPower * scale);
		}
	}
}
//-----------------------------------------------------------------------------

void GameFireball::fire( const vector2f& position, const float& _scale )
{
	body->SetTransform( b2Vec2(position.x / phys_norma, position.y / phys_norma), rotation );
	scale = _scale;

	GameEffect::fire(position, _scale);
}
//-----------------------------------------------------------------------------