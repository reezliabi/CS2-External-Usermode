#include "../../inc.hpp"
#include "../animation_system/animation_system.hpp"
#include "render_fonts.hpp"

void unknowncheats::render_t::initialize_font_system( ) {
	ImGuiIO& io = ImGui::GetIO( ); /* get imgui io */

	/* create a new cfg for each type of font */
	ImFontConfig cfg;// = new ImFontConfig( );

	cfg.PixelSnapH = true;
	cfg.OversampleH = cfg.OversampleV = 1;

	/* ranges */
	static const ImWchar ranges[ ] =
	{
	   0x0020, 0x00FF,
	   0x0400, 0x052F,
	   0x2DE0, 0x2DFF,
	   0xA640, 0xA69F,
	   0x3131, 0x3163,
	   0xAC00, 0xD79D,
	   0,
	};

	auto add_font = [ & ]( const char* font, float size )->void {
		io.Fonts->AddFontFromFileTTF( font, size, &cfg, ranges );

		printf( "Loaded font: %s, Size: %.1f\n", font, size );
	};


	add_font( "C:\\Windows\\Fonts\\verdana.ttf", 12.0f );
	io.Fonts->AddFontFromMemoryTTF( _smallest_pixel, sizeof( _smallest_pixel ), 10.f );


	/* freetype build */
	ImGuiFreeType::BuildFontAtlas( io.Fonts, ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags_MonoHinting );
}

void unknowncheats::render_t::add_line( unknowncheats::vec2_t from, unknowncheats::vec2_t to, col_t c, float thickness ) {
	ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( from.x, from.y ), ImVec2( to.x, to.y ), c.convert( ), thickness );
}

void unknowncheats::render_t::add_text( int x, int y, col_t c, int font, const char* text, int flag ) {
	ImGuiIO& io = ImGui::GetIO( );
	ImGui::PushFont( io.Fonts->Fonts[ font ] );

	if ( flag == font_flags_t::dropshadow ) {
		ext_draw_list->AddText( unknowncheats::macros::vec_t( x + 1.f, y + 1.f ), col_t( 5, 5, 5 ).modify_alpha( c.a ).convert( ), text );
	}

	if ( flag == font_flags_t::outline ) {
		ext_draw_list->AddText( unknowncheats::macros::vec_t( x + 1, y - 1 ), col_t( 5, 5, 5 ).modify_alpha( c.a ).convert( ), text );
		ext_draw_list->AddText( unknowncheats::macros::vec_t( x - 1, y + 1 ), col_t( 5, 5, 5 ).modify_alpha( c.a ).convert( ), text );
		ext_draw_list->AddText( unknowncheats::macros::vec_t( x - 1, y - 1 ), col_t( 5, 5, 5 ).modify_alpha( c.a ).convert( ), text );
		ext_draw_list->AddText( unknowncheats::macros::vec_t( x + 1, y + 1 ), col_t( 5, 5, 5 ).modify_alpha( c.a ).convert( ), text );

		ext_draw_list->AddText( unknowncheats::macros::vec_t( x, y + 1 ), col_t( 5, 5, 5 ).modify_alpha( c.a ).convert( ), text );
		ext_draw_list->AddText( unknowncheats::macros::vec_t( x, y - 1 ), col_t( 5, 5, 5 ).modify_alpha( c.a ).convert( ), text );
		ext_draw_list->AddText( unknowncheats::macros::vec_t( x + 1, y ), col_t( 5, 5, 5 ).modify_alpha( c.a ).convert( ), text );
		ext_draw_list->AddText( unknowncheats::macros::vec_t( x - 1, y ), col_t( 5, 5, 5 ).modify_alpha( c.a ).convert( ), text );
	}

	/* text */
	ext_draw_list->AddText( unknowncheats::macros::vec_t( x, y ), c.convert( ), text );

	ImGui::PopFont( );
}

