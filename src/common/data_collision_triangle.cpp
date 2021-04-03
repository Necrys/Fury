#include <hgerect.h>
#include <math.h>
#include "data_collision_triangle.h"
#include "sys_vars.h"
#include "system_interfaces.h"
#include "helper_functions.h"

//-----------------------------------------------------------------------------

CollisionTriangle::CollisionTriangle()
{
	memset(&vertices, 0, sizeof(vector2f) * 3);
	memset(&triple, 0, sizeof(hgeTriple));
}
//-----------------------------------------------------------------------------

void CollisionTriangle::render()
{
	// render fill
#ifndef NOEDIT
	uint32 col = selected ? fillSelectedColor : fillColor;
#else
	uint32 col = fillColor;
#endif

	triple.tex = 0;
	triple.v[0].col = triple.v[1].col = triple.v[2].col = col;
	triple.v[0].x = vertices[0].x;	triple.v[0].y = vertices[0].y;
	triple.v[1].x = vertices[1].x;	triple.v[1].y = vertices[1].y;
	triple.v[2].x = vertices[2].x;	triple.v[2].y = vertices[2].y;

	hge->Gfx_RenderTriple(&triple);

	// render border
#ifndef NOEDIT
	col = selected ? borderSelectedColor : borderColor;
#else
	col = borderColor;
#endif

	hge->Gfx_RenderLine(vertices[0].x, vertices[0].y, vertices[1].x, vertices[1].y, col);
	hge->Gfx_RenderLine(vertices[1].x, vertices[1].y, vertices[2].x, vertices[2].y, col);
	hge->Gfx_RenderLine(vertices[0].x, vertices[0].y, vertices[2].x, vertices[2].y, col);

	// render corners

#ifndef NOEDIT

	if( selected )
	{
		hgeQuad corner;
		memset(&corner, 0, sizeof(hgeQuad));
		corner.v[0].col = corner.v[1].col = corner.v[2].col = corner.v[3].col = pointColor;

		corner.v[0].x = vertices[0].x - 2;	corner.v[0].y = vertices[0].y - 2;
		corner.v[1].x = vertices[0].x + 2;	corner.v[1].y = vertices[0].y - 2;
		corner.v[2].x = vertices[0].x + 2;	corner.v[2].y = vertices[0].y + 2;
		corner.v[3].x = vertices[0].x - 2;	corner.v[3].y = vertices[0].y + 2;

		hge->Gfx_RenderQuad(&corner);

		corner.v[0].x = vertices[1].x - 2;	corner.v[0].y = vertices[1].y - 2;
		corner.v[1].x = vertices[1].x + 2;	corner.v[1].y = vertices[1].y - 2;
		corner.v[2].x = vertices[1].x + 2;	corner.v[2].y = vertices[1].y + 2;
		corner.v[3].x = vertices[1].x - 2;	corner.v[3].y = vertices[1].y + 2;

		hge->Gfx_RenderQuad(&corner);

		corner.v[0].x = vertices[2].x - 2;	corner.v[0].y = vertices[2].y - 2;
		corner.v[1].x = vertices[2].x + 2;	corner.v[1].y = vertices[2].y - 2;
		corner.v[2].x = vertices[2].x + 2;	corner.v[2].y = vertices[2].y + 2;
		corner.v[3].x = vertices[2].x - 2;	corner.v[3].y = vertices[2].y + 2;

		hge->Gfx_RenderQuad(&corner);
	}

#endif
}
//-----------------------------------------------------------------------------

#ifndef NOEDIT

bool CollisionTriangle::testPoint( const vector2f& point )
{
	vector2f v = vertices[1] - vertices[0];
	vector2f u = vertices[2] - vertices[0];
	vector2f w = point - vertices[0];

	float uv = b2Dot(u ,v);
	float wv = b2Dot(w, v);
	float vv = b2Dot(v, v);
	float wu = b2Dot(w, u);
	float uu = b2Dot(u, u);

	float det = ((uv * uv) - uu * vv);

	float s = (uv * wv - vv * wu) / det;
	float t = (uv * wu - uu * wv) / det;

	if( s >= 0 && t >= 0 && s + t <= 1 )
		return true;

	return false;
}
//-----------------------------------------------------------------------------

