#pragma once

namespace unknowncheats {
	enum bone_index : DWORD {
		head = 6,
		neck_0 = 5,
		spine_1 = 4,
		spine_2 = 2,
		pelvis = 0,
		arm_upper_L = 8,
		arm_lower_L = 9,
		hand_L = 10,
		arm_upper_R = 13,
		arm_lower_R = 14,
		hand_R = 15,
		hand_R2 = 16,
		leg_upper_L = 22,
		leg_lower_L = 23,
		ankle_L = 24,
		leg_upper_R = 25,
		leg_lower_R = 26,
		ankle_R = 27,
	};

	struct bone_data { /* oh shit */
		vec3_t pos;
		char pad[ 0x14 ];
	};

	struct bone_pos {
		vec3_t pos;
		vec2_t screen_pos;
		bool is_visible = false;
	};

	class bone_t {
	private:
		DWORD64 pawn_address = 0;
	public:
		std::vector<bone_pos> bone_pos_list;

		__forceinline bool update_bone_data( const DWORD64& pawn_address ) {
			if ( pawn_address == 0 ) {
#if 1
				/* debug */
				printf( "[unknowncheats] player controller is 0\n" );
#endif 
				return false;
			}

			this->pawn_address = pawn_address;

			/* stuff */
			DWORD64 game_scene_node = 0;
			DWORD64 bone_array_address = 0;

			if ( !_proc_manager.read_memory<DWORD64>( pawn_address + offsets::c_base_entity::game_scene_node, game_scene_node ) ) {
#ifdef read_data_dbg
				print_with_data_scoped( "bone_t::pawn_address -> error -> no memory" );
#endif // read_data_dbg
				return false;
			}

			if ( !_proc_manager.read_memory<DWORD64>( game_scene_node + offsets::c_base_entity::bone_array, bone_array_address ) ) {
#ifdef read_data_dbg
				print_with_data_scoped( "bone_t::scene_node -> error -> no memory" );
#endif // read_data_dbg
				return false;
			}

			bone_data bone_array[ 30 ]{};
			if ( !_proc_manager.read_memory( bone_array_address, bone_array, 30 * sizeof( bone_data ) ) ) {
#ifdef read_data_dbg
				print_with_data_scoped( "bone_t::bone_array_address -> error -> no memory bone_array" );
#endif // read_data_dbg
				return false;
			}

			for ( int i = 0; i < 30; i++ ) {
				unknowncheats::vec2_t screen_pos;
				bool is_visible = false;

				if ( unknowncheats::_address->view.world_to_screen( bone_array[ i ].pos, screen_pos ) ) {
					is_visible = true;
				}

				this->bone_pos_list.push_back( { bone_array[ i ].pos, screen_pos, is_visible } );
			}

			return this->bone_pos_list.size( ) > 0;
		}
	};

	namespace bone_list {
		inline std::list<DWORD> trunk = { neck_0,spine_2, pelvis };
		inline std::list<DWORD> left_arm = { neck_0,  arm_upper_L, arm_lower_L, hand_L };
		inline std::list<DWORD> right_arm = { neck_0, arm_upper_R,arm_lower_R, hand_R };
		inline std::list<DWORD> left_leg = { pelvis, leg_upper_L , leg_lower_L, ankle_L };
		inline std::list<DWORD> right_leg = { pelvis, leg_upper_R , leg_lower_R, ankle_R };
		inline std::vector<std::list<DWORD>> hitbox_list = { trunk, left_arm, right_arm, left_leg, right_leg };
	}
}