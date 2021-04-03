#include "types.h"
#include "sys_vars.h"
#include "system_interfaces.h"
#include "system_collider.h"
#include "data_objectdata.h"
#include "data_staticobject.h"
#include <list>

using namespace std;

//-----------------------------------------------------------------------------

contact_point	points[phys_max_contacts];
uint32			points_count;
//-----------------------------------------------------------------------------

class ContactListener : public b2ContactListener
{
public:
	list<contact_point>	m_contacts;

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
/*		contact_point p;
		p.shape1 = contact->GetFixtureA();
		p.shape2 = contact->GetFixtureB();
		
		b2WorldManifold w;
		contact->GetWorldManifold(&w);
		p.position = w.points[0];
		p.normal = w.normal;

		m_contacts.push_back(p);*/
	}

	void BeginContact(b2Contact* point)
	{
		contact_point c;
		c.contact = point;
		c.state = CS_ADDED;
		m_contacts.push_back(c);
	}
	//-------------------------------------------------------------------------

	void Persist(const b2Contact* point)
	{
		
	}
	//-------------------------------------------------------------------------

	void EndContact(b2Contact* point)
	{
		list<contact_point>::iterator it = m_contacts.begin();
		while( it != m_contacts.end() )
		{
			if( it->contact == point )
			{
				it = m_contacts.erase(it);
				break;
			}

			it++;
		}
	}
	//-------------------------------------------------------------------------

	void update()
	{
		list<contact_point>::iterator it = m_contacts.begin();
		while( it != m_contacts.end() )
		{
			it->shape1 = it->contact->GetFixtureA();
			it->shape2 = it->contact->GetFixtureB();

			ObjectData *o1, *o2;
			o1 = (ObjectData*)it->contact->GetFixtureA()->GetUserData();
			o2 = (ObjectData*)it->contact->GetFixtureB()->GetUserData();

			b2WorldManifold w;
			it->contact->GetWorldManifold(&w);
			it->position = w.points[0];
			it->normal = w.normal;

			if( o1 && o1->type != OBJECT_STATIC && o1->type != OBJECT_UNKNOWN )
			{
				GameStaticObject* obj = static_cast<GameStaticObject*>(o1->objectPtr);
				if( obj )
					obj->onCollision( it->shape2, &(*it) );
			}

			if( o2 && o2->type != OBJECT_STATIC && o2->type != OBJECT_UNKNOWN )
			{
				GameStaticObject* obj = static_cast<GameStaticObject*>(o2->objectPtr);
				if( obj )
					obj->onCollision( it->shape1, &(*it) );
			}

			it->state = CS_PERSISTED;

			it++;
		}

//		m_contacts.clear();
	}
	//-------------------------------------------------------------------------
} listener;
//-----------------------------------------------------------------------------

void collision_detector_init()
{
	assert(box2d);
	box2d->SetContactListener(&listener);
	points_count = 0;
}
//-----------------------------------------------------------------------------

void collisions_update()
{
/*	for( uint32 i=0; i<points_count; i++ )
	{
		ObjectData* object1, *object2;
//		object1 = (ObjectData*)points[i].shape1->GetUserData();
//		object2 = (ObjectData*)points[i].shape2->GetUserData();

		if( object1 && object1->type != OBJECT_STATIC && object1->type != OBJECT_UNKNOWN )
		{
			GameStaticObject* obj = static_cast<GameStaticObject*>(object1->objectPtr);
			if( obj )
				obj->onCollision( points[i].shape2, &points[i] );
		}

		if( object2 && object2->type != OBJECT_STATIC && object2->type != OBJECT_UNKNOWN )
		{
			GameStaticObject* obj = static_cast<GameStaticObject*>(object2->objectPtr);
			if( obj )
				obj->onCollision( points[i].shape1, &points[i] );
		}
	}

	points_count = 0;
*/
	listener.update();
}
//-----------------------------------------------------------------------------

void physics_update()
{
	assert(box2d);

	points_count = 0;

	box2d->Step(1.0f/phys_hz, phys_iterations, phys_iterations);
//	box2d->Validate();

	collisions_update();
}
//-----------------------------------------------------------------------------
