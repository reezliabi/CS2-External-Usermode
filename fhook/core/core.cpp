#define WIN32_LEAN_AND_MEAN 

#include "../inc.hpp"

int main( ) {
	SetConsoleTitle( L"unknowncheats.com" );

	auto process_status = _proc_manager.attach( "cs2.exe" );

	switch ( process_status ) {
		case 1:
			goto END;
		case 2:
			goto END;
		case 3:
			goto END;
		default:
			break;
	}

	if ( !unknowncheats::framework::m_b_initialized )
		unknowncheats::framework::create( );

	if ( !unknowncheats::_interfaces->initialize( ) ) {
		printf( "[unknowncheats] failtd to init offsets\n" );
		goto END;
	}

	printf( "[unknowncheats] initialized interfaces!\n" );

	if ( !unknowncheats::_address->initialize( ) ) {
		printf( "[unknowncheats] failtd to init addresses\n" );
		goto END;
	}

	while ( !unknowncheats::framework::unloading ) {
		if ( !unknowncheats::framework::render( ) )
			return 0;

		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
	}

END:
	std::cout << std::endl;
	system( "pause" );
	return 0;
}