#pragma once

template <typename T>
__forceinline std::string precision( const T a_value ) {
	std::stringstream out;
	out << std::fixed << std::setprecision( 2 ) << a_value;
	return out.str( );
}