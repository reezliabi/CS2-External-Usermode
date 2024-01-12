#pragma once

namespace unknowncheats {
	namespace framework {
		inline bool m_b_initialized = false;
		inline bool unloading = false;
		inline bool m_b_open = false;
		inline int m_i_width = 1920;
		inline int m_i_height = 1080;
		inline unsigned int m_u_refresh_rate = 60;

		inline ID3D11Device* p_device = nullptr;
		inline ID3D11DeviceContext* p_context = nullptr;
		inline IDXGISwapChain* p_swap_chain = nullptr;
		inline ID3D11RenderTargetView* p_render_target_view = nullptr;

		inline WNDCLASSEXW window_class = { };
		inline HWND instance{ nullptr };
		inline HMODULE h_moudle{ nullptr };	

		bool create( );
		bool render( );
		void destroy( );
	}

	class menu_t {
	public:
		void render( );
		void initialize( );
	};

	inline const auto _menu = std::make_unique< menu_t >( );
}
