#include "../../inc.hpp"

DWORD64 search_for_offset ( std::string Signature, DWORD64 ModuleAddress ) {
	std::vector<DWORD64> TempAddressList;
	DWORD64 Address = 0;
	DWORD Offsets = 0;

	TempAddressList = _proc_manager.search_memory( Signature, ModuleAddress, ModuleAddress + 0x4000000 );

	if ( TempAddressList.size( ) <= 0 )
		return 0;

	if ( !_proc_manager.read_memory<DWORD>( TempAddressList.at( 0 ) + 3, Offsets ) )
		return 0;

	Address = TempAddressList.at( 0 ) + Offsets + 7;
	return Address;
}

using dword = DWORD64;

bool unknowncheats::interface_t::initialize( ) {
	dword client_dll = reinterpret_cast< DWORD64 >( _proc_manager.get_process_module_handle( "client.dll" ) );
	dword server_dll = reinterpret_cast< DWORD64 >( _proc_manager.get_process_module_handle( "server.dll" ) );

	if ( client_dll == 0 || server_dll == 0 ) {
		printf( "[unknowncheats] invalid 1\n" );
		return false;
	}

	dword temp = 0;

	/* entity list */
	temp = search_for_offset( unknowncheats::signatures::entity_list, client_dll );
	if ( temp == 0 ) {
		printf( "[unknowncheats] invalid unknowncheats::signatures::entity_list\n" );
		return false;
	}

	/* dump offset */
	unknowncheats::dragged_offsets::entity_list = temp - client_dll;

	/* local player */
	temp = search_for_offset( unknowncheats::signatures::local_player_controller, client_dll );
	if ( temp == 0 ) {
		printf( "[unknowncheats] invalid unknowncheats::signatures::local_player_controller\n" );
		return false;
	}

	/* dump offset */
	unknowncheats::dragged_offsets::local_player_controller = temp - client_dll;

	/* view matrix  */
	temp = search_for_offset( unknowncheats::signatures::view_matrix, client_dll );
	if ( temp == 0 ) {
		printf( "[unknowncheats] invalid unknowncheats::signatures::view_matrix\n" );
		return false;
	}

	/* dump offset */
	unknowncheats::dragged_offsets::matrix = temp - client_dll;

	/* global vars  */
	temp = search_for_offset( unknowncheats::signatures::global_vars, client_dll );
	if ( temp == 0 ) {
		printf( "[unknowncheats] invalid unknowncheats::signatures::global_vars\n" );
		return false;
	}

	/* dump offset */
	unknowncheats::dragged_offsets::global_vars = temp - client_dll;

	/* view angle  */
	temp = search_for_offset( unknowncheats::signatures::view_angles, client_dll );
	if ( temp == 0 ) {
		printf( "[unknowncheats] invalid unknowncheats::signatures::view_angles\n" );
		return false;
	}

	/* dump offset */
	unknowncheats::dragged_offsets::view_angle = temp - client_dll;

	/* localpawn  */
	temp = search_for_offset( unknowncheats::signatures::local_player_pawn, client_dll );
	if ( temp == 0 ) {
		printf( "[unknowncheats] invalid unknowncheats::signatures::local_player_pawn\n" );
		return false;
	}

	/* dump offset */
	unknowncheats::dragged_offsets::local_player_pawn = temp + 0x138 - client_dll;

	/* force jump  */
	temp = search_for_offset( unknowncheats::signatures::force_jump, client_dll );
	if ( temp == 0 ) {
		printf( "[unknowncheats] invalid unknowncheats::signatures::force_jump\n" );
		return false;
	}

	/* dump offset */
	unknowncheats::dragged_offsets::force_jump = temp + 0x30 - client_dll;

	/* planted c4  */
	temp = search_for_offset( unknowncheats::signatures::planted_c4, client_dll );
	if ( temp == 0 ) {
		printf( "[unknowncheats] invalid unknowncheats::signatures::planted_c4\n" );
		return false;
	}

	/* dump offset */
	unknowncheats::dragged_offsets::planted_c4 = temp - client_dll;

	return true;
}

