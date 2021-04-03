#include <hgerect.h>
#include "box2d/Box2D.h"
#include "system_interfaces.h"
#include "sys_vars.h"
#include "data_collision_circle.h"

//-----------------------------------------------------------------------------

CollisionCircle::CollisionCircle()
{
	center = vector2f(0.0f, 0.0f);
	radius = vector2f(0.0f, 0.0f);
	memset(&triangles, 0, sizeof(hgeTriple) * circleTriangles);
}
//-----------------------------------------------------------------------------

void CollisionCircle::render()
{
	// render fill
#ifndef NOEDIT
	uint32 col = selected ? fillSelectedColor : fillColor;
	uint32 brdcol = selected ? borderSelectedColor : borderColor;
#else
	uint32 col = fillColor;
	uint32 brdcol = borderColor;
#endif

	for( int i=0; i<circleTriangles; i++ )
	{
		// render fill
		triangles[i].tex = 0;
		triangles[i].v[0].col = triangles[i].v[1].col = triangles[i].v[2].col = col;
		hge->Gfx_RenderTriple(&triangles[i]);

		// render border
		hge->Gfx_RenderLine(triangles[i].v[0].x, triangles[i].v[0].y, triangles[i].v[1].x, triangles[i].v[1].y, brdcol);
	}

#ifndef NOEDIT
	// render points
	if( selected )
	{
		hgeQuad corner;
		memset(&corner, 0, sizeof(hgeQuad));
		corner.v[0].col = corner.v[1].col = corner.v[2].col = corner.v[3].col = pointColor;

		corner.v[0].x = center.x - 2;	corner.v[0].y = center.y - 2;
		corner.v[1].x = center.x + 2;	corner.v[1].y = center.y - 2;
		corner.v[2].x = center.x + 2;	corner.v[2].y = center.y + 2;
		corner.v[3].x = center.x - 2;	corner.v[3].y = center.y + 2;

		hge->Gfx_RenderQuad(&corner);

		corner.v[0].x = radius.x - 2;	corner.v[0].y = radius.y - 2;
		corner.v[1].x = radius.x + 2;	corner.v[1].y = radius.y - 2;
		corner.v[2].x = radius.x + 2;	corner.v[2].y = radius.y + 2;
		corner.v[3].x = radius.x - 2;	corner.v[3].y = radius.y + 2;

		hge->Gfx_RenderQuad(&corner);
	}
#endif
}
//-----------------------------------------------------------------------------

void CollisionCircle::rebuildBorders()
{
	float rad = (radius - center).Length();
	float phi = 0.0f;
	for( int i=0; i<circleTriangles; i++ )
	{
		triangles[i].v[0].x = rad * cos(phi) + center.x;
		triangles[i].v[0].y = rad * sin(phi) + center.y;
		phi += (M_PI * 2.0f) / circleTriangles;
		triangles[i].v[1].x = rad * cos(phi) + center.x;
		triangles[i].v[1].y = rad * sin(phi) + center.y;
		triangles[i].v[2].x = center.x;
		triangles[i].v[2].y = center.y;
	}
}
//-----------------------------------------------------------------------------

#ifndef NOEDIT

bool CollisionCircle::testPoint( const vector2f& point )
{
	return (point - center).Length() <= (radius - center).Length() ? true : false;
}
//-----------------------------------------------------------------------------
int CollisionCircle::selectPoint( const vector2f& point )
{
	hgeRect rect;

	rect = hgeRect( center.x - 4, center.y - 4, center.x + 4, center.y + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 0;

	rect = hgeRect( radius.x - 4, radius.y - 4, radius.x + 4, radius.y + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 1;

	return -1;
}
//-----------------------------------------------------------------------------

void CollisionCircle::setPointPos( const unsigned& pointID, const vector2f& position )
{
	if( pointID == 0 )
	{
		vector2f shift = position - center;
		center = position;
		radius += shift;
		for( int i=0; i<circleTriangles; i++ )
		{
			triangles[i].v[0].x += shift.x;
			triangles[i].v[0].y += shift.y;
			triangles[i].v[1].x += shift.x;
			triangles[i].v[1].y += shift.y;
			triangles[i].v[2].x += shift.x;
			triangles[i].v[2].y += shift.y;
		}
	}
	else if( pointID == 1 )
	{
		radius = position;
		rebuildBorders();
		/*
		float rad = (radius - center).Length();
		float phi = 0.0f;
		for( int i=0; i<circleTriangles; i++ )
		{
			triangles[i].v[0].x = rad * cos(phi) + center.x;
			triangles[i].v[0].y = rad * sin(phi) + center.y;
			phi += (M_PI * 2.0f) / circleTriangles;
			triangles[i].v[1].x = rad * cos(phi) + center.x;
			triangles[i].v[1].y = rad * sin(phi) + center.y;
			triangles[i].v[2].x = center.x;
			triangles[i].v[2].y = center.y;
		}
		*/
	}
}
//-----------------------------------------------------------------------------

void CollisionCircle::serialize( FILE* stream )
{
	if( !stream )
		return;

	if( (center - radius).Length() < FLOAT_EPSILON )
		return;

	struct physic_circle_struct
	{
		unsigned int	type;
		unsigned int	obj_type;
		float			center_x;
		float			center_y;
		float			radius_x;
		float			radius_y;
	} circleobj;
	circleobj.type = FLF_SECTION_PHYSIC_GEOMETRY;
	circleobj.obj_type = PHYSIC_OBJ_CIRCLE;
	circleobj.center_x = center.x;
	circleobj.center_y = center.y;
	circleobj.radius_x = radius.x;
	circleobj.radius_y = radius.y;

	fwrite(&circleobj, sizeof(physic_circle_struct), 1, stream);
}
//-----------------------------------------------------------------------------

#else

void CollisionCircle::addToBox2D( b2World* box2d )
{
	assert(box2d);
	
	b2BodyDef	b_def;

	b_def.position.Set( center.x / phys_norma, center.y / phys_norma );
	b_def.type = b2_staticBody;
	b2Body*	body = box2d->CreateBody(&b_def);

	b2CircleShape shape;
	shape.m_radius = (radius - center).Length() / phys_norma;

	b2FixtureDef fix;
	fix.density = 0.0f;
	fix.friction = basicFriction / phys_norma;
	fix.restitution = 0.0f;
	fix.shape = &shape;
	fix.filter.categoryBits = OBJCAT_STATIC;
	fix.filter.maskBits = OBJCAT_DYNAMIC | OBJCAT_PLAYER | OBJCAT_FIREBALL;

	b2Fixture* f = body->CreateFixture(&fix);
	f->SetUserData(&data);

//	b2CircleDef*	shapeDef = new b2CircleDef();
//	shapeDef->density = 0.0f;
//	shapeDef->friction = basicFriction / phys_norma;
//	shapeDef->restitution = 0.0f;
//	shapeDef->radius = ;
//	shapeDef->filter.categoryBits = OBJCAT_STATIC;
//	shapeDef->filter.maskBits = OBJCAT_DYNAMIC | OBJCAT_PLAYER | OBJCAT_FIREBALL;
//	b2Shape* shape = body->CreateShape(shapeDef);
//	shape->SetUserData(&data);
//	body->SetMassFromShapes();

}

//-----------------------------------------------------------------------------
#endif