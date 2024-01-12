#include "custom.hpp"
using namespace ImGui;

void c_custom::render_arrows_for_horizontal_bar( ImVec2 pos, float alpha, float width, float height ) {

    auto draw = GetWindowDrawList();
    draw->AddRect( pos, pos + ImVec2( width, height ), ImColor( 0.f, 0.f, 0.f, alpha * GetStyle( ).Alpha ), 4 );
}

bool c_custom::expand_button( const char* label, bool selected, float rounding, ImDrawFlags flags ) {

    ImGuiWindow* window = GetCurrentWindow( );
    ImGuiID id = window->GetID( label );

    ImVec2 label_size = CalcTextSize( label, 0, 1 );
    ImVec2 icon_size = GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( GetIO( ).Fonts->Fonts[3]->FontSize - 2, FLT_MAX, 0, "A" );
    ImVec2 pos = window->DC.CursorPos;

    auto draw = window->DrawList;

    ImRect rect( pos, pos + ImVec2( GetWindowWidth( ), 45 ) );
    ItemAdd( rect, id );
    ItemSize( rect, GetStyle( ).FramePadding.y );

    bool hovered, held;
    bool pressed = ButtonBehavior( rect, id, &hovered, &held, NULL );

    static std::unordered_map < ImGuiID, float > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, 0.f } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, ( selected ? 1.f : 0.f ), 0.035f );

    draw->AddRectFilled( rect.Min, rect.Max, ImColor( 1.f, 1.f, 1.f, 0.025f * value->second ), rounding, flags );
    draw->AddText( GetIO( ).Fonts->Fonts[3], GetIO( ).Fonts->Fonts[3]->FontSize - 2, ImVec2( rect.Min.x + 50 / 2 - icon_size.x / 2, rect.GetCenter( ).y - icon_size.y / 2 ), GetColorU32( ImGuiCol_Text, ( value->second > 0.3f ? value->second : 0.3f ) ), "A" );
    draw->AddText( ImVec2( rect.Min.x + 50, rect.GetCenter( ).y - label_size.y / 2 ), GetColorU32( ImGuiCol_Text, ( value->second > 0.3f ? value->second : 0.3f ) ), label );

    return pressed;
}

bool c_custom::tab( const char* icon, const char* label, bool selected ) {

    ImGuiWindow* window = GetCurrentWindow( );
    ImGuiID id = window->GetID( label );

    ImVec2 label_size = CalcTextSize( label, 0, 1 );
    ImVec2 icon_size = GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( GetIO( ).Fonts->Fonts[3]->FontSize, FLT_MAX, 0, icon );
    ImVec2 pos = window->DC.CursorPos;

    auto draw = window->DrawList;

    ImRect rect( pos, pos + ImVec2( GetWindowWidth( ), 40 ) );
    ItemAdd( rect, id );
    ItemSize( rect, GetStyle( ).FramePadding.y );

    bool hovered, held;
    bool pressed = ButtonBehavior( rect, id, &hovered, &held, NULL );

    static std::unordered_map < ImGuiID, float > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, 0.f } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, ( selected || hovered ? 1.f : 0.f ), 0.035f );

    draw->AddText( GetIO( ).Fonts->Fonts[3], GetIO( ).Fonts->Fonts[3]->FontSize, ImVec2( rect.Min.x + 50 / 2 - icon_size.x / 2, rect.GetCenter( ).y - icon_size.y / 2 ), GetColorU32( ImGuiCol_Text, ( value->second > 0.3f ? value->second : 0.3f ) ), icon );
    draw->AddText( ImVec2( rect.Min.x + 50, rect.GetCenter( ).y - label_size.y / 2 ), GetColorU32( ImGuiCol_Text, ( value->second > 0.3f ? value->second : 0.3f ) ), label );

    return pressed;
}

