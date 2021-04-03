#include "types.h"
#include "hgefont.h"
#include "system_main.h"
#include "system_interfaces.h"
#include "data_level.h"
#include "data_staticobject.h"
#include "helper_functions.h"
#include "sys_vars.h"

//-----------------------------------------------------------------------------

void update_camera();
void set_camera();
void render_fps();
//-----------------------------------------------------------------------------

void box2d_debugdraw();
void render_line( const b2Vec2& p1, const b2Vec2& p2, const uint32& color = 0xFFFFFFFF );
void render_poly( const b2Vec2* vertices, const uint32& vertex_count, const uint32& color = 0xFFFFFFFF );
void render_circle( const b2Vec2& center, const float& radius, const b2Vec2& axis, const uint32& color = 0xFFFFFFFF );
void render_shape( const b2Fixture* shape, const b2Transform& xform, const uint32& color );
//-----------------------------------------------------------------------------

bool onRenderFunc()
{
	hge->Gfx_BeginScene();

	if( level )
	{
		if( level->backgroundImage )
			level->backgroundImage->render();

		if( frame_tick )
			update_camera();
		set_camera();
		level->render();
	}

	if( debug_render )
		box2d_debugdraw();

	hge->Gfx_SetTransform();

	gui_system->render();

	log_render();

	hge->Gfx_RenderQuad(&fullscreenQuad);

	if( show_fps )
		render_fps();

	if( level && level->player )
		level->player->draw_player_data();

	hge->Gfx_EndScene();

	return false;
}
//-----------------------------------------------------------------------------

void update_camera()
{
	if( level && level->player )
	{
		// updating camera position
		vector2f newPos = camera_position;
		if( abs(level->player->position.x - (camera_position.x + 400.0f)) > 40.0f )
			newPos.x = newPos.x + (level->player->position.x - (camera_position.x + 400.0f))/20.0f;
		if( abs(level->player->position.y - (camera_position.y + 300.0f)) > 80.0f )
			newPos.y = newPos.y + (level->player->position.y - (camera_position.y + 300.0f))/30.0f;

		// check level bounds
		if(newPos.x < phys_world_bounds.x1)
			newPos.x = phys_world_bounds.x1;
		if(newPos.y < phys_world_bounds.y1)
			newPos.y = phys_world_bounds.y1;
		if(newPos.x + screen_width > phys_world_bounds.x2)
			newPos.x = phys_world_bounds.x2 - screen_width;
		if(newPos.y + screen_height > phys_world_bounds.y2)
			newPos.y = phys_world_bounds.y2 - screen_height;

		camera_position = newPos;

		camera_rect.x1 = camera_position.x;
		camera_rect.y1 = camera_position.y;
		camera_rect.x2 = camera_rect.x1 + screen_width;
		camera_rect.y2 = camera_rect.y1 + screen_height;
	}
}
//-----------------------------------------------------------------------------

void set_camera()
{
	hge->Gfx_SetTransform(0.0f, 0.0f, (float)(int)(-camera_position.x * globalScale), (float)(int)(-camera_position.y * globalScale), 0.0f, globalScale, globalScale);
}
//-----------------------------------------------------------------------------

void render_fps()
{
	assert(default_font);

	uint32 fps = hge->Timer_GetFPS();
	if( fps < fps_extra_low_threshold )
		default_font->SetColor( COLOR_CRITICAL );
	else if( fps < fps_low_threshold )
		default_font->SetColor( COLOR_WARNING );
	else
		default_font->SetColor( COLOR_SUCCESS );

	default_font->printf( 5.0f, 5.0f, HGETEXT_LEFT, "%d", fps );
}
//-----------------------------------------------------------------------------

void render_shape( const b2Fixture* shape, const b2Transform& xform, const uint32& color )
{
	assert( shape );

	switch (shape->GetShape()->GetType())
	{
	case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)shape->GetShape();
			uint32 vertex_count = poly->GetVertexCount();
			const b2Vec2* local_vertices = poly->m_vertices;

			b2Vec2 vertices[b2_maxPolygonVertices];
			for( uint32 i = 0; i < vertex_count; ++i )
				vertices[i] = b2Mul(xform, local_vertices[i]);
			
			render_poly( vertices, vertex_count, color );
		}
		break;
	case b2Shape::e_circle:
		{
			b2CircleShape* circle = (b2CircleShape*)shape->GetShape();

			b2Vec2 center = b2Mul(xform, circle->m_p);
			float radius = circle->m_radius;
			b2Vec2 axis = xform.R.col1;

			render_circle( center, radius, axis, color );
		}
		break;