int CollisionTriangle::selectPoint( const vector2f& point )
{
	hgeRect rect;

	rect = hgeRect( vertices[0].x - 4, vertices[0].y - 4, vertices[0].x + 4, vertices[0].y + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 0;

	rect = hgeRect( vertices[1].x - 4, vertices[1].y - 4, vertices[1].x + 4, vertices[1].y + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 1;

	rect = hgeRect( vertices[2].x - 4, vertices[2].y - 4, vertices[2].x + 4, vertices[2].y + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 2;

	return -1;
}
//-----------------------------------------------------------------------------

void CollisionTriangle::setPointPos( const unsigned& pointID, const vector2f& position )
{
	if( pointID < 3 )
		vertices[pointID] = position;
}
//-----------------------------------------------------------------------------

void CollisionTriangle::serialize( FILE* stream )
{
	if( !stream )
		return;

	vector2f v2, v3;
	v2 = vertices[1] - vertices[0];
	v3 = vertices[2] - vertices[0];

	vector2f v0;
	v0 = vertices[2] - vertices[1];

	float v1len = v2.Length();
	float v2len = v3.Length();
	float v3len = v0.Length();

	float per = ( v1len + v2len + v3len ) / 2.0f;

	vector2f v4, v5, v6, v7;
	v4 = vertices[0] - vertices[1];
	v5 = vertices[2] - vertices[1];

	v6 = vertices[0] - vertices[2];
	v7 = vertices[1] - vertices[2];

	float square = sqrtf( per * ( per - v1len ) * ( per - v2len ) * ( per - v3len ) );
	float angle[3];
	angle[0] = v3.Angle(v2);
	angle[1] = v4.Angle(v5);
	angle[2] = v6.Angle(v7);

	if( square < FLOAT_EPSILON )
		return;

	for(int i=0; i<3; i++)
		if( angle[i] < 0.04 )
			return;

	struct physic_triangle_struct
	{
		unsigned int	type;
		unsigned int	obj_type;
		float			point1_x;
		float			point1_y;
		float			point2_x;
		float			point2_y;
		float			point3_x;
		float			point3_y;
	} triangleobj;
	triangleobj.type = FLF_SECTION_PHYSIC_GEOMETRY;
	triangleobj.obj_type = PHYSIC_OBJ_TRIANGLE;
	triangleobj.point1_x = vertices[0].x;
	triangleobj.point1_y = vertices[0].y;
	triangleobj.point2_x = vertices[1].x;
	triangleobj.point2_y = vertices[1].y;
	triangleobj.point3_x = vertices[2].x;
	triangleobj.point3_y = vertices[2].y;

	fwrite(&triangleobj, sizeof(physic_triangle_struct), 1, stream);
}
//-----------------------------------------------------------------------------

#else

void CollisionTriangle::addToBox2D( b2World* box2d )
{
	if( !box2d )
		return;


	vector2f p = vertices[0];
	if( p.y > vertices[1].y )
	{
		p = vertices[1];
		vertices[1] = vertices[0];
		vertices[0] = p;
	}
	if( p.y > vertices[2].y )
	{
		p = vertices[2];
		vertices[2] = vertices[0];
		vertices[0] = p;
	}

	vector2f v2, v3;
	v2 = vertices[1] - vertices[0];
	v3 = vertices[2] - vertices[0];

	vector2f x_axis = vector2f(1.0f, 0.0f);
	float a[3];

	a[1] = v2.Angle(x_axis);
	a[2] = v3.Angle(x_axis);

	if( a[1] > a[2] )
	{
		vector2f vt = v2;
		v2 = v3;
		v3 = vt;
	}
	/*
	for( int j=0; j<2; j++ )
	{
		int minIndex = j;
		for( int i=j; i<3; i++ )
		{
			if( a[i] < a[minIndex] )
				minIndex = i;
		}
		if( minIndex != j )
		{
			float t = a[minIndex];
			a[minIndex] = a[j];
			a[j] = t;
			hgeVector vt = vertices[minIndex];
			vertices[minIndex] = vertices[j];
			vertices[j] = vt;
		}
	}
	*/

	b2BodyDef bodyDef;
	bodyDef.position.Set( vertices[0].x / phys_norma, vertices[0].y / phys_norma );
	bodyDef.type = b2_staticBody;

	b2Body*	body = box2d->CreateBody(&bodyDef);

	b2PolygonShape shape;
	
	b2Vec2 vertices[3];
	vertices[0] = b2Vec2( 0.0f, 0.0f );
	vertices[1] = b2Vec2( v2.x / phys_norma, v2.y / phys_norma );
	vertices[2] = b2Vec2( v3.x / phys_norma, v3.y / phys_norma );
	shape.Set(vertices, 3);

	b2FixtureDef fix;
	fix.density = 0.0f;
	fix.friction = basicFriction;
	fix.restitution = 0.0f;
	fix.filter.categoryBits = OBJCAT_STATIC;
	fix.filter.maskBits = OBJCAT_DYNAMIC | OBJCAT_PLAYER | OBJCAT_FIREBALL;
	fix.shape = &shape;

	b2Fixture* f = body->CreateFixture(&fix);
	f->SetUserData(&data);
}
//-----------------------------------------------------------------------------

#endif