bool c_custom::sub_tab( const char* label, bool selected ) {

    ImGuiWindow* window = GetCurrentWindow( );
    ImGuiID id = window->GetID( label );

    ImVec2 label_size = CalcTextSize( label, 0, 1 );
    ImVec2 pos = window->DC.CursorPos;

    auto draw = window->DrawList;

    ImRect rect( pos, pos + label_size );
    ItemAdd( rect, id );
    ItemSize( ImVec4( rect.Min.x, rect.Min.y, rect.Max.x + 15, rect.Max.y ), GetStyle( ).FramePadding.y );

    bool hovered, held;
    bool pressed = ButtonBehavior( rect, id, &hovered, &held, NULL );

    static std::unordered_map < ImGuiID, float > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, 0.f } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, ( selected ? 1.f : 0.f ), 0.035f );

    draw->AddText( rect.Min, GetColorU32( ImGuiCol_Text, ( value->second > 0.3f ? value->second : 0.3f ) ), label );
    draw->AddCircleFilled( ImVec2( rect.GetCenter( ).x, rect.Max.y + 10 ), 3 * value->second, custom.get_accent_color( value->second * GetStyle( ).Alpha ) );
    
    return pressed;
}

struct tab_area_structure {

    bool hovered, should_expand;
    float width;
};

void c_custom::tab_area( const char* str_id, ImVec2 size, std::function < void ( ) > content ) {

    static std::unordered_map < std::string, tab_area_structure > values;
    auto value = values.find( std::string( str_id ) );
    if ( value == values.end( ) ) {

        values.insert( { std::string( str_id ), { false, false, size.x } } );
        value = values.find( std::string( str_id ) );
    }

    value->second.width = ImLerp( value->second.width, ( value->second.should_expand && value->second.hovered ) ? size.x + 60 : size.x, 0.08f );

    if ( ( value->second.should_expand && value->second.hovered ) )
        GetForegroundDrawList( )->AddRectFilled( GetWindowPos( ) + ImVec2( value->second.width, 0 ), GetWindowPos( ) + GetWindowSize( ), ImColor( 0.f, 0.f, 0.f, 0.2f ), GetStyle( ).WindowRounding );

    PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );

    BeginChild( std::string( str_id ).append( ".child" ).c_str( ), ImVec2( value->second.width, size.y ) );

    value->second.hovered = IsMouseHoveringRect( GetCurrentWindow( )->Pos, GetCurrentWindow( )->Pos + ImVec2( value->second.width, size.y ) ); // could use IsWindowHovered( ); , but it bugs

    GetWindowDrawList( )->AddRectFilled( GetCurrentWindow( )->Pos, GetCurrentWindow( )->Pos + GetCurrentWindow( )->Size, ImColor( 28, 30, 36 ), GetStyle( ).WindowRounding, ImDrawFlags_RoundCornersTopLeft );

    if ( expand_button( "Expand", value->second.should_expand, GetStyle( ).WindowRounding, ImDrawFlags_RoundCornersTopLeft ) )
        value->second.should_expand = !value->second.should_expand;

    content( );

    EndChild( );

    PopStyleVar( );

}

bool c_custom::settings_widget( const char* u_id ) {

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImGuiID id = window->GetID( u_id );

    ImVec2 icon_size = GetIO( ).Fonts->Fonts[ 4 ]->CalcTextSizeA( GetIO( ).Fonts->Fonts[ 4 ]->FontSize + 1, FLT_MAX, 0, "7" );

    ImVec2 pos = window->DC.CursorPos;
    ImRect bb( pos + ImVec2( 0, 1 ), pos + icon_size + ImVec2( 0, 1 ) );

    auto draw = window->DrawList;

    ItemAdd( bb, id );
    ItemSize( bb );

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );

    static std::unordered_map < ImGuiID, float > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, { 0.f } } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, ( hovered ? 1.f : 0.5f ), 0.035f );

    //' ne4
    draw->AddText( GetIO( ).Fonts->Fonts[ 4 ], GetIO( ).Fonts->Fonts[ 4 ]->FontSize + 1, bb.Min, GetColorU32( ImGuiCol_Text, value->second ), "7" );

    return pressed;
}

