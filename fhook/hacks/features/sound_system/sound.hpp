#pragma once

namespace unknowncheats {
	struct sound_info_t {
		vec3_t player_origin;
		bool has_changed;
	};

	class sound_info {
	public:
		void push_sound( const c_entity& entity );
		void push_sound2( const c_entity& entity, int idx );

		std::vector<sound_info_t> s_info;
		std::vector<sound_info_t> s_info_l;
	};

	inline const auto _sound = std::make_unique< sound_info >( );
}