#pragma once
//-----------------------------------------------------------------------------

class hgeRect;
class b2Body;
//-----------------------------------------------------------------------------

void watersim_add_body( b2Body* body, hgeRect* water_blob );
void watersim_remove_body( b2Body* body, hgeRect* water_blob );
void watersim_step();
void watersim_clean();
//-----------------------------------------------------------------------------