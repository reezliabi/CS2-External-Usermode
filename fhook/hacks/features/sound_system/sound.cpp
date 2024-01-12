#include "../../../inc.hpp"
#include "../../../sdk/animation_system/animation_system.hpp"

/* rendering stuff gonna sit there */
void render_filled_3d_circle( const unknowncheats::vec3_t& origin, float radius, unknowncheats::col_t color ) {
	static constexpr float pi = 3.14159265358979323846f;

	static constexpr float Step = pi * 2.0f / 60;
	std::vector<ImVec2> points;
	for ( float lat = 0.f; lat <= pi * 2.0f; lat += Step ) {
		const auto& point3d = unknowncheats::vec3_t( sin( lat ), cos( lat ), 0.f ) * radius;
		unknowncheats::vec2_t point2d;
		if ( unknowncheats::_address->view.world_to_screen( origin + point3d, point2d ) )
			points.push_back( ImVec2( point2d.x, point2d.y ) );
	}
	ImGui::GetBackgroundDrawList( )->AddConvexPolyFilled( points.data( ), points.size( ), color.convert( ) );
}

/* rendering stuff gonna sit there */
void render_3d_circle( const unknowncheats::vec3_t& origin, float radius, unknowncheats::col_t color ) {
	static constexpr float pi = 3.14159265358979323846f;
	static constexpr float Step = pi * 2.0f / 60;

	std::vector<ImVec2> points;
	for ( float lat = 0.f; lat <= pi * 2.0f; lat += Step ) {
		const auto& point3d = unknowncheats::vec3_t( sin( lat ), cos( lat ), 0.f ) * radius;
		unknowncheats::vec2_t point2d;
		if ( unknowncheats::_address->view.world_to_screen( origin + point3d, point2d ) )
			points.push_back( ImVec2( point2d.x, point2d.y ) );
	}

	if ( !points.empty( ) ) {
		points.push_back( points.front( ) );
		ImGui::GetBackgroundDrawList( )->AddPolyline( points.data( ), points.size( ), color.convert( ), false, 1.0f );
	}
}

void unknowncheats::sound_info::push_sound( const c_entity& entity ) {
	if ( !_settings->local_sound )
		return; /* do not run */

	// game::m_flEmitSoundTime
	float smth3; // will we need to iterate?
	_proc_manager.read_memory<float>( entity.player_pawn.address + offsets::pawn::emit_sound_time, smth3 );

	static float main = smth3;
	
	if ( main != smth3 ) {
#if 0
		printf( "mem[0]\n" );
#endif
		/* should pushback */
		this->s_info_l.push_back( sound_info_t{ entity.player_pawn.vec_origin, true } );

		/* reupdate */
		main = smth3;
	}

	/* draw */
	for ( int i = 0; i < this->s_info_l.size( ); i++ ) {
		auto animation = animation_controller.get( "soundinfo" + std::to_string( i ) + animation_controller.current_child );
		animation.adjust( animation.value + 3.f * animation_controller.get_min_deltatime( _settings->sound_animation_speed_l ) * ( this->s_info_l[i].has_changed ? 1.f : -1.f ) );

		if ( animation.value >= 0.99f )
			this->s_info_l[ i ].has_changed = false;

		render_3d_circle( this->s_info_l[ i ].player_origin, _settings->local_range * animation.value, _render->to_main_color( _settings->sound_col_l ) );

		if ( i >= 60 ) {
#if 0
			printf( "mem[1]" );
#endif
			this->s_info_l.erase( this->s_info_l.begin( ), this->s_info_l.begin( ) + 51 );

#if 0
			print_with_data_scoped( "size: " + std::to_string( this->s_info_l.size( ) ) )
#endif
		}
	}
}

void unknowncheats::sound_info::push_sound2( const c_entity& entity, int idx ) { /* use that for enemy */
	if ( !_settings->enemy_sound )
		return;

	// game::m_flEmitSoundTime
	float smth3; // will we need to iterate?
	_proc_manager.read_memory<float>( entity.player_pawn.address + offsets::pawn::emit_sound_time, smth3 );

	static float main[ 64 ]{smth3};

	if ( main[idx] != smth3) {
#if 0
		printf( "mem[0]\n" );
#endif
		/* should pushback */
		this->s_info.push_back( sound_info_t{ entity.player_pawn.vec_origin, true } );

		/* reupdate */
		main[idx] = smth3;
	}

	/* draw */
	for ( int i = 0; i < this->s_info.size( ); i++ ) {
		auto animation = animation_controller.get( "soundinfo" + std::to_string( i ) + animation_controller.current_child );
		animation.adjust( animation.value + 3.f * animation_controller.get_min_deltatime( _settings->sound_animation_speed_e ) * ( this->s_info[ i ].has_changed ? 1.f : -1.f ) );

		if ( animation.value >= 0.99f )
			this->s_info[ i ].has_changed = false;

		render_3d_circle( this->s_info[ i ].player_origin, _settings->enemy_range * animation.value, _render->to_main_color( _settings->sound_col_e ) );

		if ( i >= 60 ) {
#if 0
			printf( "mem[1]" );
#endif
			this->s_info.erase( this->s_info.begin( ), this->s_info.begin( ) + 51 );

#if 0
			print_with_data_scoped( "size: " + std::to_string( this->s_info.size( ) ) )
#endif
		}
	}
}
