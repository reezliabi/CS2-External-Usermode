#include "../inc.hpp"
#include <Uxtheme.h>
#include <dwmapi.h>
#include "../thirdparty/bytes.hpp"
#include "../thirdparty/custom.hpp"
#include "../sdk/render/render_fonts.hpp"
LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
    if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wp, lp ) ) {
        return 0;
    }
    switch ( msg ) {
        case WM_SYSCOMMAND:
        {
            if ( ( wp & 0xfff0 ) == SC_KEYMENU )
                return 0;
            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage( 0 );
            return 0;
        }
    }
    return DefWindowProc( hwnd, msg, wp, lp );
}


namespace unknowncheats::framework {
	bool render( ) {
        // get screen resolution
        m_i_width = GetSystemMetrics( SM_CXSCREEN );
        m_i_height = GetSystemMetrics( SM_CYSCREEN );

        if ( !m_b_initialized )
            return false;

        static bool bRunning = true;
        if ( bRunning ) {
            MSG message;
            while ( PeekMessage( &message, nullptr, 0U, 0U, PM_REMOVE ) ) {
                TranslateMessage( &message );
                DispatchMessage( &message );
            }
            if ( message.message == WM_QUIT )
                bRunning = false;

            static bool bToggled = false;
            if ( GetAsyncKeyState( VK_INSERT ) & 1 && !bToggled ) {
                m_b_open = !m_b_open;
                SetForegroundWindow( instance );
                bToggled = true;
            } else if ( !GetAsyncKeyState( VK_INSERT ) & 1 ) {
                LONG_PTR windowStyle = GetWindowLongPtr( instance, GWL_EXSTYLE );
                SetWindowLongPtr( instance, GWL_EXSTYLE, windowStyle | WS_EX_TRANSPARENT );
                bToggled = false;
            }

            if ( GetAsyncKeyState( VK_END ) & 1 ) {
                return 0;
            }

            ImGui_ImplDX11_NewFrame( );
            ImGui_ImplWin32_NewFrame( );

            ImGui::NewFrame( );

            if ( m_b_open ) {
                LONG_PTR windowStyle = GetWindowLongPtr( instance, GWL_EXSTYLE );
                windowStyle &= ~WS_EX_TRANSPARENT;
                SetWindowLongPtr( instance, GWL_EXSTYLE, windowStyle );

                _menu->render( );
            }

            unknowncheats::_hacks->run( );

            ImDrawList* pBackgroundDrawList = ImGui::GetBackgroundDrawList( );
            // Draw::RenderDrawData( pBackgroundDrawList ); rendering

            ImGui::Render( );

            constexpr float flColor[ 4 ] = { 0.f, 0.f, 0.f, 0.f };
            p_context->OMSetRenderTargets( 1U, &p_render_target_view, NULL );
            p_context->ClearRenderTargetView( p_render_target_view, flColor );

            ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

            p_swap_chain->Present( 1U, 0U );
        }

        if ( !bRunning ) {
            destroy( );
            return false;
        }

        return true;
    }

    bool create( ) {
        if ( m_b_initialized )
            return true;

        m_i_width = GetSystemMetrics( SM_CXSCREEN );
        m_i_height = GetSystemMetrics( SM_CYSCREEN );

        window_class.cbSize = sizeof( WNDCLASSEX );
        window_class.style = 0;
        window_class.lpfnWndProc = wnd_proc;
        window_class.hInstance = h_moudle;
        window_class.lpszClassName = ( L"unknowncheats-external" );

        RegisterClassExW( &window_class );

        instance = CreateWindowExW(
            WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
            window_class.lpszClassName, ( L"" ), WS_POPUP | WS_VISIBLE,
            0, 0, m_i_width, m_i_height, nullptr, nullptr, h_moudle, nullptr );

        if ( !SetLayeredWindowAttributes( instance, RGB( 0, 0, 0 ), BYTE( 255 ), LWA_ALPHA ) )
            throw std::runtime_error( ( "failed to set layered window attributes" ) );

        {
            RECT clientArea = { };
            if ( !GetClientRect( instance, &clientArea ) )
                throw std::runtime_error( ( "failed to get client rect" ) );

            RECT windowArea = { };
            if ( !GetWindowRect( instance, &windowArea ) )
                throw std::runtime_error( ( "failed to get window rect" ) );

            POINT diff = { };
            if ( !ClientToScreen( instance, &diff ) )
                throw std::runtime_error( ( "failed to get client to screen" ) );

            const MARGINS margins{
                windowArea.left + ( diff.x - windowArea.left ),
                windowArea.top + ( diff.y - windowArea.top ),
                windowArea.right,
                windowArea.bottom
            };

            if ( FAILED( DwmExtendFrameIntoClientArea( instance, &margins ) ) )
                throw std::runtime_error( ( "failed to extend frame into client area" ) );
        }

        // get refreshrate
        HDC hDC = GetDC( instance );
        m_u_refresh_rate = GetDeviceCaps( hDC, VREFRESH );
        ReleaseDC( instance, hDC );

        DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
        swapChainDesc.BufferDesc.RefreshRate.Numerator = m_u_refresh_rate;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1U;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.SampleDesc.Count = 1U;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2U;
        swapChainDesc.OutputWindow = instance;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        constexpr D3D_FEATURE_LEVEL levels[ 2 ]
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0
        };

