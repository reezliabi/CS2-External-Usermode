#include "../../../inc.hpp"

void unknowncheats::flash_builder::run_flash_builder( const c_entity& entity ) { 
	if ( !_settings->flash_builder )
		return;

	if ( !_settings->remove_full_flash ) {
		float fl = _settings->flash_alpha;
		float fl_t = _settings->flash_time;
		_proc_manager.write_memory<float>( entity.player_pawn.address + offsets::pawn::max_alpha, fl ); /* xref: m_flFlashMaxAlpha */
		_proc_manager.write_memory<float>( entity.player_pawn.address + offsets::pawn::flash_Duration, fl_t ); /* xref: m_flFlashDuration */

	} else {
		// xref: m_flFlashBangTime 
		float fl = 0.f; 
		_proc_manager.write_memory<float>( entity.player_pawn.address + offsets::pawn::flashbang_time, fl );
	}
}