/*	case e_edgeShape:
		{
			b2EdgeShape* edge = (b2EdgeShape*)shape;

			b2Vec2 vertex1 = b2Mul(xform, edge->GetVertex1());
			b2Vec2 vertex2 = b2Mul(xform, edge->GetVertex2());

			render_line( vertex1, vertex2, color );
		}
		break;
*/
	}
}
//-----------------------------------------------------------------------------

void draw_joint( b2Joint* joint )
{
	b2Body* b1 = joint->GetBodyA();
	b2Body* b2 = joint->GetBodyB();
	const b2Transform& xf1 = b1->GetTransform();
	const b2Transform& xf2 = b2->GetTransform();
	b2Vec2 x1 = xf1.position;
	b2Vec2 x2 = xf2.position;
	b2Vec2 p1 = joint->GetAnchorA();
	b2Vec2 p2 = joint->GetAnchorB();

	uint32 color = 0xFF77FFFF;

	switch (joint->GetType())
	{
	case e_distanceJoint:
		render_line(p1, p2, color);
		break;

	case e_pulleyJoint:
		{
			b2PulleyJoint* pulley = (b2PulleyJoint*)joint;
			b2Vec2 s1 = pulley->GetGroundAnchorA();
			b2Vec2 s2 = pulley->GetGroundAnchorB();
			render_line(s1, p1, color);
			render_line(s2, p2, color);
			render_line(s1, s2, color);
		}
		break;

	case e_mouseJoint:
		// don't draw this
		break;

	default:
		render_line(x1, p1, color);
		render_line(p1, p2, color);
		render_line(x2, p2, color);
	}
}
//-----------------------------------------------------------------------------

void render_line( const b2Vec2& p1, const b2Vec2& p2, const uint32& color )
{
	assert( hge );
	hge->Gfx_RenderLine(p1.x * phys_norma, 
						p1.y * phys_norma, 
						p2.x * phys_norma, 
						p2.y * phys_norma, 
						color );
}
//-----------------------------------------------------------------------------

void render_poly( const b2Vec2* vertices, const uint32& vertex_count, const uint32& color )
{
	assert( vertices );
	assert( vertex_count > 2 );

	render_line( vertices[0], vertices[vertex_count-1] );
	for( uint32 i=1; i<vertex_count; i++ )
		render_line( vertices[i], vertices[i-1], color );
}
//-----------------------------------------------------------------------------

void render_circle( const b2Vec2& center, const float& radius, const b2Vec2& axis, const uint32& color )
{
	const uint32 k_segments = 32;
	const float32 k_increment = 2.0f * b2_pi / (float)k_segments;
	float32 theta = 0.0f;

	b2Vec2 vertices[k_segments];

	for( uint32 i = 0; i < k_segments; ++i )
	{
		vertices[i] = center + radius * b2Vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}

	b2Vec2 p = center + radius * axis;

	render_line( p, center, color );

	render_line( vertices[0], vertices[k_segments-1], color );

	for( int32 i=1; i<k_segments; i++ )
		render_line( vertices[i-1], vertices[i], color );
}
//-----------------------------------------------------------------------------

void box2d_debugdraw()
{
	// draw shapes
	b2Body*	body = box2d->GetBodyList();
	while( body )
	{
		const b2Transform& xform = body->GetTransform();
		b2Fixture* shape = body->GetFixtureList();
		while( shape )
		{
			uint32 color = 0xFFFFFFFF;

			if( body->GetType() == b2_staticBody )
				color = 0xFF77FF77;
			else if( body->IsAwake() )
				color = 0xFF7777FF;

			render_shape( shape, xform, color );

			shape = shape->GetNext();
		}

		body = body->GetNext();
	}

	// draw joints
	b2Joint* joint = box2d->GetJointList();
	while( joint )
	{
		draw_joint( joint );

		joint = joint->GetNext();
	}
}
//-----------------------------------------------------------------------------