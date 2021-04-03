#include "data_collision_square.h"
#include "system_interfaces.h"
#include "sys_vars.h"
#include "helper_functions.h"
#include "system_main.h"

//-----------------------------------------------------------------------------

CollisionSquare::CollisionSquare()
{
	memset(&quad, 0, sizeof(hgeQuad));
}
//-----------------------------------------------------------------------------

void CollisionSquare::render()
{
	// render fill
#ifndef NOEDIT
	correctBounds();
	uint32 col = selected ? fillSelectedColor : fillColor;
#else
	uint32 col = fillColor;
#endif

	quad.v[0].col = col;
	quad.v[1].col = col;
	quad.v[2].col = col;
	quad.v[3].col = col;

	quad.v[0].x = bounds.x1;	quad.v[0].y = bounds.y1;
	quad.v[1].x = bounds.x2;	quad.v[1].y = bounds.y1;
	quad.v[2].x = bounds.x2;	quad.v[2].y = bounds.y2;
	quad.v[3].x = bounds.x1;	quad.v[3].y = bounds.y2;

	hge->Gfx_RenderQuad(&quad);
	
	// render borders
#ifndef NOEDIT
	col = selected ? borderSelectedColor : borderColor;
#else
	col = borderColor;
#endif

	hge->Gfx_RenderLine(bounds.x1, bounds.y1, bounds.x2, bounds.y1, col);
	hge->Gfx_RenderLine(bounds.x2, bounds.y1, bounds.x2, bounds.y2, col);
	hge->Gfx_RenderLine(bounds.x2, bounds.y2, bounds.x1, bounds.y2, col);
	hge->Gfx_RenderLine(bounds.x1, bounds.y2, bounds.x1, bounds.y1, col);

	// render corners
#ifndef NOEDIT

	if( selected )
	{
		hgeQuad corner;
		memset(&corner, 0, sizeof(hgeQuad));
		corner.v[0].col = corner.v[1].col = corner.v[2].col = corner.v[3].col = pointColor;

		corner.v[0].x = bounds.x1 - 2;	corner.v[0].y = bounds.y1 - 2;
		corner.v[1].x = bounds.x1 + 2;	corner.v[1].y = bounds.y1 - 2;
		corner.v[2].x = bounds.x1 + 2;	corner.v[2].y = bounds.y1 + 2;
		corner.v[3].x = bounds.x1 - 2;	corner.v[3].y = bounds.y1 + 2;

		hge->Gfx_RenderQuad(&corner);

		corner.v[0].x = bounds.x2 - 2;	corner.v[0].y = bounds.y1 - 2;
		corner.v[1].x = bounds.x2 + 2;	corner.v[1].y = bounds.y1 - 2;
		corner.v[2].x = bounds.x2 + 2;	corner.v[2].y = bounds.y1 + 2;
		corner.v[3].x = bounds.x2 - 2;	corner.v[3].y = bounds.y1 + 2;

		hge->Gfx_RenderQuad(&corner);

		corner.v[0].x = bounds.x2 - 2;	corner.v[0].y = bounds.y2 - 2;
		corner.v[1].x = bounds.x2 + 2;	corner.v[1].y = bounds.y2 - 2;
		corner.v[2].x = bounds.x2 + 2;	corner.v[2].y = bounds.y2 + 2;
		corner.v[3].x = bounds.x2 - 2;	corner.v[3].y = bounds.y2 + 2;

		hge->Gfx_RenderQuad(&corner);

		corner.v[0].x = bounds.x1 - 2;	corner.v[0].y = bounds.y2 - 2;
		corner.v[1].x = bounds.x1 + 2;	corner.v[1].y = bounds.y2 - 2;
		corner.v[2].x = bounds.x1 + 2;	corner.v[2].y = bounds.y2 + 2;
		corner.v[3].x = bounds.x1 - 2;	corner.v[3].y = bounds.y2 + 2;

		hge->Gfx_RenderQuad(&corner);
	}

#endif
}
//-----------------------------------------------------------------------------

#ifndef NOEDIT

bool CollisionSquare::testPoint( const vector2f& point )
{
	correctBounds();
	return bounds.TestPoint(point.x, point.y);
}
//-----------------------------------------------------------------------------

