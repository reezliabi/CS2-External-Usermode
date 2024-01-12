#include "../../inc.hpp"

bool unknowncheats::settings_t::save( std::string file_name ) {
	std::string file_path = "C:\\cs2_internal\\" + file_name + ".cfg";

	std::fstream file( file_path, std::ios::out | std::ios::in | std::ios::trunc );
	file.close( );

	file.open( file_path, std::ios::out | std::ios::in );
	if ( !file.is_open( ) ) {
		file.close( );
		return false;
	}

	const size_t settings_size = sizeof( unknowncheats::settings_t );
	for ( int i = 0; i < settings_size; i++ ) {
		byte current_byte = *reinterpret_cast< byte* >( uintptr_t( this ) + i );
		for ( int x = 0; x < 8; x++ ) {
			file << ( int )( ( current_byte >> x ) & 1 );
		}
	}

	file.close( );

	return true;
}

bool unknowncheats::settings_t::load( std::string file_name ) {
	CreateDirectory( L"C:\\cs2_internal", NULL );
	std::string file_path = "C:\\cs2_internal\\" + file_name + ".cfg";

	std::fstream file;
	file.open( file_path, std::ios::out | std::ios::in );
	if ( !file.is_open( ) ) {
		file.close( );
		return false;
	}

	std::string line;
	while ( file ) {
		std::getline( file, line );

		const size_t settings_size = sizeof( unknowncheats::settings_t );
		if ( line.size( ) > settings_size * 8 ) {
			file.close( );
			return false;
		}
		for ( int i = 0; i < settings_size; i++ ) {
			byte current_byte = *reinterpret_cast< byte* >( uintptr_t( this ) + i );
			for ( int x = 0; x < 8; x++ ) {
				if ( line[ ( i * 8 ) + x ] == '1' )
					current_byte |= 1 << x;
				else
					current_byte &= ~( 1 << x );
			}
			*reinterpret_cast< byte* >( uintptr_t( this ) + i ) = current_byte;
		}
	}

	file.close( );

	return true;
}
