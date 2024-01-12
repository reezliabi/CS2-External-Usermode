#include "../../../inc.hpp"

void release_mouse_event_rage( ) {
    /* we can now shot so lets do it */
    std::this_thread::sleep_for( std::chrono::milliseconds( unknowncheats::_settings->shot_delay ) ); /* add a custom delay and more */
    mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 ); /* release shot event */
}

void unknowncheats::rage_t::run_aimbot( const c_entity& entity, const c_entity& local, vec3_t local_pos, int ent_idx, int local_idx ) {
    // gheto ass pasted - aimstar with modifications
    if ( !_settings->rage )
        return;
    
    float yaw, pitch, distance, norm, lenght, target_x, target_y;
    vec3_t opp_pos;
    vec2_t angles{};
    int screen_center_x = 1920 / 2;
    int screen_center_y = 1080 / 2;

    /* hacks_ctx.cpp */
    float distance_to_sight = 0;
    float max_aim_distance = 1500;
    vec3_t aim_pos{ 0, 0, 0 };

    float distance_aim = local.player_pawn.vec_origin.dist_to( entity.player_pawn.vec_origin );
    if ( _settings->ignore_if_Distance_tO_high ) {
        if ( distance_aim > _settings->distance_to_rage ) {
            return; /* do not try for higher distance */
        }
    }

    // multipoint [0]
    bool b1 = unknowncheats::_settings->ragebot_stuff[ 1 ] && entity.player_pawn.health < 30;
    bool entity_lol = entity.player_pawn.health > local.player_pawn.health;
    bool b2 = unknowncheats::_settings->ragebot_stuff[ 2 ] && entity_lol;

    if ( _settings->ragebot_stuff[ 0 ] ) { /* thats the multipoint bruther */

        if ( !b1 || !b2 ) {
            switch ( _settings->ragebot_stuff2[ 0 ] ) {
                case 0:
                {
                    this->aim_position = bone_index::head;
                } break;
                case 1:
                {
                    this->aim_position = bone_index::pelvis;
                } break;
            }
        }

        if ( b1 || b2 ) {
            this->aim_position = bone_index::pelvis;
        }
       
    }

    /* hitbox shit */
    switch ( unknowncheats::_settings->rage_hitbox ) {
        case 0:
        {
            this->aim_position = bone_index::head;
        } break;
        case 1:
        {
            this->aim_position = bone_index::neck_0;
        } break;
        case 2:
        {
            this->aim_position = bone_index::spine_1;
        } break;
        case 3:
        {
            this->aim_position = bone_index::pelvis;
        } break;
    }

    distance_to_sight = entity.get_bone( ).bone_pos_list[ bone_index::head ].screen_pos.dist_to( { ( 1920 / 2 ), ( 1080 / 2 ) } );
    if ( distance_to_sight < max_aim_distance ) {
        max_aim_distance = distance_to_sight;

        if ( !_settings->ignore_wall || entity.player_pawn.spotted_by_mask & ( DWORD64( 1 ) << ( local_idx ) ) || local.player_pawn.spotted_by_mask & ( DWORD64( 1 ) << ( ent_idx ) ) ) {
            aim_pos = entity.get_bone( ).bone_pos_list[ this->aim_position ].pos;

            if ( this->aim_position == bone_index::head )
                aim_pos.z -= 1.f;
        }
    }

    /* paste fix */
    if ( aim_pos == vec3_t( 0, 0, 0 ) ) {
        return;
    }

    if ( framework::m_b_open ) {
        return;
    }

    if ( entity.player_pawn.health <= 0 ) {
        return;
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

    if ( norm < _settings->rage_fov ) {
        if ( screen_pos.x != screen_center_x ) {
            target_x = ( screen_pos.x > screen_center_x ) ? -( screen_center_x - screen_pos.x ) : screen_pos.x - screen_center_x;
           // target_x /= 0.5f;
            target_x = ( target_x + screen_center_x > screen_center_x * 2 || target_x + screen_center_x < 0 ) ? 0 : target_x;
        }

        if ( screen_pos.y != 0 ) {
            if ( screen_pos.y != screen_center_y ) {
                target_y = ( screen_pos.y > screen_center_y ) ? -( screen_center_y - screen_pos.y ) : screen_pos.y - screen_center_y;
               // target_y /= 0.5f;
                target_y = ( target_y + screen_center_y > screen_center_y * 2 || target_y + screen_center_y < 0 ) ? 0 : target_y;
            }
        }

        /* should be much better now */
        if ( ( target_x < 2 && target_y < 2 ) && _esp->l_spotted( entity, local, local_idx, ent_idx ) ) {
            mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
            std::thread trigger_thread( release_mouse_event_rage );
            trigger_thread.detach( );
        }

   
        float distance_ratio = norm / _settings->rage_fov;
        float speed_factor = 1.0f + ( 1.0f - distance_ratio );
        target_x /= speed_factor;
        target_y /= speed_factor;

        if ( screen_pos.x != screen_center_x ) {
            target_x = ( screen_pos.x > screen_center_x ) ? -( screen_center_x - screen_pos.x ) : screen_pos.x - screen_center_x;
           // target_x /= 0.5f;
            target_x = ( target_x + screen_center_x > screen_center_x * 2 || target_x + screen_center_x < 0 ) ? 0 : target_x;
        }

        if ( screen_pos.y != 0 ) {
            if ( screen_pos.y != screen_center_y ) {
                target_y = ( screen_pos.y > screen_center_y ) ? -( screen_center_y - screen_pos.y ) : screen_pos.y - screen_center_y;
               // target_y /= 0.5f;
                target_y = ( target_y + screen_center_y > screen_center_y * 2 || target_y + screen_center_y < 0 ) ? 0 : target_y;
            }
        }
        mouse_event( MOUSEEVENTF_MOVE, target_x, target_y, NULL, NULL );
      
    }

}
