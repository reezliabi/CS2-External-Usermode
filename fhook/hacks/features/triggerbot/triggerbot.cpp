#include "../../../inc.hpp"

/* handlers and stuff */
DWORD ent_handler = 0;
DWORD64 list_avaible_entries = 0;
DWORD64 entry_pawn_address = 0;
unknowncheats::c_entity entity;
bool allow_shot = false;
/* handlers and stuff */

void release_mouse_event( ) {
	/* we can now shot so lets do it */
	std::this_thread::sleep_for( std::chrono::milliseconds( unknowncheats::_settings->shot_delay ) ); /* add a custom delay and more */
	mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 ); /* release shot event */
}

void unknowncheats::trigger_t::run_trigger( const unknowncheats::c_entity& local_entity ) { 
	/* read memory of ent index */
	if ( !_proc_manager.read_memory<DWORD>( local_entity.player_pawn.address + offsets::pawn::ent_index, ent_handler ) ) {
#ifdef read_data_dbg
		print_with_data_scoped( "trigger_t::run_trigger -> error -> no memory [ 0 ]" );
#endif // read_data_dbg
		return;
	}

	if ( ent_handler == -1 ) {
#ifdef read_data_dbg_no
		print_with_data_scoped( "trigger_t::run_trigger -> error -> idx -1 ( invalid ) [ 1 ]" );
#endif // read_data_dbg
		return;
	}

	/* store this shit here */
	list_avaible_entries = _proc_manager.trace_address( _address->get_entity_list_address( ), { 0x8 * ( ent_handler >> 9 ) + 0x10,0x0 } );
	if ( list_avaible_entries == 0 ) {
#ifdef read_data_dbg
		print_with_data_scoped( "trigger_t::run_trigger -> error -> idx 0 ( invalid ) [ 2 ]" );
#endif // read_data_dbg
		return;
	}

	if ( !_proc_manager.read_memory<DWORD64>( list_avaible_entries + 0x78 * ( ent_handler & 0x1FF ), entry_pawn_address ) ) {
#ifdef read_data_dbg
		print_with_data_scoped( "trigger_t::run_trigger -> error -> no memory [ 3 ]" );
#endif // read_data_dbg
		return;
	}

	/* update pawn */
	if ( !entity.update_pawn( entry_pawn_address ) ) {
#ifdef read_data_dbg_no
		print_with_data_scoped( "trigger_t::run_trigger -> error -> pawn::handler_t [ 4 ]" );
#endif // read_data_dbg
		return;
	}

	/* add team check here */
	allow_shot = entity.player_pawn.health > 0;

	if ( !allow_shot ) {
		/* we re not gonna log this */
		return;
	}


	static std::chrono::time_point last_point = std::chrono::steady_clock::now( );
	auto cur_point = std::chrono::steady_clock::now( );
	if ( cur_point - last_point >= std::chrono::milliseconds( _settings->reaction_time ) ) {
		const bool shooting = GetAsyncKeyState( VK_LBUTTON ) < 0;

		/* run if !shoting */
		if ( !shooting ) {
			mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
			std::thread trigger_thread( release_mouse_event );
			trigger_thread.detach( );
		}

		/* egalize these */
		last_point = cur_point;
	}
}

void unknowncheats::trigger_t::run_thread_check( const c_entity& local_entity ) noexcept { 
	/* here we run thread check and stuff */
}
