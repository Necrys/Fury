#pragma once
//-----------------------------------------------------------------------------

#include "hgerect.h"
#include "common.h"
#include "data_objectdata.h"
//-----------------------------------------------------------------------------

class water_object
{
public:

	water_object( const hgeRect& bounds );
	~water_object();

#ifndef NOEDIT

	bool m_selected;

	void render();
	bool test_point( const vector2f& point );
	int  select_point( const vector2f& point );
	void set_point_pos( const unsigned& pointID, const vector2f& position );
	void serialize( FILE* stream );
	void correct_bounds();

#else

	ObjectData m_data;

	void add_to_box2d( b2World* box2d );

#endif

private:

	hgeRect m_bounds;
};
//-----------------------------------------------------------------------------

