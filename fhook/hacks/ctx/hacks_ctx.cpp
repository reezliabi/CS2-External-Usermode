#include "hacks_ctx.hpp"
#include "../../inc.hpp"
#include "../../sdk/animation_system/animation_system.hpp"

unknowncheats::c_entity* get_entity( int index ) { // just leave it like that
	unknowncheats::c_entity* ent = ( unknowncheats::c_entity* )( unknowncheats::_address->get_entity_list_entry() + 0x78 * ( index + 1 ) );
	return ent;
}

void unknowncheats::hacks_t::run( ) { 
	/* update matrix */
	if ( !_proc_manager.read_memory( unknowncheats::_address->get_matrix_address( ), unknowncheats::_address->view.matrix, 64 ) ) {
#ifdef read_data_dbg
		print_with_data_scoped( "hacks_t::run -> error -> read_memory::get_matrix_address" );
#endif // read_data_dbg
		return;
	}

	/* update entity list entry */
	unknowncheats::_address->update_entity_list_entry( );

	/* local player stuff */
	DWORD64 local_player_adr = 0;
	DWORD64 local_pawn_adr = 0;
	if ( !_proc_manager.read_memory( unknowncheats::_address->get_local_controller_address( ), local_player_adr ) ) {
#ifdef read_data_dbg
		print_with_data_scoped( "hacks_t::run -> error -> read_memory::get_local_controller_address" );
#endif // read_data_dbg
		return;
	}

	if ( !_proc_manager.read_memory( unknowncheats::_address->get_local_pawn_address( ), local_pawn_adr ) ) {
#ifdef read_data_dbg
		print_with_data_scoped( "hacks_t::run -> error -> read_memory::get_local_pawn_address" );
#endif // read_data_dbg
		return;
	}

	unknowncheats::c_entity local_player;
	static int local_player_index = 1;

	if ( !local_player.update_controller( local_player_adr ) ) {
#ifdef read_data_dbg
		print_with_data_scoped( "hacks_t::run -> error -> update_controller::local_player_adr" );
#endif // read_data_dbg
		return;
	}

	if ( !local_player.update_pawn( local_pawn_adr ) ) {
#ifdef read_data_dbg
		print_with_data_scoped( "hacks_t::run -> error -> update_pawn::local_pawn_adr" );
#endif // read_data_dbg
		return;
	}


	/* loop between entity */
	for ( int i = 0; i < 64; i++ ) {
		c_entity entity;
		DWORD64 entity_address = 0;

		if ( !_proc_manager.read_memory<DWORD64>( unknowncheats::_address->get_entity_list_entry( ) + ( i + 1 ) * 0x78, entity_address ) ) {
#ifdef read_data_dbg1
			print_with_data_scoped( "hacks_t::run -> error -> loop::get_entity_list_entry" );
#endif // read_data_dbg
			continue;
		}

		if ( entity_address == local_player.controller.address ) {
			local_player_index = i;
			continue;
		}

		if ( !entity.update_controller( entity_address ) ) {
#ifdef read_data_dbg1
			print_with_data_scoped( "hacks_t::run -> error -> update_controller::entity_address" );
#endif // read_data_dbg
			continue;
		}

		if ( !entity.update_pawn( entity.player_pawn.address ) ) {
#ifdef read_data_dbg
			print_with_data_scoped( "hacks_t::run -> error -> update_pawn::entity.player_pawn.address" );
#endif // read_data_dbg
			continue;
		}

		if ( entity.controller.team_id == local_player.controller.team_id ) {
			continue;
		}

		if ( !entity.in_screen( ) ) {
			continue;
		}

		unknowncheats::col_t col = unknowncheats::col_t( 255, 0, 0 );
		_proc_manager.write_memory( entity.player_pawn.address + 0xA73, col );

		_shots->hitmarker( entity, local_player );

		bool is = true;
		bool smth;
		float smth3;
		int smth4;
		DWORD64 smth2;


#if 0
#define int_3
#ifdef int_
		_proc_manager.read_memory<int>( local_player.player_pawn.address + 0x1537, smth4 );
#elif defined(int_2)
		_proc_manager.read_memory<float>( local_player.player_pawn.address + 0x15BC, smth3 );
#elif defined(int_3)
		_proc_manager.read_memory<bool>( entity.player_pawn.address + 0x1668, smth );
#endif

#ifdef int_
		if ( GetKeyState( VK_F2 ) )
			print_with_data_scoped( "s: " + std::to_string( smth4 ) )
#elif defined(int_2)
		if ( GetKeyState( VK_F2 ) )
			print_with_data_scoped( "s: " + std::to_string( smth3 ) )
#elif defined(int_3)
		if ( GetKeyState( VK_F2 ) )
			print_with_data_scoped( "s: " + std::to_string( smth ) )
#endif
#endif

		_sound->push_sound2( entity, i );

		_legit->run_aimbot( entity, local_player, local_player.player_pawn.camera_pos, i, local_player_index );
		_rage->run_aimbot( entity, local_player, local_player.player_pawn.camera_pos, i, local_player_index );

		ImVec4 rect = unknowncheats::_esp->get_player_bounding_box( entity );
		unknowncheats::_esp->render_esp( local_player, entity, rect, local_player_index, i );
		unknowncheats::_esp->killed_by_hs( entity, i );
	}

	/* loop between other entity */
	for ( int i_smoke = 64; i_smoke < 1024; i_smoke++ ) {
		uintptr_t ent = ( uintptr_t )get_entity( i_smoke );

		if ( ent == 0 ) {
#ifdef read_data_dbg
			print_with_data_scoped( "hacks_t::run2 -> error -> loop::continue - 1024 [ 0 ]" );
#endif // read_data_dbg
			continue;
		}

		unknowncheats::_esp->change_smoke_color( ent );
		unknowncheats::_esp->remove_smoke( ent );
	}

	_legit->draw_aimbot_fov( );
	_sound->push_sound( local_player );
	_flash->run_flash_builder( local_player );
	_shots->hitsound( local_player );

	/*
		scoped: _proc_manager.read_memory<bool>( local_player.player_pawn.address + 0x13A8, smth );
		defusing: _proc_manager.read_memory<bool>( local_player.player_pawn.address + 0x13B0, smth );
		grabbing_hostage: _proc_manager.read_memory<bool>( local_player.player_pawn.address + 0x13B1, smth );
		gungame_imunity: _proc_manager.read_memory<bool>( local_player.player_pawn.address + 0x13C4, smth );

		shots fired, calc 1 shot, when u do shot : local_player.player_pawn.address + 0x1420
		is_flashed: _proc_manager.read_memory<float>( local_player.player_pawn.address + 0x145C, smth3 ); // smth3 > 0
		
	*/

	

	
	static int prev_total_hits;
	bool total_hits;
	float minexp;
	bool on = true;
	float min = 0.15;

	uintptr_t bullet_services;

	//_proc_manager.read_memory( local_player.controller.address + 0x1F4, bullet_services );
	//_proc_manager.read_memory<bool>( bullet_services + 0xD04, total_hits );
	//_proc_manager.read_memory<float>( bullet_services + 0xD0C, minexp );


	//_proc_manager.write_memory( bullet_services + 0xCEC, min );



	bool is = true;
	bool smth;
	float smth3;
	int smth4;

#if 0
#define int_2
#ifdef int_
	_proc_manager.read_memory<int>( local_player.player_pawn.address + 0x1537, smth4 );
#elif defined(int_2)
	_proc_manager.read_memory<float>( local_player.player_pawn.address + 0x15BC, smth3 );
#elif defined(int_3)
	_proc_manager.read_memory<bool>( local_player.player_pawn.address + 0x1537, smth );
#endif

#ifdef int_
	if ( GetKeyState( VK_F2 ) )
		print_with_data_scoped( "s: " + std::to_string( smth4 ) )
#elif defined(int_2)
	if ( GetKeyState( VK_F2 ) )
		print_with_data_scoped( "s: " + std::to_string( smth3 ) )
#elif defined(int_3)
	if ( GetKeyState( VK_F2 ) )
		print_with_data_scoped( "s: " + std::to_string( smth ) )
#endif
#endif
		float fl = 0.5f;
	//_proc_manager.write_memory<float>( local_player.player_pawn.address + 0x15C0, fl );
	//_proc_manager.write_memory<float>( local_player.player_pawn.address + 0x1464, fl );

#if 0
	if ( smth ) {
		printf( "yes\n" );
	} else {
		printf( "no\n" );
	}
#endif


	/* trigger bot */
	if ( _settings->triggerbot ) {
		switch ( _settings->activationz_type ) {
			case 0: /* hold */
			{
				if ( GetAsyncKeyState( _input_key->get_bind_id( _settings->triggerkey ) ) ) {
					_triggerbot->run_trigger( local_player );
				}
			} break;
			case 1: /* toggle */
			{
				if ( GetKeyState( _input_key->get_bind_id( _settings->triggerkey ) ) ) {
					_triggerbot->run_trigger( local_player );
				}
			} break;
			case 2: /* always on */
			{
				_triggerbot->run_trigger( local_player );
			} break;
		}
	}
}
