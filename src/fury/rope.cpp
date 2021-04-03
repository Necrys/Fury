#include "rope.h"
#include "system_main.h"
#include "system_interfaces.h"
#include "sys_vars.h"
#include <assert.h>
//-----------------------------------------------------------------------------

using namespace std;
//-----------------------------------------------------------------------------

const uint32	links_count = 30;
const float		step = 0.08f;
//-----------------------------------------------------------------------------

rope::rope()
{
	m_created = false;
}
//-----------------------------------------------------------------------------

void rope::create( b2Body* ceiling, const b2Vec2& anchor, b2Body* tiny )
{
	assert(tiny);
	assert(ceiling);
	assert(box2d);

	m_tiny = tiny;
	m_ceiling = ceiling;
	m_anchor = anchor;

	b2Vec2 vec = anchor - tiny->GetPosition();
	float distance = vec.Length() - ( 10.0f / phys_norma );
	uint32 count = (uint32)(distance / step);

	hge->System_Log("count: %d\ndistance: %f", count, distance);

	if( count < 1 || count > links_count )
		return;

	vec.Normalize();
	b2Vec2 v_step = b2Vec2( vec.x * step, vec.y * step );

	for( uint32 i = 0; i<count; i++ )
	{
		b2BodyDef b_def;
		b_def.position = anchor - b2Vec2( (i+1) * v_step.x, (i+1) * v_step.y );
		b_def.type = b2_dynamicBody;
		b2Body* b = box2d->CreateBody(&b_def);

		b2CircleShape b_shape;
		b_shape.m_radius = 0.025f;

		b2FixtureDef b_fix;
		b_fix.density = 15.0f;
		b_fix.shape = &b_shape;

		b->CreateFixture( &b_fix );

		m_links.push_back(b);
	}

	b2Body* last_link = 0;
	b2Joint* joint;
	list<b2Body*>::iterator it = m_links.begin();
	while( it != m_links.end() )
	{
		b2DistanceJointDef def;
		if( last_link )
			def.Initialize( *it, last_link, (*it)->GetPosition(), last_link->GetPosition() );
		else
			def.Initialize( *it, ceiling, (*it)->GetPosition(), anchor );

		joint = box2d->CreateJoint( &def );

		m_joints.push_back( joint );

		b2BodyDef b_def;
		b_def.position = anchor;
		b_def.type = b2_dynamicBody;

		b2RevoluteJointDef r_def;
		r_def.enableLimit = true;
		r_def.lowerAngle = -2.0f;
		r_def.upperAngle = 2.0f;
		if( last_link )
			r_def.Initialize( *it, last_link, (*it)->GetPosition());
		else
			r_def.Initialize( *it, ceiling, (*it)->GetPosition());
		joint = box2d->CreateJoint( &r_def );
		m_joints.push_back( joint );

		last_link = *it;
		it++;
	}

	b2DistanceJointDef def;
	def.Initialize( tiny, last_link, tiny->GetPosition(), last_link->GetPosition() );
	m_tiny_joint = box2d->CreateJoint( &def );

	m_created = true;
}
//-----------------------------------------------------------------------------

rope::~rope()
{
	clean();
}
//-----------------------------------------------------------------------------

void rope::move_up()
{
	if( !m_tiny_joint )
		return;

	if( m_links.size() <= 1 )
		return;

	if( m_tiny_joint )
		box2d->DestroyJoint( m_tiny_joint );

	b2Vec2 anchor = (*m_links.rbegin())->GetPosition();

	b2Vec2 vec = anchor - m_tiny->GetPosition();
	float distance = vec.Length();
	b2Vec2 advance = b2Vec2( vec.x * 0.1f, vec.y * 0.1f );

	if( distance > 0.30f )
	{
		m_tiny->SetTransform( m_tiny->GetPosition() + advance, 0.0f );

		b2Body* last_link = *m_links.rbegin();

		b2DistanceJointDef def;
		def.Initialize( m_tiny, last_link, m_tiny->GetPosition(), last_link->GetPosition() );
		m_tiny_joint = box2d->CreateJoint( &def );
	}
	else
	{
		box2d->DestroyJoint( *m_joints.rbegin() );
		m_joints.pop_back();
		box2d->DestroyJoint( *m_joints.rbegin() );
		m_joints.pop_back();
		box2d->DestroyBody( *m_links.rbegin() );
		m_links.pop_back();

		b2Body* last_link = *m_links.rbegin();

		b2DistanceJointDef def;
		def.Initialize( m_tiny, last_link, anchor, last_link->GetPosition() );
		m_tiny_joint = box2d->CreateJoint( &def );
	}
}
//-----------------------------------------------------------------------------

