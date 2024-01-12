#pragma once

namespace unknowncheats {
	class flash_builder {
	public:
		void run_flash_builder( const c_entity& entity );
	};

	inline const auto _flash = std::make_unique< flash_builder >( );
}