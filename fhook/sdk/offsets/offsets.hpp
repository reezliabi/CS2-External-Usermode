#pragma once

namespace unknowncheats {
	namespace offsets {
		namespace c_base_entity {
			inline DWORD health = 0x32C;
			inline DWORD team_id = 0x3BF;
			inline DWORD vec_velocity = 0x3D8;
			inline DWORD game_scene_node = 0x310;
			inline DWORD bone_array = 0x1E0; /* xref: CSkeletonInstance_::m_modelState + CGameSceneNode_::m_vecOrigin -> if this gets outdate */
		}

		namespace c_base_player_controler {
			inline DWORD player_pawn = 0x7EC;
			inline DWORD pawn_alive = 0x7F4;
			inline DWORD player_name = 0x640;
			inline DWORD ping = 0x720;

			inline DWORD has_hemlet = 0x801;
			inline DWORD has_defuser = 0x800;
		}

		namespace pawn {
			inline DWORD health = 0x32C;
			inline DWORD vec_old_origin = 0x1224;
			inline DWORD dormant = 0xE7;
			inline DWORD viewangle = 0x1518;
			inline DWORD camera_pos = 0x1294;
			inline DWORD ping_services = 0x1268;
			inline DWORD camera_services = 0x10E0;
			inline DWORD processing_value = 0x1F4;

			inline DWORD max_alpha = 0x146C;
			inline DWORD flash_Duration = 0x1470;
			inline DWORD flashbang_time = 0x145C;
			inline DWORD emit_sound_time = 0x1404;
			inline DWORD killed_by_hs = 0x1668;

			inline DWORD ent_index = 0x1544; /* should work */
			inline DWORD spotted = 0x1638 + 0xC; /* xref: m_entitySpottedState + m_bSpottedByMask  */
		}

		namespace post_processing {
			inline DWORD exposure_control = 0xD05; // bool
			inline DWORD min_exposure = 0xCEC; // f
			inline DWORD max_exposure = 0xCF0; // f
		}

		namespace item_services {
			inline DWORD item_services_pawn = 0x10B0;
			inline DWORD has_defuser = 0x40;
			inline DWORD has_hemlet = 0x41;
			inline DWORD has_heavy_armor = 0x42;
		}

		namespace ping_services {
			inline DWORD player_ping = 0x40;
		}

		namespace c_base_weapon {
			inline DWORD clipping_weapon = 0x12B0;
			inline DWORD wpn_data_ptr = 0x360;
			inline DWORD weapon_services = 0x10A8;
			inline DWORD active_weapon = 0x60;
			inline DWORD clip_1 = 0x1570;
			inline DWORD cycle_time = 0xC34;
			inline DWORD max_clip = 0x1FC;
			inline DWORD penetration = 0xD3C;
			inline DWORD weapon_type = 0x240;
			inline DWORD zs_name = 0xC18;
			inline DWORD inaccuracy = 0xC7C;
		}

		namespace c_base_smoke {
			inline DWORD effect_begin = 0x1108;
			inline DWORD effect_spawn = 0x1149;
			inline DWORD smoke_effect = 0x110C;
			inline DWORD smoke_color = 0x1114;
		}

		namespace controller {
			inline DWORD money_services = 0x700;
			inline DWORD tracking_services = 0x710;
			inline DWORD damage_services = 0x718;
		}

		namespace c_money_services {
			inline DWORD i_account = 0x40;
		}

		namespace damage_services {
			inline DWORD i_dmg = 0x58;
		}

		namespace match_making {
			inline DWORD m_i_competitive_ranking = 0x770; // int32_t
			inline DWORD m_i_competitive_wins = 0x774; // int32_t
			inline DWORD m_i_competitive_rank_type = 0x778; // int8_t
			inline DWORD m_i_competitive_ranking_predicted_win = 0x77C; // int32_t
			inline DWORD m_i_competitive_ranking_predicted_loss = 0x780; // int32_t
			inline DWORD m_i_competitive_ranking_predicted_tie = 0x784; // int32_t
		}

		namespace tracking_services {
			inline DWORD total_damage = 0x110;
		}

		namespace modulation {
			inline DWORD sky3d = 0x1168;
		}

		namespace glow {
			inline DWORD glow_propety = 0xB98; /* xref: player + glow_property */

			/* glow property 
				player + glow_property -> glow_address
				glow_address->glow_color/glowing
			*/
			inline DWORD glow_color = 0x8; /* xref: vector - clr[0-3] */
			inline DWORD glow_type = 0x30; /* xref: vector - clr[0-3] */
			inline DWORD glow_color_override = 0x40; /* xref: Color - clr[0-3] */
			inline DWORD glowing = 0x51; /* xref: bool */
		}

		namespace c_game_scene_mode {
			inline DWORD vec_origin = 0x80;
		}
	}

	namespace dragged_offsets {
		inline DWORD entity_list;
		inline DWORD matrix;
		inline DWORD view_angle;
		inline DWORD local_player_controller;
		inline DWORD local_player_pawn;
		inline DWORD force_jump;
		inline DWORD global_vars;
		inline DWORD inventory_services;
		inline DWORD planted_c4;
	}

	namespace signatures {
		const std::string inventory_services = "E8 ?? ?? ?? ?? 8B 45 D0 48 8B 55 D8"; // outdate
		const std::string global_vars = "48 89 0D ?? ?? ?? ?? 48 89 41"; // old?
		const std::string entity_list = "48 8B 0D ?? ?? ?? ?? 48 89 7C 24 ?? 8B FA C1 EB";
		const std::string local_player_controller = "48 8B 05 ?? ?? ?? ?? 48 85 C0 74 4F";
		const std::string view_angles = "48 8B 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC 40 55";
		const std::string view_matrix = "48 8D 0D ?? ?? ?? ?? 48 C1 E0 06";
		const std::string local_player_pawn = "48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC 48 83 EC ?? 8B 0D";
		const std::string force_jump = "48 8B 05 ?? ?? ?? ?? 48 8D 1D ?? ?? ?? ?? 48 89 45";
		const std::string planted_c4 = "48 8B 15 ?? ?? ?? ?? FF C0 48 8D 4C 24 40";
	}
}