void rope::move_down()
{
	if( !m_tiny_joint )
		return;

	if( m_links.size() >= links_count )
		return;

	if( m_tiny_joint )
		box2d->DestroyJoint( m_tiny_joint );

	b2Vec2 anchor = (*m_links.rbegin())->GetPosition();

	b2Vec2 vec = anchor - m_tiny->GetPosition();
	float distance = vec.Length();
	b2Vec2 advance = b2Vec2( vec.x * 0.1f, vec.y * 0.1f );

	if( distance < 0.30f )
	{
		m_tiny->SetTransform( m_tiny->GetPosition() - advance, 0.0f );

		b2Body* last_link = *m_links.rbegin();

		b2DistanceJointDef def;
		def.Initialize( m_tiny, last_link, m_tiny->GetPosition(), last_link->GetPosition() );
		m_tiny_joint = box2d->CreateJoint( &def );
	}
	else
	{
		b2BodyDef b_def;
		b2Vec2 addition = b2Vec2( advance.x * 7.5f, advance.y * 7.5f );
		b_def.position = m_tiny->GetPosition() + addition;
		b_def.type = b2_dynamicBody;
		b2Body* b = box2d->CreateBody(&b_def);

		b2CircleShape b_shape;
		b_shape.m_radius = 0.025f;

		b2FixtureDef b_fix;
		b_fix.density = 15.0f;
		b_fix.shape = &b_shape;

		b->CreateFixture( &b_fix );

		b2Body* last_link = *m_links.rbegin();

		m_links.push_back(b);

		b2Joint* joint;
		b2DistanceJointDef def;
		def.Initialize( b, last_link, b->GetPosition(), last_link->GetPosition() );

		joint = box2d->CreateJoint( &def );

		m_joints.push_back( joint );

		b2RevoluteJointDef r_def;
		r_def.enableLimit = true;
		r_def.lowerAngle = -2.0f;
		r_def.upperAngle = 2.0f;
		r_def.Initialize( b, last_link, b->GetPosition());
		joint = box2d->CreateJoint( &r_def );
		m_joints.push_back( joint );

		b2DistanceJointDef d_def;
		d_def.Initialize( m_tiny, b, anchor, b->GetPosition() );
		m_tiny_joint = box2d->CreateJoint( &d_def );
	}
}
//-----------------------------------------------------------------------------

void rope::detach()
{
	clean();
}
//-----------------------------------------------------------------------------

void rope::clean()
{
	assert(box2d);

	if( m_tiny_joint )
		box2d->DestroyJoint( m_tiny_joint );
	m_tiny_joint = 0;

	list<b2Joint*>::iterator joint = m_joints.begin();
	while( joint != m_joints.end() )
	{
		box2d->DestroyJoint( *joint );
		joint++;
	}
	m_joints.clear();

	list<b2Body*>::iterator link = m_links.begin();
	while( link != m_links.end() )
	{
		box2d->DestroyBody( *link );
		link++;
	}
	m_links.clear();

	m_created = false;
}
//-----------------------------------------------------------------------------

bool rope::is_created()
{
	return m_created;
}
//-----------------------------------------------------------------------------

void rope::render()
{
	if( !m_created )
		return;

	b2Vec2 last_point = m_tiny->GetPosition();
	b2Vec2 point;

	list<b2Body*>::reverse_iterator link = m_links.rbegin();
	while( link != m_links.rend() )
	{
		point = (*link)->GetPosition();
		hge->Gfx_RenderLine( last_point.x * phys_norma, last_point.y * phys_norma, point.x * phys_norma, point.y * phys_norma, 0xff7f7fff );
		last_point = point;

		link++;
	}
}
//-----------------------------------------------------------------------------

float rope::get_angle()
{
	if( !m_created )
		return 0.0f;

	b2Vec2 tiny_point = m_tiny->GetPosition();
	b2Vec2 rope_point = (*m_links.rbegin())->GetPosition();

	b2Vec2 vec = rope_point - tiny_point;
	float angle = vec.Angle( b2Vec2(0.0f, -1.0f) );

	return tiny_point.x < rope_point.x ? angle : -angle;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

rope_raycast::rope_raycast()
{
	m_body = 0;
}
//-----------------------------------------------------------------------------

rope_raycast::~rope_raycast()
{
	// nothing
}
//-----------------------------------------------------------------------------

float32 rope_raycast::ReportFixture( b2Fixture* fixture, const b2Vec2& point, 
									const b2Vec2& normal, float32 fraction )
{
	assert( fixture );

	b2Body* b = fixture->GetBody();
	if( b->GetType() == b2_staticBody || b->GetType() == b2_dynamicBody )
	{
		m_body = b;
		m_point = point;

		return fraction;
	}

	return 1;
}
//-----------------------------------------------------------------------------