unknowncheats::vec2_t unknowncheats::render_t::text_size( const char* text, int font ) {
	ImGuiIO& io = ImGui::GetIO( );

	return {
		io.Fonts->Fonts[ font ]->CalcTextSizeA( io.Fonts->Fonts[ font ]->FontSize, FLT_MAX, -1.0f, text ).x,
		io.Fonts->Fonts[ font ]->CalcTextSizeA( io.Fonts->Fonts[ font ]->FontSize, FLT_MAX, -1.0f, text ).y
	};
}

void unknowncheats::render_t::add_rect_filled( int x, int y, int w, int h, unknowncheats::col_t col, int round ) {
	ext_draw_list->AddRectFilled( unknowncheats::macros::vec_t( x, y ), unknowncheats::macros::vec_t( x + w, y + h ), col.convert( ), round );
}

void unknowncheats::render_t::add_rect( int x, int y, int w, int h, unknowncheats::col_t col, int round, int tickness ) {
	ext_draw_list->AddRect( unknowncheats::macros::vec_t( x, y ), unknowncheats::macros::vec_t( x + w, y + h ), col.convert( ), round, 0, tickness );
}

unknowncheats::col_t unknowncheats::render_t::to_main_color( float color[ 4 ] ) {
	return unknowncheats::col_t( color[ 0 ] * 255.f, color[ 1 ] * 255.f, color[ 2 ] * 255.f, color[ 3 ] * 255.f );
}

void unknowncheats::render_t::add_filled_circle( unknowncheats::vec2_t center, float radius, unknowncheats::col_t c ) {
	ext_draw_list->AddCircleFilled( unknowncheats::macros::vec_t( center.x, center.y ), radius, c.convert( ) );
}

void unknowncheats::render_t::add_circle( unknowncheats::vec2_t center, float radius, unknowncheats::col_t c ) {
	ext_draw_list->AddCircle( unknowncheats::macros::vec_t( center.x, center.y ), radius, c.convert( ) );
}

unknowncheats::macros::u32_t lerp_color( unknowncheats::macros::u32_t col1, unknowncheats::macros::u32_t col2, float t ) {
	unknowncheats::macros::vec4_t c1 = ImGui::ColorConvertU32ToFloat4( col1 );
	unknowncheats::macros::vec4_t c2 = ImGui::ColorConvertU32ToFloat4( col2 );
	unknowncheats::macros::vec4_t lerped = unknowncheats::macros::vec4_t( c1.x + t * ( c2.x - c1.x ),
													  c1.y + t * ( c2.y - c1.y ),
													  c1.z + t * ( c2.z - c1.z ),
													  c1.w + t * ( c2.w - c1.w ) );
	return ImGui::ColorConvertFloat4ToU32( lerped );
}

void unknowncheats::render_t::add_gradient_circle_filled( unknowncheats::vec2_t c, int radius, unknowncheats::col_t inner_color, unknowncheats::col_t outer_color ) {
	for ( int i = 0; i <= radius; ++i ) {
		float t = static_cast< float >( i ) / radius;
		ImU32 color = ImGui::GetColorU32( lerp_color( inner_color.convert( ), outer_color.convert( ), t ) );
		ext_draw_list->AddCircleFilled( unknowncheats::macros::vec_t( c.x, c.y ), static_cast< float >( i ), color );
	}
}

void unknowncheats::render_t::add_gradient_vertical( int x, int y, int w, int h, unknowncheats::col_t c1, unknowncheats::col_t c2 ) {
	ext_draw_list->AddRectFilledMultiColor( unknowncheats::macros::vec_t( x, y ), unknowncheats::macros::vec_t( x + w, y + h ), c1.convert( ), c2.convert( ), c2.convert( ), c1.convert( ) );
}

void unknowncheats::render_t::add_gradient_horizontal( int x, int y, int w, int h, unknowncheats::col_t c1, unknowncheats::col_t c2 ) {
	ext_draw_list->AddRectFilledMultiColor( unknowncheats::macros::vec_t( x, y ), unknowncheats::macros::vec_t( x + w, y + h ), c1.convert( ), c1.convert( ), c2.convert( ), c2.convert( ) );
}