        D3D_FEATURE_LEVEL level = { };

        if ( D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0U,
            levels,
            2U,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &p_swap_chain,
            &p_device,
            &level,
            &p_context ) != S_OK )
            return false;

        ID3D11Texture2D* pBackBuffer = nullptr;
        p_swap_chain->GetBuffer( 0U, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
        if ( pBackBuffer ) {
            p_device->CreateRenderTargetView( pBackBuffer, nullptr, &p_render_target_view );
            pBackBuffer->Release( );
        } else throw std::runtime_error( ( "failed to get back buffer" ) );

        SetWindowLong( instance, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW );
        ShowWindow( instance, SW_SHOW );
        UpdateWindow( instance );

        ImGui::CreateContext( );
        ImGui::StyleColorsDark( );

        ImGui_ImplWin32_Init( instance );
        ImGui_ImplDX11_Init( p_device, p_context );

        static bool o = false;
        if ( !o ) {
            _render->initialize_font_system( );
            o = true;
        }

        _menu->initialize( );

        printf( "[unknowncheats] created ui!\n" );

        m_b_initialized = true;
    }

    void destroy( ) {
        ImGui_ImplDX11_Shutdown( );
        ImGui_ImplWin32_Shutdown( );

        ImGui::DestroyContext( );

        if ( p_swap_chain )
            p_swap_chain->Release( );

        if ( p_context )
            p_context->Release( );

        if ( p_device )
            p_device->Release( );

        if ( p_render_target_view )
            p_render_target_view->Release( );

        if ( !DestroyWindow( instance ) )
            throw std::runtime_error( ( "failed to destroy window" ) );

        if ( !UnregisterClassW( window_class.lpszClassName, window_class.hInstance ) )
            throw std::runtime_error( ( "failed to destroy window" ) );
    }
}

