#include "../../../inc.hpp"
#include "shots_hitsound.hpp"
#pragma comment(lib, "Winmm.lib")
#include "../../../sdk/animation_system/animation_system.hpp"

void unknowncheats::shots_t::hitsound( const c_entity& entity ) {


	static int prev_total_hits;
	int total_hits;
	uintptr_t bullet_services;

	_proc_manager.read_memory( entity.player_pawn.address + 0x16B8, bullet_services );
	_proc_manager.read_memory( bullet_services + 0x40, total_hits );

	if ( total_hits != prev_total_hits ) {
		if ( total_hits == 0 && prev_total_hits != 0 ) { } else {
			/*xref
				    vector < const char* > hitsound = { 
						"Neverlose",
                        "Skeet",
                        "Primordial",
                        "Sound 1",
                        "Sound 2",
						"Toy duck",
						"Bell 1",
						"Bell 2",
						"Button",
						"Pop",
						"Wow"
								};
			*/
			if ( _settings->hitsound ) {
				switch ( _settings->hitsound_type ) {
					case 0:
					{
						PlaySoundA( reinterpret_cast< char* > ( neverlose_sound ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
					case 1:
					{
						PlaySoundA( reinterpret_cast< char* > ( skeet_sound ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
					case 2:
					{
						PlaySoundA( reinterpret_cast< char* > ( primordial_sound ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
					case 3:
					{
						PlaySoundA( reinterpret_cast< char* > ( cock_sound ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
					case 4:
					{
						PlaySoundA( reinterpret_cast< char* > ( bepis_sound ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
					case 5:
					{
						PlaySoundA( reinterpret_cast< char* > ( ratamac ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
					case 6:
					{
						PlaySoundA( reinterpret_cast< char* > ( bell1 ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
					case 7:
					{
						PlaySoundA( reinterpret_cast< char* > ( bell2 ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
					case 8:
					{
						PlaySoundA( reinterpret_cast< char* > ( button_snd ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
					case 9:
					{
						PlaySoundA( reinterpret_cast< char* > ( popsnd ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
					case 10:
					{
						PlaySoundA( reinterpret_cast< char* > ( wowsnd ), NULL, SND_ASYNC | SND_MEMORY );
					} break;
				}
			}
				
			if ( _settings->hitmarker ) {
				hitmarker_info info;
				info.impacted = true;
				info.headshot = false;
				this->hitmarkers.push_back( info );
			}
		}
	}
	prev_total_hits = total_hits;
}

void unknowncheats::shots_t::hitmarker( const c_entity& entity, const c_entity& local ) {
	if ( !_settings->hitmarker ) {
		if ( !this->hitmarkers.empty( ) ) { /* clear these mothercukers */
			this->hitmarkers.clear( );
		}

		/* do not run multiple times */
		return;
	}

	if ( local.player_pawn.health <= 0 ) { /* clear these mothercukers */
		if ( !this->hitmarkers.empty( ) ) {
			this->hitmarkers.clear( );
		}

		/* do not run multiple times */
		return;
	}

	auto center = ImGui::GetIO( ).DisplaySize * 0.5;

	for ( int i = 0; i < this->hitmarkers.size( ); i++ ) {
		auto animation = animation_controller.get( "hitmurker" + std::to_string( i ) + animation_controller.current_child );
		animation.adjust( animation.value + 3.f * animation_controller.get_min_deltatime( _settings->sound_animation_speed_l ) * ( this->hitmarkers[i].impacted ? 1.f : -1.f ) );

		// unknowncheats::_render->add_line( );
		unknowncheats::col_t col = this->hitmarkers[ i ].headshot ? unknowncheats::col_t( 255, 0, 0 ) : _render->to_main_color( _settings->hitmarker_col );

		unknowncheats::_render->add_line( unknowncheats::vec2_t( center.x - 8, center.y - 8 ), unknowncheats::vec2_t( center.x - 4, center.y - 4 ), col.modify_alpha( 255 * animation.value ), 1 );
		unknowncheats::_render->add_line( unknowncheats::vec2_t( center.x - 8, center.y + 8 ), unknowncheats::vec2_t( center.x - 4, center.y + 4 ), col.modify_alpha( 255 * animation.value ), 1 );
		unknowncheats::_render->add_line( unknowncheats::vec2_t( center.x + 8, center.y - 8 ), unknowncheats::vec2_t( center.x + 4, center.y - 4 ), col.modify_alpha( 255 * animation.value ), 1 );
		unknowncheats::_render->add_line( unknowncheats::vec2_t( center.x + 8, center.y + 8 ), unknowncheats::vec2_t( center.x + 4, center.y + 4 ), col.modify_alpha( 255 * animation.value ), 1 );

		if ( animation.value >= 0.99 ) {
			this->hitmarkers[ i ].impacted = false;
		}

		if ( animation.value <= 0.0f && !this->hitmarkers[ i ].impacted ) {
			this->hitmarkers.erase( this->hitmarkers.begin( ) + i );
		}
	}
#if 0
	print_with_data_scoped( std::to_string( this->hitmarkers.size( ) ) )
#endif
}