void unknowncheats::render_t::add_drop_shadow( int x, int y, int w, int h ) {
	this->add_gradient_horizontal( x, y - 5, w, 5, unknowncheats::col_t( 12, 26, 36, 0 ), unknowncheats::col_t( 12, 26, 36, 100 ) );
	this->add_gradient_horizontal( x, y + h, w, 5, unknowncheats::col_t( 12, 26, 36, 100 ), unknowncheats::col_t( 12, 26, 36, 0 ) );

	this->add_gradient_vertical( x - 5, y, 5, h, unknowncheats::col_t( 12, 26, 36, 0 ), unknowncheats::col_t( 12, 26, 36, 100 ) );
	this->add_gradient_vertical( x + w, y, 5, h, unknowncheats::col_t( 12, 26, 36, 100 ), unknowncheats::col_t( 12, 26, 36, 0 ) );
}

void unknowncheats::render_t::add_image( int x, int y, int w, int h, unknowncheats::macros::texture_id user_texture_id, unknowncheats::col_t c ) {
	ext_draw_list->AddImage( user_texture_id, unknowncheats::macros::vec_t( x, y ), unknowncheats::macros::vec_t( x + w, y + h ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), c.convert( ) );
}

void unknowncheats::render_t::bind_animation( int id, std::string text, unknowncheats::col_t color, int font, int x, int y, text_animation_t animation_type, float animation_speed ) {
	static int animation_[ 1000 ]; /* iterate */

	int w_of_text = this->text_size( text.c_str( ), font ).x;
	int h_of_text = this->text_size( text.c_str( ), font ).y;

	switch ( animation_type ) {
		case text_animation_t::left_to_right:
		{
			auto animation = animation_controller.get( text + std::to_string( id ) + animation_controller.current_child );
			animation.adjust( animation.value + 3.f * animation_controller.get_min_deltatime( animation_speed ) * ( ( animation_[ id ] < w_of_text ) ? 1.f : -1.f ) );

			if ( animation.value >= 0.99f ) {
				animation_[ id ] = w_of_text;
			} else if ( animation.value <= 0.f ) {
				animation_[ id ] = 0;
			}

			this->clip_rect( x, y, w_of_text * animation.value, h_of_text, [ & ]( ) {
				this->add_text( x, y, color, font, text.c_str( ) );
							 } );
		} break;
		case text_animation_t::middle_pulse:
		{
			auto animation = animation_controller.get( text + std::to_string( id ) + animation_controller.current_child );
			animation.adjust( animation.value + 3.f * animation_controller.get_min_deltatime( animation_speed ) * ( ( animation_[ id ] < w_of_text ) ? 1.f : -1.f ) );

			if ( animation.value >= 0.99f ) {
				animation_[ id ] = w_of_text;
			} else if ( animation.value <= 0.f ) {
				animation_[ id ] = 0;
			}

			this->clip_rect( x + ( w_of_text / 2 ) - ( ( ( w_of_text * 2 ) / 2 ) * ( animation.value / 2 ) ), y,
							 ( ( w_of_text * 2 ) / 2 ) * animation.value, h_of_text, [ & ]( ) {
								 this->add_text( x, y, color, font, text.c_str( ) );
							 } );
		} break;
		case text_animation_t::tiny_color:
		{
			auto animation = animation_controller.get( text + std::to_string( id ) + animation_controller.current_child );
			animation.adjust( animation.value + 3.f * animation_controller.get_min_deltatime( animation_speed ) * ( ( animation_[ id ] < w_of_text ) ? 1.f : -1.f ) );

			if ( animation.value >= 0.99f ) {
				animation_[ id ] = w_of_text;
			} else if ( animation.value <= 0.f ) {
				animation_[ id ] = 0;
			}

			this->clip_rect( ( x - 15 ) + ( w_of_text * animation.value ), y, 45, h_of_text, [ & ]( ) {
				this->add_text( x, y, color, font, text.c_str( ) );
							 } );
		} break;
	}
}

void unknowncheats::render_t::clip_rect( float x, float y, float w, float h, std::function< void( ) > function ) {
	ext_draw_list->PushClipRect( { x, y }, { x + w, y + h }, true );

	function( );

	ext_draw_list->PopClipRect( );
}