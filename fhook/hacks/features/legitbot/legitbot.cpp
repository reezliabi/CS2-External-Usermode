#include "../../../inc.hpp"
#include "../../../sdk/animation_system/animation_system.hpp"

void unknowncheats::legit_t::run_aimbot( const c_entity& entity, const c_entity& local, vec3_t local_pos, int ent_idx, int local_idx ) {
    float yaw, pitch, distance, norm, lenght;
	vec3_t opp_pos;
	vec2_t angles{};
	int screen_center_x = 1920 / 2;
	int screen_center_y = 1080 / 2;

    /* hacks_ctx.cpp */
    float distance_to_sight = 0;
    float max_aim_distance = 1500;
    vec3_t aim_pos{ 0, 0, 0 };

    bool body_if_lethal = _settings->legitbot_stuff[ 9 ] && local.player_pawn.health < 30;

    /* hitbox shit */
    if ( !body_if_lethal ) {
        switch ( unknowncheats::_settings->hitbox ) {
            case 0:
            {
                _legit->aim_position = bone_index::head;
            } break;
            case 1:
            {
                _legit->aim_position = bone_index::neck_0;
            } break;
            case 2:
            {
                _legit->aim_position = bone_index::spine_1;
            } break;
            case 3:
            {
                _legit->aim_position = bone_index::pelvis;
            } break;
        }
    } else if ( body_if_lethal ) {
        _legit->aim_position = bone_index::pelvis;
    } 

    distance_to_sight = entity.get_bone( ).bone_pos_list[ bone_index::head ].screen_pos.dist_to( { ( 1920 / 2 ), ( 1080 / 2 ) } );
    if ( distance_to_sight < max_aim_distance ) {
        max_aim_distance = distance_to_sight;

        if ( !_settings->visible_check || entity.player_pawn.spotted_by_mask & ( DWORD64( 1 ) << ( local_idx ) ) || local.player_pawn.spotted_by_mask & ( DWORD64( 1 ) << ( ent_idx ) ) ) {
            aim_pos = entity.get_bone( ).bone_pos_list[ _legit->aim_position ].pos;
                    
            if ( _legit->aim_position == bone_index::head )
                aim_pos.z -= 1.f;
        }
    }

    //bool[9] // last used

    /* dinamic scale will work when u use the aimbot */
    bool use_dinamic_while_moving = _settings->legitbot_stuff[ 1 ] && entity.player_pawn.vec_velocity.length( ) > 50.f;
    bool use_dinamic_while_player_lethal = _settings->legitbot_stuff[ 2 ] && entity.player_pawn.health < 30;
    bool use_dinamic_while_local_is_lethal = _settings->legitbot_stuff[ 3 ] && local.player_pawn.health < 30;
    bool use_dinamic_while_good_player = _settings->legitbot_stuff[ 7 ] && entity.controller.total_dmg > _settings->legitbot_stuff_int[0];
    static float dinamic_scale[4]; /* run this static, whatever */

    /* dinamic smooth */
    bool use_dinamic_smoth_1 = _settings->legitbot_stuff[ 5 ] && entity.player_pawn.vec_velocity.length( ) > 50.f;
    bool use_dinamic_smoth_2 = _settings->legitbot_stuff[ 6 ] && local.player_pawn.health < 30;
    bool use_dinamic_while_good_player2 = _settings->legitbot_stuff[ 8 ] && entity.controller.total_dmg > _settings->legitbot_stuff_int[ 2 ];
    static float dinamic_smooth[ 3 ]; /* run this static, whatever */

    if ( _settings->aimbot ) {
        switch ( _settings->a_activationz_type ) {
            case 0: /* hold */
            {
                if ( !GetAsyncKeyState( _input_key->get_bind_id( _settings->a_triggerkey ) ) ) {
                    dinamic_scale[ 0 ] = 0.f; /* reset */
                    dinamic_scale[ 1 ] = 0.f; /* reset */
                    dinamic_scale[ 2 ] = 0.f; /* reset */
                    dinamic_scale[ 3 ] = 0.f; /* reset */
                    this->dinamic_csale = 0.f;

                    dinamic_smooth[ 0 ] = 0.f;
                    dinamic_smooth[ 1 ] = 0.f;
                    dinamic_smooth[ 2 ] = 0.f;
                    this->dinamic_smoth = 0.f;

                    return;
                }
            } break;
            case 1: /* toggle */
            {
                if ( !GetKeyState( _input_key->get_bind_id( _settings->a_triggerkey ) ) ) {
                    dinamic_scale[ 0 ] = 0.f; /* reset */
                    dinamic_scale[ 1 ] = 0.f; /* reset */
                    dinamic_scale[ 2 ] = 0.f; /* reset */
                    dinamic_scale[ 3 ] = 0.f; /* reset */
                    this->dinamic_csale = 0.f;

                    dinamic_smooth[ 0 ] = 0.f;
                    dinamic_smooth[ 1 ] = 0.f;
                    dinamic_smooth[ 2 ] = 0.f;
                    this->dinamic_smoth = 0.f;
                    return;
                }
            } break;
            case 2: /* always on */
            {
                /* shit is always on*/
            } break;
        }
    } else if ( !_settings->aimbot ) {
        return;
    }

    /* paste fix */
    if ( aim_pos == vec3_t( 0, 0, 0 ) ) {
        dinamic_scale[0] = 0.f; /* reset */
        dinamic_scale[1] = 0.f; /* reset */
        dinamic_scale[2] = 0.f; /* reset */
        dinamic_scale[3] = 0.f; /* reset */
        this->dinamic_csale = 0.f;

        dinamic_smooth[ 0 ] = 0.f;
        dinamic_smooth[ 1 ] = 0.f;
        dinamic_smooth[ 2 ] = 0.f;
        this->dinamic_smoth = 0.f;
        return;
    }

    if ( framework::m_b_open ) {
        dinamic_scale[ 0 ] = 0.f; /* reset */
        dinamic_scale[ 1 ] = 0.f; /* reset */
        dinamic_scale[ 2 ] = 0.f; /* reset */
        dinamic_scale[ 3 ] = 0.f; /* reset */
        this->dinamic_csale = 0.f;

        dinamic_smooth[ 0 ] = 0.f;
        dinamic_smooth[ 1 ] = 0.f;
        dinamic_smooth[ 2 ] = 0.f;
        this->dinamic_smoth = 0.f;
        return;
    }

    if ( entity.player_pawn.health <= 0 ) {
        dinamic_scale[ 0 ] = 0.f; /* reset */
        dinamic_scale[ 1 ] = 0.f; /* reset */
        dinamic_scale[ 2 ] = 0.f; /* reset */
        dinamic_scale[ 3 ] = 0.f; /* reset */
        this->dinamic_csale = 0.f;

        dinamic_smooth[ 0 ] = 0.f;
        dinamic_smooth[ 1 ] = 0.f;
        dinamic_smooth[ 2 ] = 0.f;
        this->dinamic_smoth = 0.f;
        return;
    }

    if ( _settings->legitbot_stuff[ 0 ] ) { /* dinamic fov */
        /* some conditions so we dont overrun this shit */
        if ( use_dinamic_while_moving ) {
            /* enemy is moving that is not that much of bullshit */
            dinamic_scale[ 0 ] = 1.f; /* just 1.f added to fov, since he is moving, thats not much shit to do */
        } else {
            dinamic_scale[ 0 ] = 0.0f; /* reset it */
        }

        if ( use_dinamic_while_player_lethal ) {
            /* player is lethal, so we might want to hit it easier, we are going to add 1.3f */
            dinamic_scale[ 1 ] = 1.3f; /* just add to fov */
        } else {
            dinamic_scale[ 1 ] = 0.0f; /* just add to fov */
        }

        if ( use_dinamic_while_local_is_lethal ) {
            /* local is lethal, so we might want to hit it easier, we are going to add 0.5f */
            dinamic_scale[ 2 ] = 0.5f; /* just add to fov */
        } else {
            dinamic_scale[ 2 ] = 0.f;
        }

        if ( use_dinamic_while_good_player ) {
            /* player is lethal, so we might want to hit it easier, we are going to add 1.3f */
            dinamic_scale[ 3 ] = 1.f; /* just add to fov */
        } else {
            dinamic_scale[ 3 ] = 0.0f; /* just add to fov */
        }

        this->dinamic_csale = dinamic_scale[ 0 ] + dinamic_scale[ 1 ] + dinamic_scale[ 2 ];
    }

    /* reset if this is off */
    if ( !_settings->legitbot_stuff[ 0 ] ) {
        dinamic_scale[ 0 ] = 0.f; /* reset */
        dinamic_scale[ 1 ] = 0.f; /* reset */
        dinamic_scale[ 2 ] = 0.f; /* reset */
        this->dinamic_csale = 0.f;
    }

    /* dinamic smooth */
    if ( _settings->legitbot_stuff[ 4 ] ) {
        /* some conditions so we dont overrun this shit */
        if ( use_dinamic_smoth_1 ) {
            /* enemy is moving that is not that much of bullshit */
            dinamic_smooth[ 0 ] = 0.2f; /* just 1.f added to fov, since he is moving, thats not much shit to do */
        } else {
            dinamic_smooth[ 0 ] = 0.0f; /* reset it */
        }

        if ( use_dinamic_smoth_2 ) {
            /* player is lethal, so we might want to hit it easier, we are going to add 1.3f */
            dinamic_smooth[ 1 ] = 0.2f; /* just add to fov */
        } else {
            dinamic_smooth[ 1 ] = 0.0f; /* just add to fov */
        }

        if ( use_dinamic_while_good_player2 ) {
            /* player is lethal, so we might want to hit it easier, we are going to add 1.3f */
            dinamic_smooth[ 2 ] = 0.2f; /* just add to fov */
        } else {
            dinamic_smooth[ 2 ] = 0.0f; /* just add to fov */
        }

        this->dinamic_smoth = dinamic_smooth[ 0 ] + dinamic_smooth[ 1 ];
    } 
    
    if ( !_settings->legitbot_stuff[ 4 ] ) {
        dinamic_smooth[ 0 ] = 0.f;
        dinamic_smooth[ 1 ] = 0.f;
        this->dinamic_smoth = 0.f;
    }

	/* fix */
	opp_pos = aim_pos - local_pos;
	distance = sqrt( pow( opp_pos.x, 2 ) + pow( opp_pos.y, 2 ) );
	lenght = sqrt( distance * distance + opp_pos.z * opp_pos.z );

	yaw = atan2f( opp_pos.y, opp_pos.x ) * 57.295779513 - local.player_pawn.viewangle.y;
	pitch = -atan( opp_pos.z / distance ) * 57.295779513 - local.player_pawn.viewangle.x;
	norm = sqrt( pow( yaw, 2 ) + pow( pitch, 2 ) );

	vec2_t screen_pos;
	_address->view.world_to_screen( vec3_t( aim_pos ), screen_pos );

    if ( norm >= ( _settings->fov + this->dinamic_csale ) ) {
        return; // If condition not met, exit early
    }

    float smooth_factor = ( ( _settings->smooth - this->dinamic_smoth ) != 0.0f ) ? ( _settings->smooth - this->dinamic_smoth ) : 1.5f;

    auto calculate_target = [ ]( int screen_pos, int screen_center, float smooth ) {
        return ( screen_pos != screen_center ) ? ( ( screen_pos > screen_center ) ? -( screen_center - screen_pos ) : ( screen_pos - screen_center ) ) / smooth : 0;
        };

    float target_x = calculate_target( screen_pos.x, screen_center_x, smooth_factor );
    float target_y = ( screen_pos.y != screen_center_y && screen_pos.y != 0 ) ? calculate_target( screen_pos.y, screen_center_y, smooth_factor ) : 0;

    if ( !_settings->smooth ) {
        mouse_event( MOUSEEVENTF_MOVE, target_x, target_y, NULL, NULL );
        return;
    }

    float distance_ratio = norm / ( _settings->fov + this->dinamic_csale );
    float speed_factor = 1.0f + ( 1.0f - distance_ratio );

    target_x = calculate_target( screen_pos.x, screen_center_x, smooth_factor ) / ( ( _settings->smooth - this->dinamic_smoth ) * speed_factor );
    target_y = ( screen_pos.y != screen_center_y && screen_pos.y != 0 ) ? calculate_target( screen_pos.y, screen_center_y, smooth_factor ) / ( ( _settings->smooth - this->dinamic_smoth ) * speed_factor ) : 0;

    mouse_event( MOUSEEVENTF_MOVE, target_x, target_y, NULL, NULL );

}

void unknowncheats::legit_t::draw_aimbot_fov( ) { 
    if ( !_settings->draw_fov ) {
        return;
    }

    float screen_x = GetSystemMetrics( SM_CXSCREEN ) / 2.f;
    float screen_y = GetSystemMetrics( SM_CYSCREEN ) / 2.f;

    /* run animation */
    auto animation = animation_controller.get( "animation_fov" + std::to_string( 0 ) + animation_controller.current_child );
    animation.adjust( animation.value + 3.f * animation_controller.get_min_deltatime( 1.f ) * ( ( this->dinamic_csale > 0.f ) ? 1.f : -1.f ) );

    _render->add_circle( unknowncheats::vec2_t( screen_x, screen_y ), ( ( _settings->fov + this->dinamic_csale * animation.value ) * 10 ), _render->to_main_color( _settings->fov_color ) );
}