int CollisionSquare::selectPoint( const vector2f& point )
{
	correctBounds();

	hgeRect rect;

	rect = hgeRect( bounds.x2 - 4, bounds.y2 - 4, bounds.x2 + 4, bounds.y2 + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 2;

	rect = hgeRect( bounds.x2 - 4, bounds.y1 - 4, bounds.x2 + 4, bounds.y1 + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 1;

	rect = hgeRect( bounds.x1 - 4, bounds.y2 - 4, bounds.x1 + 4, bounds.y2 + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 3;

	rect = hgeRect( bounds.x1 - 4, bounds.y1 - 4, bounds.x1 + 4, bounds.y1 + 4 );
	if( rect.TestPoint(point.x, point.y) )
		return 0;

	return -1;
}
//-----------------------------------------------------------------------------

void CollisionSquare::setPointPos( const unsigned& pointID, const vector2f& position )
{
	correctBounds();

	switch( pointID )
	{
	case 0:
		bounds.x1 = position.x;
		bounds.y1 = position.y;
		break;
	case 1:
		bounds.x2 = position.x;
		bounds.y1 = position.y;
		break;
	case 2:
		bounds.x2 = position.x;
		bounds.y2 = position.y;
		break;
	case 3:
		bounds.x1 = position.x;
		bounds.y2 = position.y;
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------------------------

void CollisionSquare::correctBounds()
{
	if( bounds.x1 > bounds.x2 )
	{
		float t = bounds.x1;
		bounds.x1 = bounds.x2;
		bounds.x2 = t;
		
		if( selectedCollisionCorner == 0 )
			selectedCollisionCorner = 1;
		else if( selectedCollisionCorner == 1 )
			selectedCollisionCorner = 0;
		else if( selectedCollisionCorner == 2 )
			selectedCollisionCorner = 3;
		else if( selectedCollisionCorner == 3 )
			selectedCollisionCorner = 2;
	}

	if( bounds.y1 > bounds.y2 )
	{
		float t = bounds.y1;
		bounds.y1 = bounds.y2;
		bounds.y2 = t;

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

void CollisionSquare::serialize( FILE* stream )
{
	if( !stream )
		return;

	correctBounds();

	float square = (bounds.x2 - bounds.x1) * (bounds.y2 - bounds.y1);
	if( square < FLOAT_EPSILON )
		return;

	struct physic_square_struct
	{
		unsigned int	type;
		unsigned int	obj_type;
		float			bounds_x1;
		float			bounds_y1;
		float			bounds_x2;
		float			bounds_y2;
	} squareobj;
	squareobj.type = FLF_SECTION_PHYSIC_GEOMETRY;
	squareobj.obj_type = PHYSIC_OBJ_SQUARE;
	squareobj.bounds_x1 = bounds.x1;
	squareobj.bounds_y1 = bounds.y1;
	squareobj.bounds_x2 = bounds.x2;
	squareobj.bounds_y2 = bounds.y2;

	fwrite(&squareobj, sizeof(physic_square_struct), 1, stream);
}
//-----------------------------------------------------------------------------

#else

void CollisionSquare::addToBox2D( b2World* box2d )
{
	if( !box2d )
		return;

	float half_width, half_height;
	half_width = (bounds.x2 - bounds.x1) / 2.0f;
	half_height = (bounds.y2 - bounds.y1) / 2.0f;

	b2BodyDef bodyDef;
	bodyDef.position.Set( (bounds.x1 + half_width)  / phys_norma, (bounds.y1 + half_height) / phys_norma );
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
	f->SetUserData(&data);

	/*
	shapeDef->vertexCount = 4;
	shapeDef->vertices[0] = b2Vec2( -half_width / physicsNorma, -half_height / physicsNorma );
	shapeDef->vertices[3] = b2Vec2( half_width / physicsNorma, -half_height / physicsNorma );
	shapeDef->vertices[2] = b2Vec2( half_width / physicsNorma, half_height / physicsNorma );
	shapeDef->vertices[1] = b2Vec2( -half_width / physicsNorma, half_height / physicsNorma );
	*/
//	b2Shape* shape = body->CreateShape(shapeDef);
//	shape->SetUserData(&data);
//	body->SetMassFromShapes();
}
//-----------------------------------------------------------------------------

#endif