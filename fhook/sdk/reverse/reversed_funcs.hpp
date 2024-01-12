#pragma once
#include "../../inc.hpp"

namespace unknowncheats {
	class c_weapon_data {
	public:
	};

	class CCSWeaponBaseVData {
	public:
	};

	class reverse_t {
	public:
		c_weapon_data* get_wpn_data( ) {
			// sigscan: E8 ? ? ? ? 40 32 F6 F3 0F 10 3D ? ? ? ? -> im not tho very sure
			// return client.offset( "client.dll", "E8 ? ? ? ? 40 32 F6 F3 0F 10 3D ? ? ? ?" )
			return nullptr;
		}

		CCSWeaponBaseVData* get_wpn_data2( ) { 
			// #STR: "CCSWeaponBaseVData"
			// return client.offset( "client.dll", "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8B 81 ? ? ? ? F3 0F 10 80 ? ? ? ?" )
			return nullptr;
		}

		void __fastcall emit_sound( ) {
			// E8 ? ? ? ? 41 2B F6
			// #STR: "CBaseEntity::EmitSound", "Sound"
		}

		std::string get_weapon_name( ) {
			// #STR: "Weapon '%s' script file not found, but its data was access, "<unknown>"
			// FF 15 ? ? ? ? 84 C0 74 01 CC 48 8B C3

			return "";
		}

		void __fastcall send_last_dmg_to_killer( ) {
			// 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 40 4C 89 74 24 ?
		}
	};

	namespace signatures {
		/*	xref: // #STR: "Client Game", "Physics_SimulateEntity"
			40 53 48 83 EC 30 48 8B D9 BA ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 85 C0 75 0B 48 8B 05 ? ? ? ? 48 8B 40 08 80 38 00 75 09
		*/
	}

	inline const auto _reverse = std::make_unique< reverse_t >( );
}