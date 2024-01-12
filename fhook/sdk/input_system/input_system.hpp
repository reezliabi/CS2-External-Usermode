#pragma once

namespace unknowncheats {
	class input_t {
	public:
		int get_bind_id( int setting );
	};

	inline const auto _input_key = std::make_unique< input_t >( );
}