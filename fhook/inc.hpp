#pragma once
#pragma warning ( disable : 4005 )
#pragma warning ( disable : 26495 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4267 )
#pragma warning ( disable : 4715 )
#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4305 )

#include <windows.h>
#include <windowsx.h>
#include <memory>
#include <sstream>
#include <iostream>
#include <functional>
#include <WinUser.h>
#include <string_view>
#include <map>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <deque>
#include <mutex>
#include <optional>
#include <filesystem>
#include <fstream>
#include <random>
#include <array>
#include <mutex>

/* directx includes */
#include <d3d11.h>
#include <D3D11.h>

/* libs for directx */
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "freetype_debug.lib")

#include "thirdparty/imgui.h"
#include "thirdparty/imgui_freetype.h"
#include "thirdparty/imgui_impl_win32.h"
#include "thirdparty/imgui_impl_dx9.h"
#include "thirdparty/imgui_impl_dx11.h"

#define ext_draw_list ImGui::GetBackgroundDrawList( )
#define read_data_dbg
#define print_with_data(s) std::cout << "[unknowncheats-debug] " + s << std::endl;
#define print_with_data_scoped(s) std::cout << "[unknowncheats-debug] " + std::string(s) << std::endl;

#define custompi 3.14159265358979323846264338327950288f
#define rad_pi 57.295779513082f;

#define rad2deg(x) ((float)(x) * (float)(180.f / custompi))
#define deg2rad(x) ((float)(x) * (float)(custompi / 180.f))

/* inc */
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

#include "sdk/math/color_t.hpp"
#include "sdk/math/rect_t.hpp"
#include "sdk/math/str_t.hpp"
#include "sdk/render/render_sdk.hpp"

#include "sdk/process_manager/process_manager.hpp"
#include "sdk/memory/mem.hpp"
#include "sdk/offsets/offsets.hpp"
#include "sdk/classes/view.hpp"
#include "core/ctx/ctx.hpp"

#include "sdk/classes/bone_system.hpp"

#include "sdk/classes/entity.hpp"
#include "sdk/render/render.hpp"
#include "sdk/input_system/input_system.hpp"

#include "framework/config/cfg.hpp"
#include "hacks/features/esp/esp.hpp"
#include "hacks/features/triggerbot/triggerbot.hpp"
#include "hacks/features/legitbot/legitbot.hpp"
#include "hacks/features/ragebot/ragebot.hpp"
#include "hacks/features/sound_system/sound.hpp"
#include "hacks/features/flash_builder/flash_builder.hpp"
#include "hacks/features/shots/shots.hpp"
#include "hacks/features/grenade/grenades.hpp"

#include "hacks/ctx/hacks_ctx.hpp"
#include "framework/framework.hpp"

/* resources */
#define IDS_APP_TITLE 103
#define IDR_MAINFRAME 128
#define IDD_GUITEST_DIALOG 102
#define IDD_ABOUTBOX 103
#define IDM_ABOUT 104
#define IDM_EXIT 105
#define IDI_GUITEST 107
#define IDI_SMALL 108
#define IDC_GUITEST 109
#define IDC_MYICON 2
#ifndef IDC_STATIC
#define IDC_STATIC -1
#endif

#define ALPHA    ( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )
#define NO_ALPHA ( ImGuiColorEditFlags_NoTooltip    | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )