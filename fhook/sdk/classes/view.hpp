#pragma once

namespace unknowncheats {
	class view_t {
	public:
		float matrix[ 4 ][ 4 ]{};

		bool world_to_screen( const vec3_t& Pos, vec2_t& ToPos ) {
			float View = 0.f;
			float SightX = ( ImGui::GetIO( ).DisplaySize.x / 2 ), SightY = ( ImGui::GetIO( ).DisplaySize.y / 2 );

			View = matrix[ 3 ][ 0 ] * Pos.x + matrix[ 3 ][ 1 ] * Pos.y + matrix[ 3 ][ 2 ] * Pos.z + matrix[ 3 ][ 3 ];

			if ( View <= 0.01 )
				return false;

			ToPos.x = SightX + ( matrix[ 0 ][ 0 ] * Pos.x + matrix[ 0 ][ 1 ] * Pos.y + matrix[ 0 ][ 2 ] * Pos.z + matrix[ 0 ][ 3 ] ) / View * SightX;
			ToPos.y = SightY - ( matrix[ 1 ][ 0 ] * Pos.x + matrix[ 1 ][ 1 ] * Pos.y + matrix[ 1 ][ 2 ] * Pos.z + matrix[ 1 ][ 3 ] ) / View * SightY;

			return true;
		}

		vec2_t get_screen_center( ) {
			vec2_t Pos;
			Pos.x = ImGui::GetIO( ).DisplaySize.x / 2;
			Pos.y = ImGui::GetIO( ).DisplaySize.y / 2;
			return Pos;
		}
	};
}