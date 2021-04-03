#pragma once

#include <Box2d/Box2D.h>
#include <list>
//-----------------------------------------------------------------------------

class rope_raycast: public b2RayCastCallback
{
public:

	rope_raycast();
	virtual ~rope_raycast();

	virtual float32 ReportFixture( b2Fixture* fixture, const b2Vec2& point, 
		const b2Vec2& normal, float32 fraction );

	b2Body*	m_body;
	b2Vec2	m_point;
};
//-----------------------------------------------------------------------------

class rope
{
public:

	rope();
	~rope();

	void	create( b2Body* ceiling, const b2Vec2& anchor, b2Body* tiny );
	void	move_up();
	void	move_down();
	void	detach();
	void	clean();
	bool	is_created();
	void	render();
	float	get_angle();

private:

	std::list<b2Body*>	m_links;
	std::list<b2Joint*>	m_joints;
	b2Joint*			m_tiny_joint;
	b2Body*				m_tiny;
	b2Body*				m_ceiling;
	b2Vec2				m_anchor;
	bool				m_created;
};
//-----------------------------------------------------------------------------