void c_custom::prepared_popup( const char* id, const char* name, std::function<void( )> content ) {

    ImGui::SetNextWindowSize( { 230,230 } );

    ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 8,11 } );
    ImGui::PushStyleVar( ImGuiStyleVar_PopupRounding, 4 );
    ImGui::PushStyleColor( ImGuiCol_PopupBg, ImVec4( ImColor( 21, 20, 27 ) ) );
    PushStyleColor( ImGuiCol_Border, ImVec4( 1.f, 1.f, 1.f, 0.05f ) );

    if ( ImGui::BeginPopup( id ) ) {

        auto pos = ImGui::GetCurrentWindow( )->Pos, size = ImGui::GetCurrentWindow( )->Size;
        //ImGui::GetCurrentWindow( )->DrawList->AddText( ImGui::GetIO( ).Fonts->Fonts[ 2 ], ImGui::GetIO( ).Fonts->Fonts[ 2 ]->FontSize - 2, ImVec2( pos.x + 15, pos.y + 10 ), ImColor( 1.f, 1.f, 1.f ), name );
        //ImGui::GetCurrentWindow( )->DrawList->AddLine( ImVec2( pos.x + 10, pos.y + 35 ), ImVec2( pos.x + size.x - 10, pos.y + 35 ), ImColor( 1.f, 1.f, 1.f, 0.1f ), 2 );

        ImGui::SetCursorPos( { 10, 10 } );
        custom.group_box_alternative( id, ImVec2( size.x - 20, size.y - 20 ) );

        content( );

        custom.end_group_box_alternative( );


        ImGui::EndPopup( );
    }

    ImGui::PopStyleColor( 2 );
    ImGui::PopStyleVar( 2 );
}

void c_custom::group_box_alternative( const char* name, ImVec2 size_arg, ImVec2 padding ) {

    ImGuiWindow* window = GetCurrentWindow( );
    ImVec2 pos = window->DC.CursorPos;

    auto name_size = GetIO( ).Fonts->Fonts[ 1 ]->CalcTextSizeA( GetIO( ).Fonts->Fonts[ 1 ]->FontSize, FLT_MAX, 0.f, name );

    BeginChild( std::string( name ).append( ".main" ).c_str( ), size_arg, false, ImGuiWindowFlags_NoScrollbar );

    GetWindowDrawList( )->AddRectFilled( pos, pos + size_arg, to_vec4( 28, 30, 36, custom.m_anim * GetStyle( ).Alpha ), 3 );
    GetWindowDrawList( )->AddText( pos + ImVec2( 10, 8 ), custom.get_accent_color( custom.m_anim * GetStyle( ).Alpha ), name, FindRenderedTextEnd( name ) );

    SetCursorPosY( padding.y );
    BeginChild( name, { size_arg.x, size_arg.y - padding.y } );
    SetCursorPosX( padding.x );

    BeginGroup( );

    GetWindowDrawList( )->AddText( pos + ImVec2( 10, 8 ), custom.get_accent_color( custom.m_anim ), name, FindRenderedTextEnd( name ) );

    PushStyleVar( ImGuiStyleVar_ItemSpacing, { 8, 8 } );
    PushStyleVar( ImGuiStyleVar_Alpha, custom.m_anim * GetStyle( ).Alpha );
}

void c_custom::end_group_box_alternative( ) {

    PopStyleVar( 2 );
    EndGroup( );
    EndChild( );
    EndChild( );
}


void c_custom::begin_child( const char* name, ImVec2 size ) {

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImVec2 pos = window->DC.CursorPos;

    BeginChild( std::string( name ).append( ".main" ).c_str( ), size, false, ImGuiWindowFlags_NoScrollbar );

    GetWindowDrawList( )->AddRectFilled( pos, pos + size, to_vec4( 28, 30, 36, custom.m_anim ), 4 );
    GetWindowDrawList( )->AddText( pos + ImVec2( 10, 8 ), custom.get_accent_color( custom.m_anim ), name, FindRenderedTextEnd( name ) );

    SetCursorPosY( 30 );
    BeginChild( name, { size.x, size.y - 30 } );
    SetCursorPosX( 10 );

    BeginGroup( );

    PushStyleVar( ImGuiStyleVar_Alpha, custom.m_anim );
    PushStyleVar( ImGuiStyleVar_ItemSpacing, { 8, 8 } );
}

void c_custom::end_child( ) {

    PopStyleVar( 2 );
    EndGroup( );
    EndChild( );
    EndChild( );
}
