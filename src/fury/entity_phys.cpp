#include "types.h"
#include "entity_phys.h"
#include "system_interfaces.h"
//-----------------------------------------------------------------------------

entity_phys phys_entities[max_phys_entities];
uint32		phys_entities_count = 0;
//-----------------------------------------------------------------------------

entity_phys* phys_create( b2BodyDef* body_def, b2FixtureDef* shape_def, void* data )
{
	assert(body_def);
	assert(shape_def);

	phys_entities[phys_entities_count].body = box2d->CreateBody(body_def);
	phys_entities[phys_entities_count].shape = phys_entities[phys_entities_count].body->CreateFixture(shape_def);

	if( data )
		phys_entities[phys_entities_count].shape->SetUserData(data);

	return &phys_entities[phys_entities_count++];
}
//-----------------------------------------------------------------------------