void unknowncheats::menu_t::render( ) { 
    using namespace ImGui;

    static bool bools[ 50 ]{};
    static int ints[ 50 ]{};
    vector < const char* > items = { "Head", "Chest", "Body", "Legs", "Feet" };
    vector < const char* > cfgs = { "default", "legit", "auto", "scout", "other" };

    vector < const char* > animation_types = { "Left to right",
                                        "Middle pulse",
                                        "Tiny color" };

    vector < const char* > keymode = { "Hold",
                                    "Toggle",
                                    "Always" };

    vector < const char* > keymode2 = { "Prefer head",
                                "Prefer baim" };

    vector < const char* > stuff21 = { "Normal",
                            "Gradient" };

    vector < const char* > hitsound = {
                       "Neverlose",
                       "Skeet",
                       "Primordial",
                       "Sound 1",
                       "Sound 2",
                       "Toy duck",
                       "Bell 1",
                       "Bell 2",
                       "Button",
                       "Pop",
                       "Wow"
    };

    vector < const char* > hitboxes = { "Head",
                                "Neck",
                                "Chest", "Pelvis" };

    vector < const char* > key_binds = { "none", "mouse1", "mouse2", "mouse3", "mouse4", "mouse5", "a", 
        "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", 
        "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "f1", 
        "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "alt" };

    static float color[ 4 ] = { 1.f, 1.f, 1.f, 1.f };

    PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0, 0 ) );
    PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 510, 380 ) );

    ImGui::Begin( "Hello, world!", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse ); {
        auto window = GetCurrentWindow( );
        auto draw = window->DrawList;
        auto pos = window->Pos;
        auto size = window->Size;
        auto style = GetStyle( );

        custom.m_anim = ImLerp( custom.m_anim, 1.f, 0.03f );

        draw->AddLine( pos + ImVec2( 65, 40 ), pos + ImVec2( size.x - 15, 40 ), ImColor( 1.f, 1.f, 1.f, 0.05f ) );

        //PushFont( ImGui::GetIO( ).Fonts->Fonts[ sizeof( unknowncheats::fonts_t ) + 1 ] );

        switch ( custom.m_tab ) {
            case 0:
            {
                SetCursorPos( ImVec2( 70, 13 ) );
                BeginGroup( );
                {
                    for ( int i = 0; i < custom.rage_subtabs.size( ); ++i ) {

                        if ( custom.sub_tab( custom.rage_subtabs.at( i ), custom.m_rage_subtab == i ) && custom.m_rage_subtab != i )
                            custom.m_rage_subtab = i, custom.m_anim = 0.f;

                        if ( i != custom.rage_subtabs.size( ) - 1 )
                            ImGui::SameLine( );
                    }
                }

                EndGroup( );

                SetCursorPos( ImVec2( 65, 50 ) );
                BeginChild( "##rage_childs", ImVec2( GetWindowWidth( ) - 80, GetWindowHeight( ) - 80 ) );
                {

                    switch ( custom.m_rage_subtab ) {

                        case 0:
                        {
                            custom.begin_child( "General", ImVec2( GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( ) ) );
                            {
                                ImGui::Checkbox( "Enable", &unknowncheats::_settings->aimbot );

                                ImGui::Checkbox( "Draw fov", &unknowncheats::_settings->draw_fov );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                ImGui::ColorEdit4( "###foviclo", unknowncheats::_settings->fov_color, ALPHA );

                                ImGui::Checkbox( "Only visible", &unknowncheats::_settings->visible_check );
                                ImGui::Combo( "Aimbot key", &unknowncheats::_settings->a_triggerkey, key_binds.data( ), key_binds.size( ) );
                                ImGui::Combo( "Aimbot mode", &unknowncheats::_settings->a_activationz_type, keymode.data( ), keymode.size( ) );
                                ImGui::Combo( "Hitbox ", &unknowncheats::_settings->hitbox, hitboxes.data( ), hitboxes.size( ) );
                                ImGui::Checkbox( "Body-aim if lethal", &unknowncheats::_settings->legitbot_stuff[9] );

                                // ImGui::SliderInt( "Max distance", &unknowncheats::_settings->aim_distance_max, 200, 10000 );

                                ImGui::Checkbox( "Dynamic fov", &unknowncheats::_settings->legitbot_stuff[0] );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                if ( custom.settings_widget( "##aimbot.enable.settings" ) ) OpenPopup( "##aimbot.enable.popup" );
                                custom.prepared_popup( "##aimbot.enable.popup", "Dynamic fov", [ ]( ) {
                                    ImGui::Checkbox( "Enemy moving", &unknowncheats::_settings->legitbot_stuff[ 1 ] );
                                    ImGui::Checkbox( "Enemy lethal", &unknowncheats::_settings->legitbot_stuff[ 2 ] );
                                    ImGui::Checkbox( "Local lethal", &unknowncheats::_settings->legitbot_stuff[ 3 ] );
                                    ImGui::Checkbox( "Dealt alot of damage", &unknowncheats::_settings->legitbot_stuff[ 7 ] );
                                    if ( unknowncheats::_settings->legitbot_stuff[ 7 ] ) {
                                        ImGui::SliderInt( "Dealt damage", &unknowncheats::_settings->legitbot_stuff_int[0], 100, 500 );
                                    }

                                                       } );
                               
                                ImGui::Checkbox( "Dynamic smooth", &unknowncheats::_settings->legitbot_stuff[ 4 ] );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                if ( custom.settings_widget( "##popup2" ) ) OpenPopup( "##popup2" );
                                custom.prepared_popup( "##popup2", "Dynamic smooth", [ ]( ) {
                                    ImGui::Checkbox( "Enemy moving", &unknowncheats::_settings->legitbot_stuff[ 5 ] ); // 0.5
                                    ImGui::Checkbox( "Local lethal", &unknowncheats::_settings->legitbot_stuff[ 6 ] ); // 0.5
                                    ImGui::Checkbox( "Dealt alot of damage", &unknowncheats::_settings->legitbot_stuff[ 8 ] );
                                    if ( unknowncheats::_settings->legitbot_stuff[ 8 ] ) {
                                        ImGui::SliderInt( "Dealt damage", &unknowncheats::_settings->legitbot_stuff_int[ 1 ], 100, 500 );
                                    }
                                                       } );


                                ImGui::SliderFloat( "Fov", &unknowncheats::_settings->fov, 0, 10 );                             
                                ImGui::SliderFloat( "Smooth", &unknowncheats::_settings->smooth, 0, 10 );




                            } custom.end_child( );

                            ImGui::SameLine( );

                            custom.begin_child( "Other", ImVec2( GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( ) ) );
                            {
                                ImGui::Checkbox( "Triggerbot", &unknowncheats::_settings->triggerbot );
                                ImGui::Combo( "Triggerbot key", &unknowncheats::_settings->triggerkey, key_binds.data( ), key_binds.size( ) );
                                ImGui::Combo( "Triggerbot mode", &unknowncheats::_settings->activationz_type, keymode.data( ), keymode.size( ) );

                                ImGui::SliderInt( "Next reaction time", &unknowncheats::_settings->reaction_time, 200, 2500 );
                                ImGui::SliderInt( "Next shot delay", &unknowncheats::_settings->shot_delay, 10, 2500 );

                            } custom.end_child( );
                        } break;

                            
                        case 1:
                        {
                            custom.begin_child( "General", ImVec2( GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( ) ) );
                            {
                                ImGui::Checkbox( "Enable", &unknowncheats::_settings->rage );
                                ImGui::Checkbox( "Ignore autowall", &unknowncheats::_settings->ignore_wall );
                                ImGui::Checkbox( "Multipoint", &unknowncheats::_settings->ragebot_stuff[0] );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                if ( custom.settings_widget( "##popup3" ) ) OpenPopup( "##popup3" );
                                custom.prepared_popup( "##popup3", "Multipoint", [ & ]( ) {
                                    ImGui::Combo( "Prefer..", &unknowncheats::_settings->ragebot_stuff2[0], keymode2.data( ), keymode2.size( ) );
                                    ImGui::Checkbox( "Baim while lethal", &unknowncheats::_settings->ragebot_stuff[ 1 ] );
                                    ImGui::Checkbox( "Baim unsafe", &unknowncheats::_settings->ragebot_stuff[ 2 ] );

                                } );

                               // ImGui::Combo( "Hitbox ", &unknowncheats::_settings->rage_hitbox, hitboxes.data( ), hitboxes.size( ) );

                                ImGui::Checkbox( "Ignore if dist is over max", &unknowncheats::_settings->ignore_if_Distance_tO_high );
                                if ( unknowncheats::_settings->ignore_if_Distance_tO_high )
                                    ImGui::SliderInt( "Max aim distance", &unknowncheats::_settings->distance_to_rage, 200, 3000 );

                                std::string name_str = unknowncheats::_settings->rage_fov > 10 ? "Fov (!)" : "Fov";
                                ImGui::SliderInt( "Fov", &unknowncheats::_settings->rage_fov, 1, 20 );



                            } custom.end_child( );

                            ImGui::SameLine( );

                            custom.begin_child( "Other", ImVec2( GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( ) ) );
                            {


                            } custom.end_child( );
                        } break;

                    }

                } EndChild( );

            } break;
            case 1:
            {
                SetCursorPos( ImVec2( 70, 13 ) );
                BeginGroup( );

                for ( int i = 0; i < custom.visuals_subtabs.size( ); ++i ) {

                    if ( custom.sub_tab( custom.visuals_subtabs.at( i ), custom.m_visuals_subtab == i ) && custom.m_visuals_subtab != i )
                        custom.m_visuals_subtab = i, custom.m_anim = 0.f;

                    if ( i != custom.visuals_subtabs.size( ) - 1 )
                        ImGui::SameLine( );
                }

                EndGroup( );

                SetCursorPos( ImVec2( 65, 50 ) );
                BeginChild( "##visuals_childs", ImVec2( GetWindowWidth( ) - 80, GetWindowHeight( ) - 80 ) );
                {

                    switch ( custom.m_visuals_subtab ) {

                        case 0:

                            custom.begin_child( "ESP", ImVec2( GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( ) ) );
                            {
                                ImGui::Checkbox( "Spotted esp", &unknowncheats::_settings->change_by_visibility );
                                ImGui::Checkbox( "Dormancy esp", &unknowncheats::_settings->dormancy );
                                ImGui::Checkbox( "Bounding box", &unknowncheats::_settings->bounding_box );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                ImGui::ColorEdit4( "###boxcolk", unknowncheats::_settings->box_color, ALPHA );
                                if ( unknowncheats::_settings->change_by_visibility ) {
                                    ImGui::SameLine( GetWindowWidth( ) - 55 );
                                    ImGui::ColorEdit4( "###boxcolkspot", unknowncheats::_settings->box_color_inv, ALPHA );
                                }



                                ImGui::Checkbox( "Name", &unknowncheats::_settings->name_esp );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                ImGui::ColorEdit4( "###namecls", unknowncheats::_settings->name_color, ALPHA );
                                if ( unknowncheats::_settings->change_by_visibility ) {
                                    ImGui::SameLine( GetWindowWidth( ) - 55 );
                                    ImGui::ColorEdit4( "###namceks", unknowncheats::_settings->name_color_inv, ALPHA );
                                }

                                if ( unknowncheats::_settings->name_esp ) {
                                    ImGui::Checkbox( "Name animation", &unknowncheats::_settings->name_animation );
                                    ImGui::SameLine( GetWindowWidth( ) - 33 );
                                    ImGui::ColorEdit4( "###nameanimcl", unknowncheats::_settings->name_color_a, ALPHA );

                                    ImGui::Combo( "Name animation type", &unknowncheats::_settings->name_at, animation_types.data( ), animation_types.size( ) );
                                }

                                ImGui::Checkbox( "Health bar", &unknowncheats::_settings->health_bar );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                if ( custom.settings_widget( "##popup7" ) ) OpenPopup( "##popup7" );
                                custom.prepared_popup( "##popup7", "Health bar", [ & ]( ) {
                                    ImGui::Checkbox( "Custom color", &unknowncheats::_settings->customhealthbar );
                                    ImGui::SameLine( GetWindowWidth( ) - 36 );
                                    ImGui::ColorEdit4( "###healthbari", unknowncheats::_settings->healthbar, ALPHA );
                                    if ( unknowncheats::_settings->change_by_visibility ) {
                                        ImGui::SameLine( GetWindowWidth( ) - 55 );
                                        ImGui::ColorEdit4( "###heabarb", unknowncheats::_settings->healthbari, ALPHA );
                                    }
                                    ImGui::Combo( "Color type", &unknowncheats::_settings->visuals_i[0], stuff21.data( ), stuff21.size( ) );
                                    if ( unknowncheats::_settings->visuals_i[ 0 ] == 1 ) {
                                        ImGui::Text( "Second color" );
                                        ImGui::SameLine( GetWindowWidth( ) - 36 );
                                        ImGui::ColorEdit4( "###healthbari22", unknowncheats::_settings->visuals_c[0], ALPHA );
                                    }

                                                       } );

                                

                                ImGui::Checkbox( "Ammo bar", &unknowncheats::_settings->ammobar );
                                ImGui::SameLine( GetWindowWidth( ) - 55 );
                                ImGui::ColorEdit4( "###ammobar", unknowncheats::_settings->ammobar_color, ALPHA );
                                if ( unknowncheats::_settings->change_by_visibility ) {
                                    ImGui::SameLine( GetWindowWidth( ) - 77 );
                                    ImGui::ColorEdit4( "###bajsdggqd", unknowncheats::_settings->ammobar_color_inv, ALPHA );
                                }
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                if ( custom.settings_widget( "##popup8" ) ) OpenPopup( "##popup8" );
                                custom.prepared_popup( "##popup8", "Ammo bar", [ & ]( ) {
                                    ImGui::Combo( "Color type", &unknowncheats::_settings->visuals_i[ 1 ], stuff21.data( ), stuff21.size( ) );
                                    if ( unknowncheats::_settings->visuals_i[ 1 ] == 1 ) {
                                        ImGui::Text( "Second color" );
                                        ImGui::SameLine( GetWindowWidth( ) - 36 );
                                        ImGui::ColorEdit4( "###healthbar123i22", unknowncheats::_settings->visuals_c[ 1 ], ALPHA );
                                    }

                                                       } );

                               

                                ImGui::Checkbox( "Distance", &unknowncheats::_settings->visuals_b[0] );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                ImGui::ColorEdit4( "###wepoasdasdsansadhg", unknowncheats::_settings->visuals_c[2], ALPHA );

                                ImGui::Checkbox( "Weapon name", &unknowncheats::_settings->eap );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                ImGui::ColorEdit4( "###weponsadhg", unknowncheats::_settings->eap_color, ALPHA );
                                if ( unknowncheats::_settings->change_by_visibility ) {
                                    ImGui::SameLine( GetWindowWidth( ) - 55 );
                                    ImGui::ColorEdit4( "###weapon12hsa", unknowncheats::_settings->eap_color_inv, ALPHA );
                                }

                                ImGui::Checkbox( "Head bone", &unknowncheats::_settings->bones_h );
                                ImGui::Checkbox( "Skeleton", &unknowncheats::_settings->bones );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                ImGui::ColorEdit4( "###bonesh12", unknowncheats::_settings->bone_color, ALPHA );
                                if ( unknowncheats::_settings->change_by_visibility ) {
                                    ImGui::SameLine( GetWindowWidth( ) - 55 );
                                    ImGui::ColorEdit4( "###onesi12", unknowncheats::_settings->bone_color_inv, ALPHA );
                                }

                                ImGui::Checkbox( "Flags", &unknowncheats::_settings->flags );
                                ImGui::Checkbox( "Show competitive wins", &unknowncheats::_settings->show_competivie_wins );
                                ImGui::Checkbox( "Show dealt damage that round", &unknowncheats::_settings->show_dmg_dealt );
                            } custom.end_child( );

                            ImGui::SameLine( );

                            custom.begin_child( "Other stuff", ImVec2( GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( ) ) );
                            {
                                ImGui::Checkbox( "Colored smoke (!)", &unknowncheats::_settings->change_smoke );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                ImGui::ColorEdit4( "###smokecolori", unknowncheats::_settings->smoke_coloringol, ALPHA );

                                ImGui::Checkbox( "Remove smoke (!)", &unknowncheats::_settings->remove_smoke );
                                ImGui::Checkbox( "Flashbang builder (!)", &unknowncheats::_settings->flash_builder );
                                if ( !_settings->remove_full_flash ) {
                                    ImGui::SameLine( GetWindowWidth( ) - 33 );
                                    if ( custom.settings_widget( "##popup6" ) ) OpenPopup( "##popup6" );
                                    custom.prepared_popup( "##popup6", "Flashbang builder (!)", [ & ]( ) {
                                        ImGui::SliderFloat( "Flashbang delay (5<)", &unknowncheats::_settings->flash_time, 0.f, 5.f );
                                        ImGui::SliderFloat( "Flashbang alpha", &unknowncheats::_settings->flash_alpha, 0.f, 255.f );


                                                           } );
                                }

                                if ( unknowncheats::_settings->flash_builder )
                                    ImGui::Checkbox( "Remove flash completly (!)", &unknowncheats::_settings->remove_full_flash );
                                                   

                                ImGui::Checkbox( "Killed by headshot", &unknowncheats::_settings->killedby_hs );
                                ImGui::Checkbox( "Hitmarker", &unknowncheats::_settings->hitmarker );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                ImGui::ColorEdit4( "###hitmarkecol", unknowncheats::_settings->hitmarker_col, ALPHA );

                                ImGui::Checkbox( "Hitsound", &unknowncheats::_settings->hitsound );
                                if ( unknowncheats::_settings->hitsound )
                                    ImGui::Combo( "Sound", &unknowncheats::_settings->hitsound_type, hitsound.data( ), hitsound.size( ) );

                                ImGui::Checkbox( "Local sound esp", &unknowncheats::_settings->local_sound );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                if ( custom.settings_widget( "##popup4" ) ) OpenPopup( "##popup4" );
                                custom.prepared_popup( "##popup4", "Local sound esp", [ & ]( ) {
                                    ImGui::SliderFloat( "Animation###local", &unknowncheats::_settings->sound_animation_speed_l, 0.1, 0.3f );
                                    ImGui::SliderInt( "Sound range", &unknowncheats::_settings->local_range, 10, 100 );

                                                       } );
                                ImGui::SameLine( GetWindowWidth( ) - 55 );
                                ImGui::ColorEdit4( "###solc1231231ol", unknowncheats::_settings->sound_col_l, ALPHA );


          
                                ImGui::Checkbox( "Enemy sound esp", &unknowncheats::_settings->enemy_sound );
                                ImGui::SameLine( GetWindowWidth( ) - 33 );
                                if ( custom.settings_widget( "##popup5" ) ) OpenPopup( "##popup5" );
                                custom.prepared_popup( "##popup5", "Enemy sound esp", [ & ]( ) {
                                    ImGui::SliderFloat( "Animation###enemy", &unknowncheats::_settings->sound_animation_speed_e, 0.1, 0.3f );
                                    ImGui::SliderInt( "Sound range###enemysound", &unknowncheats::_settings->enemy_range, 10, 100 );
                                                       } );

                                ImGui::SameLine( GetWindowWidth( ) - 55 );
                                ImGui::ColorEdit4( "###solcol3e", unknowncheats::_settings->sound_col_e, ALPHA );

                            } custom.end_child( );

                            break;

                    }

                } EndChild( );
            } break;       
            case 2:
            {

            } break;

            case 3:
            {
                SetCursorPos( ImVec2( 65, 50 ) );
                BeginChild( "##rage_childs22", ImVec2( GetWindowWidth( ) - 80, GetWindowHeight( ) - 80 ) );
                {

                    switch ( custom.m_rage_subtab ) {

                        case 0:

                            custom.begin_child( "Configs", ImVec2( GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( ) ) );
                            {
                                static int configs = 0;
                                std::string config;
                                ImGui::Combo( "Config", &configs, cfgs.data( ), cfgs.size( ) );
                                switch ( configs ) {
                                    case 0: config = "default"; break;
                                    case 1: config = "legit"; break;
                                    case 2: config = "auto_hvh"; break;
                                    case 3: config = "scout_hvh"; break;
                                    case 4: config = "pistol_hvh"; break;
                                }

                                if ( Button( "Load" ) )
                                    unknowncheats::_settings->load( config );

                                if ( Button( "Save" ) )
                                    unknowncheats::_settings->save( config );

                            } custom.end_child( );

                            ImGui::SameLine( );

                            custom.begin_child( "Other", ImVec2( GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( ) ) );
                            {


                            } custom.end_child( );

                            break;

                    }

                } EndChild( );

            } break;
        }

        //ImGui::PopFont( );

        SetCursorPosY( 0 );
        custom.tab_area( "##tab_area", ImVec2( 50, size.y - 20 ), [ ]( ) {

            for ( int i = 0; i < custom.tabs.size( ); ++i )
                if ( custom.tab( custom.tabs_icons.at( i ), custom.tabs.at( i ), custom.m_tab == i ) && custom.m_tab != i )
                    custom.m_tab = i, custom.m_anim = 0.f;

                         } );

        // footer
        draw->AddRectFilled( pos + ImVec2( 0, size.y - 20 ), pos + size, ImColor( 15, 14, 21 ), style.WindowRounding, ImDrawFlags_RoundCornersBottom );
        draw->AddText( pos + ImVec2( 5, size.y - 18 ), GetColorU32( ImGuiCol_Text ), "unknowncheats.me - V1.0" );
        draw->AddText( pos + ImVec2( size.x - CalcTextSize( "Free Cheat" ).x - 5, size.y - 18 ), GetColorU32( ImGuiCol_Text ), "Free Cheat" );

    } ImGui::End( );

    ImGui::PopStyleVar( 2 );
}

void unknowncheats::menu_t::initialize( ) { 
    ImFontConfig font_config;
    font_config.PixelSnapH = false;
    font_config.FontDataOwnedByAtlas = false;
    font_config.OversampleH = 5;
    font_config.OversampleV = 5;
    font_config.RasterizerMultiply = 1.2f;

    static const ImWchar ranges[ ] = {

        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

    font_config.GlyphRanges = ranges;

    ImGuiIO& io = ImGui::GetIO( ); ( void )io;
    io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\tahoma.ttf", 14, &font_config, ranges );
    io.Fonts->AddFontFromMemoryTTF( icons_binary, sizeof icons_binary, 15, &font_config, ranges );
    io.Fonts->AddFontFromMemoryCompressedTTF( MenuIcons_compressed_data, MenuIcons_compressed_size, 11, &font_config, io.Fonts->GetGlyphRangesJapanese( ) );

}
