/*=============================================================================

	The Fury

DESCRIPTION:
	basic effect

	Kulinich Alexander © 2010
=============================================================================*/

/*=============================================================================
	EDIT HISTORY

when		what
-----------	-------------------------------------------------------------------
31/05/2010	initial version
=============================================================================*/

#include "types.h"
#include "data_effect.h"

//-----------------------------------------------------------------------------

GameEffect::GameEffect()
{
	timeScale = 1.0f;
}
//-----------------------------------------------------------------------------

GameEffect::~GameEffect()
{
	for( uint32 i=0; i<effects.size(); i++ )
	{
		hgeParticleSystem* psystem = effects[i];
		if( psystem )
			delete psystem;
	}
}
//-----------------------------------------------------------------------------

void GameEffect::update( const float& delta )
{
	stopped = true;
	for( uint32 i=0; i<effects.size(); i++ )
	{
		hgeParticleSystem* psystem = effects[i];
		if( psystem )
		{
			psystem->MoveTo(position.x, position.y);
			psystem->Update(delta * timeScale);
			if( psystem->GetParticlesAlive() != 0 )
				stopped = false;
		}
	}
}
//-----------------------------------------------------------------------------

void GameEffect::render()
{
	for( uint32 i=0; i<effects.size(); i++ )
	{
		hgeParticleSystem* psystem = effects[i];
		if( psystem )
			psystem->Render();
	}
}
//-----------------------------------------------------------------------------

void GameEffect::fire(const vector2f &_position, const float& scale)
{
	position = _position;
	for( uint32 i=0; i<effects.size(); i++ )
	{
		hgeParticleSystem* psystem = effects[i];
		if( psystem )
		{
			psystem->FireAt( position.x, position.y );
			psystem->info.fSizeEnd *= scale;
			psystem->info.fSizeStart *= scale;
		}
	}
}
//-----------------------------------------------------------------------------

void GameEffect::stop()
{
	for( uint32 i=0; i<effects.size(); i++ )
	{
		hgeParticleSystem* psystem = effects[i];
		if( psystem )
			psystem->Stop();
	}
}
//-----------------------------------------------------------------------------

bool GameEffect::isStopped() const
{
	return stopped;
}
//-----------------------------------------------------------------------------