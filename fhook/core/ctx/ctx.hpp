#pragma once

namespace unknowncheats {
	class interface_t {
	public:
		bool initialize( );
	};

	class address_t {
	public:
		bool initialize( );

		view_t view;
	private:
		struct {
			DWORD64 server_dll;
			DWORD64 client_dll;
			DWORD64 entity_list;
			DWORD64 matrix;
			DWORD64 view_angle;
			DWORD64 entity_list_entry;
			DWORD64 local_controller;
			DWORD64 local_pawn;
			DWORD64 server_pawn;
			DWORD64 force_jump;
			DWORD64 global_vars;

		} game;
	public:
		DWORD64 get_client_dll_address( );
		DWORD64 get_server_dll_address( );
		DWORD64 get_entity_list_address( );
		DWORD64 get_matrix_address( );
		DWORD64 get_view_angle_address( );
		DWORD64 get_entity_list_entry( );
		DWORD64 get_local_controller_address( );
		DWORD64 get_local_pawn_address( );
		DWORD64 get_server_pawn_address( );
		DWORD64 get_global_vars_address( );

		bool update_entity_list_entry( );

		bool set_view_angle( float yaw, float pitch );

		bool set_force_jump( int value );
		bool get_force_jump( int& value );

	};

	inline const auto _interfaces = std::make_unique< interface_t>( );
	inline const auto _address = std::make_unique< address_t>( );
}