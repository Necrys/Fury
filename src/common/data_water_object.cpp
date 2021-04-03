#include "data_water_object.h"

#include "system_interfaces.h"
#include "sys_vars.h"
#include "hgerect.h"
//-----------------------------------------------------------------------------

water_object::water_object( const hgeRect& bounds )
{
	m_bounds = bounds;

#ifndef NOEDIT
	m_selected = false;
#else
	m_data.isWater = true;
#endif
}
//-----------------------------------------------------------------------------

water_object::~water_object()
{

}
//-----------------------------------------------------------------------------

#ifndef NOEDIT

void water_object::render()
{
	ASSERT(hge);

	uint32 border_color = 0xff7f7fff;
	uint32 fill_color = 0x7f0f0fff;

	if( m_selected )
	{
		border_color = 0xffafafff;
		fill_color = 0x7f7f7fff;
	}

	hgeQuad quad;
	memset(&quad, 0, sizeof(hgeQuad));
	quad.v[0].col = fill_color;
	quad.v[1].col = fill_color;
	quad.v[2].col = fill_color;
	quad.v[3].col = fill_color;

	quad.v[0].x = m_bounds.x1;	quad.v[0].y = m_bounds.y1;
	quad.v[1].x = m_bounds.x2;	quad.v[1].y = m_bounds.y1;
	quad.v[2].x = m_bounds.x2;	quad.v[2].y = m_bounds.y2;
	quad.v[3].x = m_bounds.x1;	quad.v[3].y = m_bounds.y2;

	hge->Gfx_RenderQuad(&quad);

	hge->Gfx_RenderLine(m_bounds.x1, m_bounds.y1, m_bounds.x2, m_bounds.y1, border_color);
	hge->Gfx_RenderLine(m_bounds.x2, m_bounds.y1, m_bounds.x2, m_bounds.y2, border_color);
	hge->Gfx_RenderLine(m_bounds.x2, m_bounds.y2, m_bounds.x1, m_bounds.y2, border_color);
	hge->Gfx_RenderLine(m_bounds.x1, m_bounds.y2, m_bounds.x1, m_bounds.y1, border_color);

	if( m_selected )
	{
		hgeQuad corner;
		memset(&corner, 0, sizeof(hgeQuad));
		corner.v[0].col = corner.v[1].col = corner.v[2].col = corner.v[3].col = 0xffafafff;

		corner.v[0].x = m_bounds.x1 - 2;	corner.v[0].y = m_bounds.y1 - 2;
		corner.v[1].x = m_bounds.x1 + 2;	corner.v[1].y = m_bounds.y1 - 2;
		corner.v[2].x = m_bounds.x1 + 2;	corner.v[2].y = m_bounds.y1 + 2;
		corner.v[3].x = m_bounds.x1 - 2;	corner.v[3].y = m_bounds.y1 + 2;

		hge->Gfx_RenderQuad(&corner);

		corner.v[0].x = m_bounds.x2 - 2;	corner.v[0].y = m_bounds.y1 - 2;
		corner.v[1].x = m_bounds.x2 + 2;	corner.v[1].y = m_bounds.y1 - 2;
		corner.v[2].x = m_bounds.x2 + 2;	corner.v[2].y = m_bounds.y1 + 2;
		corner.v[3].x = m_bounds.x2 - 2;	corner.v[3].y = m_bounds.y1 + 2;

		hge->Gfx_RenderQuad(&corner);

		corner.v[0].x = m_bounds.x2 - 2;	corner.v[0].y = m_bounds.y2 - 2;
		corner.v[1].x = m_bounds.x2 + 2;	corner.v[1].y = m_bounds.y2 - 2;
		corner.v[2].x = m_bounds.x2 + 2;	corner.v[2].y = m_bounds.y2 + 2;
		corner.v[3].x = m_bounds.x2 - 2;	corner.v[3].y = m_bounds.y2 + 2;

		hge->Gfx_RenderQuad(&corner);

		corner.v[0].x = m_bounds.x1 - 2;	corner.v[0].y = m_bounds.y2 - 2;
		corner.v[1].x = m_bounds.x1 + 2;	corner.v[1].y = m_bounds.y2 - 2;
		corner.v[2].x = m_bounds.x1 + 2;	corner.v[2].y = m_bounds.y2 + 2;
		corner.v[3].x = m_bounds.x1 - 2;	corner.v[3].y = m_bounds.y2 + 2;

		hge->Gfx_RenderQuad(&corner);
	}
}
//-----------------------------------------------------------------------------

