#pragma once

namespace unknowncheats {
	enum fonts_t {
		_default,
		_default_2
	};

	enum font_flags_t {
		none,
		dropshadow,
		outline
	};

	enum text_animation_t {
		left_to_right,
		middle_pulse,
		tiny_color
	};

	class render_t {
	public:
		void initialize_font_system( );

		void add_line( unknowncheats::vec2_t from, unknowncheats::vec2_t to, col_t c, float thickness );

		void add_text( int x, int y, col_t c, int font, const char* text, int /* or font_flags_t */ flag = font_flags_t::none );
		void add_rect_filled( int x, int y, int w, int h, unknowncheats::col_t col, int round );
		void add_rect( int x, int y, int w, int h, unknowncheats::col_t col, int round, int tickness );
		unknowncheats::col_t to_main_color( float color[ 4 ] );
		void add_filled_circle( unknowncheats::vec2_t center, float radius, unknowncheats::col_t c );
		void add_circle( unknowncheats::vec2_t center, float radius, unknowncheats::col_t c );
		void add_gradient_circle_filled( unknowncheats::vec2_t c, int radius, unknowncheats::col_t inner_color, unknowncheats::col_t outer_color );
		void add_gradient_vertical( int x, int y, int w, int h, unknowncheats::col_t c1, unknowncheats::col_t c2 );
		void add_gradient_horizontal( int x, int y, int w, int h, unknowncheats::col_t c1, unknowncheats::col_t c2 );
		void add_drop_shadow( int x, int y, int w, int h );
		void add_image( int x, int y, int w, int h, unknowncheats::macros::texture_id user_texture_id, unknowncheats::col_t c );

		void bind_animation( int id, std::string text, unknowncheats::col_t color, int font, int x, int y, text_animation_t animation_type, float animation_speed );

		void clip_rect( float x, float y, float w, float h, std::function<void( )> function );

		unknowncheats::vec2_t text_size( const char* text, int font );
		std::vector<ImFont*> fonts{};
	};

	inline const auto _render = std::make_unique<render_t>( );
}