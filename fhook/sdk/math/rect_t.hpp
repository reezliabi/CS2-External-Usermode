#pragma once

class rect_t {
public:
	float x, y, w, h;

	rect_t( ) : x( 0 ), y( 0 ), w( 0 ), h( 0 ) { }
	rect_t( float x, float y, float w, float h ) : x( x ), y( y ), w( w ), h( h ) { }
	~rect_t( ) { }
};