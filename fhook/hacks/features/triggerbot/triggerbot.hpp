#pragma once

namespace unknowncheats {
	class trigger_t {
	public:
		void run_trigger( const c_entity& local_entity );
		void run_thread_check( const c_entity& local_entity ) noexcept;
	};

	inline const auto _triggerbot = std::make_unique< trigger_t >( );
}