bool unknowncheats::address_t::initialize( ) {
	this->game.client_dll = reinterpret_cast< DWORD64 >( _proc_manager.get_process_module_handle( "client.dll" ) );
	this->game.server_dll = reinterpret_cast< DWORD64 >( _proc_manager.get_process_module_handle( "server.dll" ) );

	this->game.entity_list = get_client_dll_address( ) + unknowncheats::dragged_offsets::entity_list;
	this->game.matrix = get_client_dll_address( ) + unknowncheats::dragged_offsets::matrix;
	this->game.view_angle = get_client_dll_address( ) + unknowncheats::dragged_offsets::view_angle;
	this->game.local_controller = get_client_dll_address( ) + unknowncheats::dragged_offsets::local_player_controller;
	this->game.local_pawn = get_client_dll_address( ) + unknowncheats::dragged_offsets::local_player_pawn;
	this->game.server_pawn = get_client_dll_address( ) + unknowncheats::dragged_offsets::local_player_pawn;
	this->game.force_jump = get_client_dll_address( ) + unknowncheats::dragged_offsets::force_jump;
	this->game.global_vars = get_client_dll_address( ) + unknowncheats::dragged_offsets::global_vars;

	printf( "[unknowncheats] initialized dll addresses!\n" );

	return this->game.client_dll != 0;
}

DWORD64 unknowncheats::address_t::get_client_dll_address( ) {
	return this->game.client_dll;
}

DWORD64 unknowncheats::address_t::get_server_dll_address( ) {
	return this->game.server_dll;
}

DWORD64 unknowncheats::address_t::get_entity_list_address( ) {
	return this->game.entity_list;
}

DWORD64 unknowncheats::address_t::get_matrix_address( ) {
	return this->game.matrix;
}

DWORD64 unknowncheats::address_t::get_view_angle_address( ) {
	return this->game.view_angle;
}

DWORD64 unknowncheats::address_t::get_entity_list_entry( ) {
	return this->game.entity_list_entry;
}

DWORD64 unknowncheats::address_t::get_local_controller_address( ) {
	return this->game.local_controller;
}

DWORD64 unknowncheats::address_t::get_local_pawn_address( ) {
	return this->game.local_pawn;
}

DWORD64 unknowncheats::address_t::get_server_pawn_address( ) {
	return this->game.server_pawn;
}

DWORD64 unknowncheats::address_t::get_global_vars_address( ) {
	return this->game.global_vars;
}

bool unknowncheats::address_t::update_entity_list_entry( ) {
	DWORD64 entity_list_entry = 0;

	if ( !_proc_manager.read_memory<DWORD64>( this->get_entity_list_address( ), entity_list_entry ) ) {
#ifdef read_data_dbg
		print_with_data_scoped( "address_t::update_entity_list_entry -> error -> 1" );
#endif // read_data_dbg
		return false;
	}

	if ( !_proc_manager.read_memory<DWORD64>( entity_list_entry + 0x10, entity_list_entry ) ) {
#ifdef read_data_dbg
		print_with_data_scoped( "address_t::update_entity_list_entry -> error -> 2" );
#endif // read_data_dbg
		return false;
	}

	this->game.entity_list_entry = entity_list_entry;

	return this->game.entity_list_entry != 0;
}

bool unknowncheats::address_t::set_view_angle( float yaw, float pitch ) {
	vec2_t Angle{ pitch, yaw };

	if ( !_proc_manager.write_memory<vec2_t>( this->game.view_angle, Angle ) )
		return false;

	return false;
}

bool unknowncheats::address_t::set_force_jump( int value ) {
	if ( !_proc_manager.write_memory<int>( this->game.force_jump, value ) )
		return false;

	return true;
}

bool unknowncheats::address_t::get_force_jump( int& value ) {
	if ( !_proc_manager.read_memory<int>( this->game.force_jump, value ) )
		return false;

	return true;
}