bool water_object::test_point( const vector2f& point )
{
	correct_bounds();
	return m_bounds.TestPoint(point.x, point.y);
}
//-----------------------------------------------------------------------------

int water_object::select_point( const vector2f& point )
{
	correct_bounds();

	hgeRect rect;

	rect = hgeRect( m_bounds.x2 - 4, m_bounds.y2 - 4, m_bounds.x2 + 4, m_bounds.y2 + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 2;

	rect = hgeRect( m_bounds.x2 - 4, m_bounds.y1 - 4, m_bounds.x2 + 4, m_bounds.y1 + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 1;

	rect = hgeRect( m_bounds.x1 - 4, m_bounds.y2 - 4, m_bounds.x1 + 4, m_bounds.y2 + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 3;

	rect = hgeRect( m_bounds.x1 - 4, m_bounds.y1 - 4, m_bounds.x1 + 4, m_bounds.y1 + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 0;

	return -1;
}
//-----------------------------------------------------------------------------

void water_object::set_point_pos( const unsigned& pointID, const vector2f& position )
{
	correct_bounds();

	switch( pointID )
	{
	case 0:
		m_bounds.x1 = position.x;
		m_bounds.y1 = position.y;
		break;
	case 1:
		m_bounds.x2 = position.x;
		m_bounds.y1 = position.y;
		break;
	case 2:
		m_bounds.x2 = position.x;
		m_bounds.y2 = position.y;
		break;
	case 3:
		m_bounds.x1 = position.x;
		m_bounds.y2 = position.y;
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------------------------

void water_object::serialize( FILE* stream )
{

}
//-----------------------------------------------------------------------------

void water_object::correct_bounds()
{
	if( m_bounds.x1 > m_bounds.x2 )
	{
		float t = m_bounds.x1;
		m_bounds.x1 = m_bounds.x2;
		m_bounds.x2 = t;

		if( selectedCollisionCorner == 0 )
			selectedCollisionCorner = 1;
		else if( selectedCollisionCorner == 1 )
			selectedCollisionCorner = 0;
		else if( selectedCollisionCorner == 2 )
			selectedCollisionCorner = 3;
		else if( selectedCollisionCorner == 3 )
			selectedCollisionCorner = 2;
	}

	if( m_bounds.y1 > m_bounds.y2 )
	{
		float t = m_bounds.y1;
		m_bounds.y1 = m_bounds.y2;
		m_bounds.y2 = t;

		if( selectedCollisionCorner == 0 )
			selectedCollisionCorner = 3;
		else if( selectedCollisionCorner == 3 )
			selectedCollisionCorner = 0;
		else if( selectedCollisionCorner == 2 )
			selectedCollisionCorner = 1;
		else if( selectedCollisionCorner == 1 )
			selectedCollisionCorner = 2;
	}
}
//-----------------------------------------------------------------------------

#else

void water_object::add_to_box2d( b2World* box2d )
{
	ASSERT(box2d);

	float half_width, half_height;
	half_width = (m_bounds.x2 - m_bounds.x1) / 2.0f;
	half_height = (m_bounds.y2 - m_bounds.y1) / 2.0f;

	b2BodyDef bodyDef;
	bodyDef.position.Set( (m_bounds.x1 + half_width)  / phys_norma, (m_bounds.y1 + half_height) / phys_norma );
	bodyDef.type = b2_staticBody;
	bodyDef.linearDamping = 0.0f;

	b2Body*	body = box2d->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox( half_width / phys_norma, half_height / phys_norma );

	b2FixtureDef fix;
	fix.density = 0.0f;
	fix.friction = basicFriction;
	fix.restitution = 0.0f;
	fix.filter.categoryBits = OBJCAT_STATIC;
	fix.filter.maskBits = OBJCAT_DYNAMIC | OBJCAT_PLAYER | OBJCAT_FIREBALL;
	fix.shape = &shape;

	b2Fixture* f = body->CreateFixture(&fix);
	f->SetUserData(&m_data);
}
//-----------------------------------------------------------------------------

#endif

//-----------------------------------------------------------------------------