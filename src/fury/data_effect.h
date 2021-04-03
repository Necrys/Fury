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

#pragma once

#include <hgeparticle.h>
#include <vector>
#include "data_staticobject.h"

//-----------------------------------------------------------------------------

class GameEffect: public GameStaticObject
{
public:
	GameEffect();
	virtual ~GameEffect();
	
	std::vector<hgeParticleSystem*>	effects;

	virtual void update( const float& delta );

	virtual void render();

	virtual void fire( const vector2f& position, const float& scale = 1.0f );

	virtual void stop();

	virtual bool isStopped() const;

	float timeScale;

protected:

	bool stopped;
};
//-----------------------------------------------------------------------------
