#pragma once

namespace unknowncheats {
	namespace mem {
		template <typename T> // get_data_with_offset
		inline bool scan_memory( std::string datatype, const DWORD64& address, DWORD offset, T& data ) {
			if ( address == 0 ) {
#ifdef read_data_dbg 
				print_with_data( datatype + " address is 0" );
#endif
				return false;
			}

			if ( !_proc_manager.read_memory<T>( address + offset, data ) ) {
#ifdef read_data_dbg 
				print_with_data( datatype + " cant process memory data" );
#endif
				return false;
			}

			return true;
		}
	}
}