#include <list>

#include "water_simulation.h"
#include "common.h"
#include "sys_vars.h"

#include "box2d/Box2D.h"
#include "hgerect.h"

using namespace std;
//-----------------------------------------------------------------------------

struct watersim_body
{
	b2Body*			body;
	list<hgeRect*>	water_blobs;
};
//-----------------------------------------------------------------------------

list<watersim_body> watersim_bodies;

float  water_density = 1.0f;
b2Vec2 water_velosity = b2Vec2(0.0f, 0.0f);
float  water_linear_drag = 1.0f;
float  water_angular_drag = 1.0f;
//-----------------------------------------------------------------------------

void watersim_add_body( b2Body* body, hgeRect* water_blob )
{
	ASSERT(body);
	ASSERT(water_blob);

	list<watersim_body>::iterator b = watersim_bodies.begin();
	while( b != watersim_bodies.end() )
	{
		if( (*b).body == body )
		{
			(*b).water_blobs.push_back(water_blob);
			return;
		}

		b++;
	}

	watersim_body wb;
	wb.body = body;
	wb.water_blobs.push_back(water_blob);
}
//-----------------------------------------------------------------------------

void watersim_remove_body( b2Body* body, hgeRect* water_blob )
{
	ASSERT(body);
	ASSERT(water_blob);

	list<watersim_body>::iterator b = watersim_bodies.begin();
	while( b != watersim_bodies.end() )
	{
		if( (*b).body == body )
		{
			(*b).water_blobs.remove(water_blob);
			if( (*b).water_blobs.size() == 0 )
				watersim_bodies.erase(b);
			return;
		}

		b++;
	}
}
//-----------------------------------------------------------------------------

void watersim_step()
{
	list<watersim_body>::iterator body = watersim_bodies.begin();
	while( body != watersim_bodies.end() )
	{
		(*body).body->GetWorldCenter();
		float offset = (*(*body).water_blobs.begin())->y1;
		list<hgeRect*>::iterator blob = (*body).water_blobs.begin();
		while( blob != (*body).water_blobs.end() )
		{
			offset = min( offset, (*blob)->y1 );

			blob++;
		}

		b2Vec2 areac(0,0);
		b2Vec2 massc(0,0);
		float32 area = 0;
		float32 mass = 0;

		for( b2Fixture* shape = body->body->GetFixtureList(); shape; shape=shape->GetNext() )
		{
			b2Vec2 sc(0,0);
			float32 sarea = shape->ComputeSubmergedArea(b2Vec2(0.0f, 1.0f), offset, &sc);
			area += sarea;
			areac.x += sarea * sc.x;
			areac.y += sarea * sc.y;
			float shapeDensity = 0;
			shapeDensity=shape->GetDensity();
			mass += sarea*shapeDensity;
			massc.x += sarea * sc.x * shapeDensity;
			massc.y += sarea * sc.y * shapeDensity;
		}
		areac.x/=area;
		areac.y/=area;
		b2Vec2 localCentroid = b2MulT(body->body->GetTransform(), areac);
		massc.x/=mass;
		massc.y/=mass;
		if( area < 0.0001f )
		{
			body++;
			continue;
		}
		//Buoyancy
		b2Vec2 buoyancyForce = -water_density * area * b2Vec2(phys_gravity.x / phys_norma, phys_gravity.y / phys_norma);
		body->body->ApplyForce(buoyancyForce, massc);
		//Linear drag
		b2Vec2 dragForce = body->body->GetLinearVelocityFromWorldPoint(areac) - water_velosity;
		dragForce *= -water_linear_drag * area;
		body->body->ApplyForce(dragForce,areac);
		//Angular drag
		//TODO: Something that makes more physical sense?
		body->body->ApplyTorque(-body->body->GetInertia() / body->body->GetMass() * area * body->body->GetAngularVelocity() * water_angular_drag);

		body++;
	}
}
//-----------------------------------------------------------------------------

void watersim_clean()
{
	list<watersim_body>::iterator b = watersim_bodies.begin();
	while( b != watersim_bodies.end() )
	{
		(*b).water_blobs.clear();
		b++;
	}

	watersim_bodies.clear();
}
//-----------------------------------------------------------------------------